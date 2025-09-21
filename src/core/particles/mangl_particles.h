/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_scene.h"

#include <apeal/signal.hpp>

#include <memory>

namespace mangl {

struct Callback {
    enum class ParticleCallbackCondition {
        None = 0,
        Time,
        Position,
        Distance,
        Collision,
    };
    Real value;
    Real state;
};


struct ParticleProgress {
    enum class Type {
        None = 0,
        Time,     // based on particles time
        Distance, // traveled distance
        Height    // distance from the gravity point
    } type;

    // Real progressK;  // value multiplied by the progression
    Real state; // at what point the final value should be achieved
    Real value; // the final value of the progression
};

struct Particle {
    PrimP prim;
    bool valid;
    Timestamp startTimestamp, currentTimestamp, lastTimestamp;
    Point startVelocity, startPosition;

    Timestamp lifespan;
    bool lifespanCallback; // whether to call the client code over life expiring

    Point pos;
    Size size, halfSize, startSize; // Particle bounds
    Rect contentsFrame;             // Particle contentsFrame
    Rect particlePos;
    Point velocity;
    Point acceleration;

    bool ignoreGravity;

    Real mass;

    ParticleProgress sizeProgress, alphaProgress, rotationProgress;

    Callback::ParticleCallbackCondition callbackCondition;
    Timestamp callbackTime;

    void* userData;
};

using ParticleConditionCallback = std::function<bool(Particle*)>;

class ParticleSystem: apl::NonCopyable {

public:
    void setup(RectA layout, ScenePrimsContainerP prims, int maxParticles);

    void setGravity(Cord pos) {
        hasGravity_ = true;
        gravityPosition_ = pos;
    }

    void setParticleMass(Real mass) {
        hasParticleMass_ = true;
        particleMass_ = mass;
    }

    void setUnitsPerMeter(Real upm) { unitsPerMeter_ = upm; }

    struct Force {
        Point pos;
        Real mass;
    };

    Force& addForce(Point pos, Real mass) {
        auto& f = forces_.emplace_back();
        f.pos = pos;
        f.mass = mass;
        return f;
    }

    void setSpeedK(Timestamp k) { speedK_ = k; }


public:
    Particle* emit(RectA contentsFrame, Point startVelocity);

    Particle* emit(Point pos, TextureA tex, Point startVelocity) {
        auto size = tex.size();
        return emit(tex, {pos.x - size.w / 2, pos.y - size.h / 2, size.w, size.h}, startVelocity);
    }

    Particle* emit(TextureA tex, RectA frame, Point startVelocity) {
        if (auto particle = emit(frame, startVelocity); particle) {
            particle->prim->setTexture(tex);
            return particle;
        }
        return nullptr;
    }

public:
    void onTimer(Timestamp timestamp);


    //
    // Callbacks
    //
    ParticleConditionCallback particleConditionCallback = nullptr;

    using ParticleSignal = apl::Signal<Particle*>;

    ParticleSignal particleLifeExpireSignal;

private:
    std::unique_ptr<Particle[]> particles_;
    int particlesCount_{};
    Rect layout_{};

    int startParticleIndex_{}, endParticleIndex_{};

    Timestamp lastTimestamp_{0};
    Timestamp speedK_{1};

    bool hasGravity_{false};

    Cord gravitationalConstant_ = 0.000066742;
    Cord gravitationConstantScale_ = 1.0/1000000.0;
    Cord earthGravityAcceleration_ = 9.807; // m/s^2
    Cord sunGravityAcceleration_ = 27.96 * earthGravityAcceleration_;
    Cord mercuryGravityAcceleration_ = 0.378 * earthGravityAcceleration_;
    Cord venusGravityAcceleration_ = 0.907 * earthGravityAcceleration_;
    Cord moonGravityAcceleration_ = 0.166 * earthGravityAcceleration_;
    Cord marsGravityAcceleration_ = 0.377 * earthGravityAcceleration_;
    Cord jupiterGravityAcceleration_ = 2.36 * earthGravityAcceleration_;
    Cord saturnGravityAcceleration_ = 0.916 * earthGravityAcceleration_;
    Cord uranusGravityAcceleration_ = 0.889 * earthGravityAcceleration_;
    Cord neptuneGravityAcceleration_ = 1.12 * earthGravityAcceleration_;
    Cord plutoGravityAcceleration_ = 0.071 * earthGravityAcceleration_;

    Cord gravityPosition_{};

    bool hasParticleMass_{false};
    Real particleMass_{};

    Cord unitsPerMeter_ = 1;

    Particle* allocateParticle();
    void updateBoundIndices();

    // 000`000`000
    std::vector<Force> forces_;
};

} // namespace mangl
