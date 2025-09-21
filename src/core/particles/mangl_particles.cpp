/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_particles.h"

#include "../mangl_framework.h"

namespace mangl {

void ParticleSystem::setup(RectA layout, ScenePrimsContainerP prims, int maxParticles)
{
    layout_ = layout;
    lastTimestamp_ = Framework::timestamp();
    particlesCount_ = maxParticles;

    //particles_.reset ( new MParticle[maxParticles] ) ;
    particles_ = std::make_unique<Particle[]>(maxParticles);

    auto particle = particles_.get();
    memset(particle, 0, sizeof(Particle) * maxParticles);

    auto prim = prims->begin();
    for (int i = 0; i < maxParticles; ++i, ++particle, ++prim) {
        particle->prim = &*prim;
    }

    startParticleIndex_ = endParticleIndex_ = 0;
}


void ParticleSystem::onTimer(Timestamp timestamp)
{
    using PT = ParticleProgress::Type;

    if (startParticleIndex_ == endParticleIndex_) {
        lastTimestamp_ = timestamp;
        return;
    }

    auto deltaT = (timestamp - lastTimestamp_) * speedK_;
    auto layoutTop = layout_.top();
    auto layoutRight = layout_.right();

    auto expireParticle = [this](Particle *particle) {
        particle->prim->hide();
        particle->valid = false;
        updateBoundIndices();
    };

    auto particle = particles_.get() + startParticleIndex_;
    for (int pindex = startParticleIndex_; pindex < endParticleIndex_; ++pindex, ++particle) {
        if (!particle->valid)
            continue;
        if (particle->startTimestamp == 0) {
            particle->startTimestamp = timestamp;
            continue;
        }

        // Check if the particle expired 
        if (particle->lifespan != 0 && timestamp - particle->startTimestamp >= particle->lifespan) {
            if (particle->lifespanCallback)
                particleLifeExpireSignal(particle);
            expireParticle(particle);
            continue;
        }

        //auto particleT = (timestamp - particle->startTimestamp) * speedK_ ;

        // Update particle velocity
        particle->velocity += particle->acceleration * deltaT; //* deltaT) ;

        // Update particle gravity
        if (hasGravity_ && !particle->ignoreGravity) {
            auto vy = earthGravityAcceleration_ * deltaT; //particleT ;
            if (gravityPosition_ < particle->pos.y)
                vy = -vy;
            vy *= unitsPerMeter_;
            particle->velocity.y += vy;
            //particle->velocity.y = vy ;
        }

        // Update particle position
        particle->pos += particle->velocity * deltaT;

        // Check if the particle changes the size
        if (particle->sizeProgress.type != PT::None) {
            auto &progress = particle->sizeProgress;
            switch (progress.type) {
            case PT::Height:
                {
                    auto lambda = 1.;
                    auto finalPosition = progress.state + gravityPosition_;
                    if (particle->pos.y <= finalPosition) {
                        progress.type = PT::None;  //  we are done
                    } else {
                        lambda = 1 - (particle->pos.y - finalPosition) / (particle->startPosition.y - finalPosition);
                    }

                    if (lambda < 0)
                        break;
                    if (lambda > 1)
                        lambda = 1;

                    auto&& k = apl::lerp(1., progress.value, lambda);
                    particle->contentsFrame.w = particle->startSize.w * k;
                    particle->contentsFrame.h = particle->startSize.h * k;
                }
                break;
            default:
                break;
            }

            // update particle size
            particle->halfSize = particle->contentsFrame.size()/2;
        }

        particle->contentsFrame.move(particle->pos - particle->halfSize.point());

        // Check if the particle is out of bounds
        if (particle->contentsFrame.x >= layoutRight || particle->contentsFrame.y >= layoutTop || particle->contentsFrame.right() < layout_.x || particle->contentsFrame.top() < layout_.y) {
            expireParticle(particle);
            continue;
        }

        particle->prim->move(particle->contentsFrame);

        if (particle->callbackCondition != Callback::ParticleCallbackCondition::None) {
            switch (particle->callbackCondition) {
            case Callback::ParticleCallbackCondition::Time:
                if (timestamp - particle->startTimestamp >= particle->callbackTime) {
                    if (particleConditionCallback)
                        particleConditionCallback(particle);
                }
                break;
            default: break;
            }
        }


        /*
        // Update forces
        if ( !forces_.empty() ) {
            Point fv{} ;  // Resulting force vector
            for ( auto &fit: forces_ ) {
                //const double g = 0.000000000066742 ;
                const MFloat g = 0.066742 ;

                // distance square
                auto delta = fit->pos - particle->pos ;
                //auto dx = particle->pos.x - fit->pos.x, dy = particle->pos.y - fit->pos.y ;
                auto r2 = delta.x*delta.x + delta.y*delta.y ;
                if ( r2 < 0.0000001 )
                    continue ;

                auto f = g * particle->mass * fit->mass / r2 ;

                auto v = delta ;
                v /= std::sqrt(r2) ;
                v *= f ;

                fv += v ;
            }

            // calculate the particle acceleration
            particle->acceleration = fv * particle->mass ;
        }
        */
    }

    lastTimestamp_ = timestamp;
}


Particle *ParticleSystem::emit(RectA contentsFrame, Point startVelocity)
{
    auto particle = allocateParticle();
    if (particle == nullptr)
        return nullptr;

    // Fast zero the particle
    auto prim = particle->prim;
    *prim = {};
    *particle = {};

    particle->prim = prim;

    // TODO: maybe for the sake of speed in debugging mode replace method calls with the actual assignements

    prim->move(contentsFrame);
    prim->setVisible();

    particle->pos = contentsFrame.center();
    particle->size = contentsFrame.size();
    particle->contentsFrame = contentsFrame;
    particle->halfSize = particle->size / 2;
    particle->velocity = startVelocity;
    particle->valid = true;

    particle->startTimestamp = 0;
    particle->startPosition = particle->pos;
    particle->startSize = particle->size;

    if (hasParticleMass_)
        particle->mass = particleMass_;

    return particle;
}

Particle *ParticleSystem::allocateParticle() {
    // look through the "holes" inside the bound indices first
    auto particle = particles_.get() + startParticleIndex_;
    for (int index = startParticleIndex_; index < endParticleIndex_; ++index, ++particle)
        if (!particle->valid)
            return particle;

    if (startParticleIndex_ != 0) {
        --startParticleIndex_;
        return particles_.get() + startParticleIndex_;
    }

    if (endParticleIndex_ < particlesCount_) {
        ++endParticleIndex_;
        return particles_.get() + endParticleIndex_ - 1;
    }

    return 0;
}

void ParticleSystem::updateBoundIndices()
{
    auto particle = particles_.get();
    endParticleIndex_ = particlesCount_;

    for (startParticleIndex_ = 0; startParticleIndex_ < endParticleIndex_; ++startParticleIndex_, ++particle) {
        if (particle->valid)
            break;
    }

    if (startParticleIndex_ == endParticleIndex_) {
        startParticleIndex_ = endParticleIndex_ = 0;
        return;
    }

    for (particle = particles_.get() + endParticleIndex_ - 1; !particle->valid; --particle)
        --endParticleIndex_;
}

}  // namespace pentix
