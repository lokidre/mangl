/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"
#include "mangl_color_names.h"

namespace mangl {

struct Color {
    using T = RealGL;
    using Self = Color;
    using Ref = Self&;
    using Arg = const Color&;

    using Hex = uint32_t;

    T r{}, g{}, b{}, a{-1};

    constexpr Color() noexcept = default;
    constexpr Color(Real _r, Real _g, Real _b, Real _a = 1) noexcept: 
        r{static_cast<T>(_r)}, g{static_cast<T>(_g)}, b{static_cast<T>(_b)}, a{static_cast<T>(_a)} {}
    constexpr explicit(false) Color(Hex rgb) noexcept { setRGB(rgb); }
    constexpr Color(Hex rgb, Real _a) noexcept { setRGB(rgb, _a); }

    Color& clear() noexcept {
        r = g = b = 0;
        a = -1;
        return *this;
    }


    [[nodiscard]] constexpr bool valid() const noexcept { return a >= 0; }
    [[nodiscard]] constexpr bool equal(Arg c) const noexcept
        { return r == c.r && g == c.g && b == c.b && a == c.a; }


    constexpr Ref setRGB(Hex rgb) noexcept {
        constexpr T bmax{255};

        r = static_cast<T>((rgb >> 16) & 0xFF)/bmax;
        g = static_cast<T>((rgb >> 8) & 0xFF)/bmax;
        b = static_cast<T>(rgb & 0xFF)/bmax;
        a = 1;

        return *this;
    }

    constexpr Ref setRGB(Hex rgb, Real _a) noexcept {
        setRGB(rgb);
        a = static_cast<T>(_a);
        return *this;
    }


    constexpr auto& operator=(Hex rgb) noexcept {
        setRGB(rgb);
        return *this;
    }

    constexpr auto& setRGBA(Hex rgba) noexcept {
        constexpr T bmax{255};

        r = static_cast<T>((rgba >> 24) & 0xff)/bmax;
        g = static_cast<T>((rgba >> 16) & 0xff)/bmax;
        b = static_cast<T>((rgba >> 8) & 0xff)/bmax;
        a = static_cast<T>(rgba & 0xff)/bmax;

        return *this;
    }

    [[nodiscard]] constexpr T maxc() const noexcept {
        return r > g ? (r > b ? r : b) : (g > b ? g : b);
    }

    [[nodiscard]] constexpr T minc() const noexcept {
        return r < g ? (r < b ? r : b) : (g < b ? g : b);
    }

    [[nodiscard]] constexpr T hue() const noexcept {
        auto cmax = maxc();
        auto delta = cmax - minc();
        if (delta == 0)
            return 0;

        T h{};
        if (cmax == r)
            h = (g-b)/delta;
        else if (cmax == g)
            h = (b-r)/delta + 2;
        else
            h = (r-g)/delta + 4;

        h *= 60;
        while (h < 0)
            h += 360;
        while (h > 360)
            h -= 360;
        return h;
    }

    [[nodiscard]] constexpr auto brightness() const noexcept { return maxc(); }
    [[nodiscard]] constexpr auto lightness() const noexcept { return (maxc()+minc())/2; }

    [[nodiscard]] constexpr auto saturation() const noexcept  {
        auto cmax = maxc();
        if (cmax == 0)
            return cmax;
        auto delta = cmax - minc();
        return delta/cmax;
    }

    [[nodiscard]] constexpr auto sat() const noexcept { return saturation(); }

    // Returns the base color (without alpha)
    [[nodiscard]] constexpr auto base() const noexcept { return a == 1 ? *this : Color{r, g, b, 1}; }

    Ref setHSB(T hue, T sat, T v) noexcept {
        if (sat == 0) {
            r = g = b = v;
            return *this;
        }

        while (hue < 0)
            hue += 360;
        while (hue > 360)
            hue -= 360;
        hue /= 60;  // sectors
        int sector = static_cast<int>(hue);  // sector index
        auto f = hue - std::floor(hue);  // remaining
        auto p = v * (1 - sat);
        auto q = v * (1 - sat * f);
        auto t = v * (1 - sat * (1 - f));

        switch (sector) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
        default: break;;
        }

        return *this;
    }

    Color& adjustBrightness(Real deltad) noexcept  {
        auto d = static_cast<T>(deltad);
        r += d;
        g += d;
        b += d;
        return *this;
    }

    Color& setBrightness(Real brightness) noexcept {
        auto delta = brightness - maxc();
        return adjustBrightness(delta);
    }

    [[nodiscard]] Color adjustedBrightness(Real offset) const noexcept {
        Color c = *this;
        c.adjustBrightness(offset);
        return c;
    }


