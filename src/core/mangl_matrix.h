/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <cognit/mtx.hpp>
#include <cognit/mtx/mtx_mul.hpp>

#include <cognit/geo/vector/mtx_vct_mul.hpp>

namespace mangl {

template <class ValueT>
struct MatrixBase: cognit::Mtx4<ValueT> {
    using T = ValueT;
    using Self = MatrixBase;
    using SelfA = const Self&;
    using Base = cognit::Mtx4<T>;

    using Base::eye;
    using Base::a;
    using Base::zero;

    MatrixBase& operator = (const Base& b) {
        this->assign(b);
        return *this;
    }

    //
    // Initialization
    //
    MatrixBase& translation(Point3DA u) {
        eye();
        a[0][3] = static_cast<T>(u.x);
        a[1][3] = static_cast<T>(u.y);
        a[2][3] = static_cast<T>(u.z);
        return *this;
    }

    static auto makeTranslation(Point3DA u) {
        Self m;
        m.translation(u);
        return m;
    }

    Self& scaling(Cord z) {
        eye();
        a[3][3] = 1/(T)z;
        return *this;
    }

    //
    // Manipulations
    Self& translate(Point3DA p) {
        a[0][3] += (T)p.x;
        a[1][3] += (T)p.y;
        a[2][3] += (T)p.z;
        return *this;
    }

    Self& zoom(Cord z) {
        a[3][3] *= static_cast<T>(1/z);
        return *this;
    }

    // builds scaling matrices

    auto& rotationX(Cord cs, Cord sn) {
        zero();

        a[0][0] =  1;
        a[1][1] =  static_cast<T>(cs);
        a[1][2] = -static_cast<T>(sn);
        a[2][1] =  static_cast<T>(sn);
        a[2][2] =  static_cast<T>(cs);
        a[3][3] =  1;

        return *this;
    }

    auto& rotationX(Cord rho) { return rotationX(cos(rho), sin(rho)); }

    auto& rotationY(Cord rho) noexcept
    {
        auto cs = std::cos((T)rho), sn = std::sin((T)rho);

        zero();

        a[0][0] =  cs;
        a[0][2] = -sn;
        a[1][1] =  1;
        a[2][0] =  sn;
        a[2][2] =  cs;
        a[3][3] =  1;

        return *this;
    }

    auto& rotationZ(Cord rho) noexcept
    {
        auto cs = std::cos((T)rho);
        auto sn = std::sin((T)rho);

        zero();

        a[0][0] =  cs;
        a[0][1] =  sn;
        a[1][0] = -sn;
        a[1][1] =  cs;
        a[2][2] =  1;
        a[3][3] =  1;

        return *this;
    }

    static Self rotationZMatrix(Cord theta) noexcept {
        Self m;
        m.rotationZ(theta);
        return m;
    }


    // Builds orthographic matrix
    // parameters: left, right, bottom, top, near, far
    auto& ortho(Cord l, Cord r, Cord b, Cord t, Cord n, Cord f) {
        zero();

        a[0][0] =  2 / T(r-l);
        a[1][1] =  2 / T(t-b);
        a[2][2] = -2 / T(f-n);
        a[3][3] =  1;

        a[0][3] = -T(r+l)/T(r-l);
        a[1][3] = -T(t+b)/T(t-b);
        a[2][3] = -T(f+n)/T(f-n);

        return *this;
    }

    // parameters: left, right, bottom, top, near, far
    Self& perspective(Cord l, Cord r, Cord b, Cord t, Cord n, Cord f) 
    {
        zero();

        a[0][0] = static_cast<T>(2*n / (r-l));
        a[0][2] = static_cast<T>((r+l) / (r-l));

        a[1][1] = static_cast<T>(2*n / (t-b));
        a[1][2] = static_cast<T>((t+b) / (t-b));

        a[2][2] = static_cast<T>(-(f+n) / (f-n));
        a[2][3] = static_cast<T>(-2*f*n / (f-n));

        a[3][2] = -1;

        return *this;
    }

    auto& perspective(Cord fov, Cord aspect, Cord n, Cord f) noexcept
    {
        zero();

        auto ff = static_cast<Cord>(1) / std::tan(fov/2);

        a[0][0] = static_cast<T>(ff/aspect);
        a[1][1] = static_cast<T>(ff);
        a[2][2] = static_cast<T>((f+n)/(f-n));
        a[2][3] = static_cast<T>(2*f*n / (f-n));
        a[3][2] = -1;

        return *this;
    }

    // Multiply by vector u: v = M*u
    //auto mulled(Point3DA v) const {
    //    return cognit::mtx4x4_vct3_mul(*this, v.convert<apl::Point3D<T>>());
    //}
    [[nodiscard]]
    auto mulled(const apl::Point3D<T>& v) const noexcept {
        return cognit::mtx4x4_vct3_mul(*this, v);
    }

    // Multiply by matrix
    [[nodiscard]]
    auto mulled(const Self& B) const noexcept {
        Self m;
        cognit::mtx_mul(*this, B, m);
        return m;
    }


    friend auto operator * (SelfA m, const apl::Point3D<T>& v) noexcept {
        return m.mulled(v);
    }

    friend auto operator * (SelfA a, SelfA b) noexcept {
        return a.mulled(b);
    }

};


using Matrix = MatrixBase<Cord>;
using MatrixGL = MatrixBase<RealGL>;

using MatrixA = const Matrix&;
using MatrixGLA = const MatrixGL&;


template <class BaseMatrix>
struct MatrixHashedBase: BaseMatrix {
    using Hash = apl::Hash64;
    Hash hash{};
    
    void updateHash() noexcept {
        hash = BaseMatrix::computeHash();
    }
    
    auto& operator=(const BaseMatrix& b) noexcept {
        this->assign(b);
        updateHash();
        return *this;
    }
};

struct MatrixHashed: MatrixHashedBase<Matrix> {
    using Base = MatrixHashedBase;
    using Base::operator=;
};

struct MatrixHashedGL: MatrixHashedBase<MatrixGL>{
    using Base = MatrixHashedBase;
    using Base::operator=;
};

using MatrixHashedGLP = MatrixHashedGL*;


}  // namespace mangl