    [[nodiscard]] Real getAlpha() const noexcept { return (Real)a; }

    Color& setAlpha(Real alpha) noexcept {
        a = static_cast<T>(alpha);
        return *this;
    }

    Color& scaleAlpha(Real s) noexcept {
        a *= static_cast<T>(s);
        return *this;
    }

    [[nodiscard]] constexpr Color updAlpha(Real alpha) const noexcept { return {r, g, b, (T)alpha}; }
    [[nodiscard]] constexpr Color updA(Real alpha) const noexcept { return {r, g, b, (T)alpha}; }

    Color &adjustAlpha(Real delta) noexcept {
        a += static_cast<T>(delta);
        return *this;
    }
    [[nodiscard]] Color adjustedAlpha(Real delta) const noexcept {
        Self c(*this);
        c.adjustAlpha(delta);
        return c;
    }

    constexpr Color &clamp() noexcept {
        if (r < 0) r = 0;
        else if (r > 1) r = 1;

        if (g < 0) g = 0;
        else if (g > 1) g = 1;

        if (b < 0) b = 0;
        else if (b > 1) b = 1;

        if (a < 0) a = 0;
        else if (a > 1) a = 1;

        return *this;
    }


    [[nodiscard]] constexpr Hex getRGB() const noexcept {
        Hex u{};

        struct { int shift; const T &comp; } comp[] = {{0, b}, {8, g}, {16, r}};

        for (auto&& ci: comp) {
            auto c = (Hex)std::clamp(ci.comp * 255, static_cast<T>(0), static_cast<T>(255));
            u |= c << ci.shift;
        }

        return u;
    }

    [[nodiscard]] constexpr Hex getRGBA() const noexcept {
        Hex u{};

        struct { int shift; const T &comp; } comp[] = {{0, a}, {8, b}, {16, g}, {24, r}};
        
        for (auto&& ci: comp) {
            auto c = static_cast<Hex>(std::clamp(ci.comp * 255, T(0), T(255)));
            u |= c << ci.shift;
        }

        return u;
    }


    // [[deprecated]] constexpr Hex rgb() const {  return getRGB(); }
    // [[deprecated]] constexpr Hex rgba() const {  return getRGBA(); }


    constexpr Ref lerp(Arg v, Real td) noexcept {
        auto lambda = static_cast<T>(td);
        auto lambda_1 = 1 - lambda;
        r = r * lambda_1 + v.r * lambda;
        g = g * lambda_1 + v.g * lambda;
        b = b * lambda_1 + v.b * lambda;
        a = a * lambda_1 + v.a * lambda;
        return *this;
    }

    friend constexpr Color lerp(Arg a, Arg b, Real td) noexcept {
        auto t = static_cast<Color::T>(td);
        auto t1 = 1 - t;
        return {
            a.r * t1 + b.r * t,
            a.g * t1 + b.g * t,
            a.b * t1 + b.b * t,
            a.a * t1 + b.a * t
        };
    }

    //
    // Operator overloading
    //
    constexpr explicit operator bool() const noexcept { return valid(); }

    friend constexpr bool operator == (Arg lhs, Arg rhs) noexcept { return lhs.equal(rhs); }
    friend constexpr bool operator != (Arg lhs, Arg rhs) noexcept { return !(lhs == rhs); }

    constexpr auto& operator *= (Arg rhs) noexcept {
        r *= rhs.r, g *= rhs.g, b *= rhs.b, a *= rhs.a;
        return *this;
    }

};

using ColorA = Color::Arg;



struct ColorHashed: Color {
    using Hash = apl::Hash64;

    Hash hash{};

    constexpr ColorHashed() noexcept { updateHash(); }
    
    constexpr ColorHashed(Real ir, Real ig, Real ib, Real ia = 1) noexcept:
        Color{ir, ig, ib, ia} { updateHash(); }
    
    constexpr explicit(false) ColorHashed(Hex rgb) noexcept:
        Color{rgb} { updateHash(); }
    
    constexpr ColorHashed(Hex rgb, Real ia) noexcept:
        Color{rgb, ia} { updateHash(); }

    constexpr explicit(false) ColorHashed(ColorA clr) noexcept:
        Color{clr} { updateHash(); }


    constexpr ColorHashed& operator=(ColorA color) noexcept {
        Color::operator=(color);
        updateHash();
        return *this;
    }

    constexpr void updateHash() noexcept {
        hash = 0;
        apl::hashCombine(hash, r);
        apl::hashCombine(hash, g);
        apl::hashCombine(hash, b);
        apl::hashCombine(hash, a);

        // hash = apl::hashObject(*this);
    }
};


using ColorGradient = std::array<Color, 4>;
using ColorGradientA = const ColorGradient&;


}  // namespace mangl
