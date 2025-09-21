/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

//
// Declarations of all the basic types
//
#include "mangl_setup.h"

#include <apeal/assert.hpp>
#include <apeal/geo.hpp>
#include <apeal/singleton.hpp>
#include <apeal/val_range.hpp>
#include <apeal/signal.hpp>
#include <apeal/hash.hpp>
#include <apeal/data_param.hpp>

#include <apeal/parse/float.hpp>


#include <cognit/vec.hpp>
#include <cognit/mtx.hpp>

#include <type_traits>

#include <cstdint>
#include <string>
#include <functional>
#include <mutex>
#include <condition_variable>


#include <array>
#include <map>
#include <vector>
#include <unordered_map>


namespace mangl {

//using Real = float;
using Real = double;

using RealGL = float;  // Graphics rendering real value

constexpr Real operator"" _mr(unsigned long long x) { return (Real)x;  }
constexpr Real operator"" _mr(long double x) { return (Real)x;  }

namespace M {
constexpr Real Zero{0};
constexpr Real Half{0.5};
constexpr Real One{1};
}  // namespace M

using apl::Index;

using Timestamp = Real;

// 
// Definitions of various Ids and keys
using IdCommon32 = std::uint_fast32_t;
using IdCommon64 = std::uint_fast64_t;

using CommonId = int; //IdCommon64;  // Used as general id
using Flags = std::uint_fast64_t;
using WidgetId = CommonId;
using MusicTrackId = CommonId;
using SfxId = CommonId;
using TextureId = CommonId;

using PeerKey = std::uint_fast32_t;

namespace net {
enum class ConnectionType: char {
    Unknown = 0,
    Local = 'L',
    Nearby = 'N',
    Online = 'O',
};
}  // namespace net



// Geometric primitives
using Cord = Real;
using Coord = Real;
using CordI = int;
using CoordI = int;
using CordGL = RealGL;

namespace M {

constexpr Real InvalidCord = std::numeric_limits<Real>::quiet_NaN();
constexpr Real InvalidCoord = InvalidCord;

constexpr CordI InvalidCordI = std::numeric_limits<CordI>::min();
constexpr CordI InvalidCoordI = InvalidCordI;


inline bool isInvalidCord(Real v) {  return std::isnan(v); }
inline bool isInvalidCordI(Real v) {  return InvalidCordI == v; }

inline bool isValidCord(Real v) {  return !isInvalidCord(v); }
inline bool isValidCordI(Real v) {  return !isInvalidCord(v); }

//constexpr Cord InvalidCord{std::numeric_limits<Cord>::min()};
//constexpr Cord InvalidCordI{std::numeric_limits<CordI>::min()};
}  // namespace M

using apl::String;
using apl::StringA;

using apl::StrView;


using Char = apl::Char;

// Obsolete
using Size = apl::Size<Cord>;
using SizeA = Size;

using SizeI = apl::Size<int>;
using SizeIA = SizeI;

using SizeGL = apl::Size<RealGL>;

using Point = apl::Point<Cord>;
using PointA = Point;  // Points passed by value in 64 bit systems

using PointI = apl::Point<int>;
using PointIA = PointI;


using PointGL = apl::Point<RealGL>;

using Vec2 = apl::Point<Cord>;
using Vec3 = apl::Point3D<Cord>;
using Vec4 = cognit::Vec4<Cord>;

using Point3D = apl::Point3D<Cord>;
using Point3DA = const Point3D&;
using Point3DI = apl::Point3D<CordI>;
using Point3DGL = apl::Point3D<RealGL>;


using Rect = apl::Rect<Real>;
using RectA = const Rect&;  // 4 doubles are passed by reference, 4 floats by value


using MRect = Rect;  // For Apple's OSes
using MPoint = Point;  // For Apple's OSses
using MSize = Size;  // For Apple's OSes

using RectGL = apl::Rect<RealGL>;


using RectI = apl::Rect<CordI>;
using RectIA = const RectI&;

using CordRange = apl::ValRange<Cord>;
using CordRangeI = apl::ValRange<CordI>;

using Margins = apl::Margins<Cord>;
using MarginsA = apl::Margins<Cord>::Arg;

using Padding = Margins;
using PaddingA = MarginsA;


using Hash = apl::Hash64;

template <class ...Args>
using Signal = apl::Signal<Args...>;

template <class ...Args>
using Slot = apl::Slot<Args...>;

//template <class ...Args>
//using Signal = apl::Signal<Args...>;
//
//template <class ...Args>
//using MSlot = typename Signal<Args...>::Func;

using SignalVoid = apl::Signal<>;
using SlotVoid = typename apl::Signal<>::Func;
using SlotIdVoid = typename Signal<CommonId>::Func;


using SignalId = Signal<CommonId>;

using SignalInt = Signal<int>;
using SignalIdInt = Signal<CommonId, int>;
using SlotInt = Signal<int>::Func;
using SlotIdInt = Signal<CommonId, int>::Func;


using SignalBool = Signal<bool>;
using SignalIdBool = Signal<CommonId, bool>;
using SlotBool = Signal<bool>::Func;
using SlotIdBool = Signal<CommonId, bool>::Func;

using SignalReal = Signal<Real>;
using SignalIdReal = Signal<CommonId, Real>;
using SlotReal = Signal<Real>::Func;
using SlotIdReal = Signal<CommonId, Real>::Func;

using SignalString = Signal<StringA>;
using SignalIdString = Signal<CommonId, StringA>;
using SlotString = Signal<StringA>::Func;
using SlotIdString = Signal<CommonId, StringA>::Func;

using SignalStrView = Signal<StrView>;
using SignalIdStrView = Signal<CommonId, StrView>;
using SlotStrView = Signal<StrView>::Func;
using SlotIdStrView = Signal<CommonId, StrView>::Func;



// Pass by value since data param has only pointer and size
using DataParam = apl::DataParam<>;
using DataParamA = apl::DataParam<>::Arg;

// Containers
template <class Key, class Value>
using KeyValCont = std::unordered_map<Key, Value>;

struct StringHash {
    using is_transparent = void;
    using hash_type = std::hash<std::string_view>;
    std::size_t operator()(std::string_view str) const   { return hash_type{}(str); }
    std::size_t operator()(const std::string& str) const { return hash_type{}(str); }
    std::size_t operator()(const char* str) const        { return hash_type{}(str); }
};


//#if __cpp_lib_generic_unordered_lookup
//
//template <class Value>
//using StrValCont = std::unordered_map<MString, Value, StringHash, std::equal_to<>>;
//
//#else



// Enabling string_view for unorderd_map until it's implemented in c++20
// Fix hashing algorithm to be aligned to 32.
// It crashes on Android 32 bits.
#if 1
// std::map based string/value container

template <class Value>
class StrValCont: public std::map<String, Value> {
    using Key = String;
    using Base = std::map<Key, Value>;
    using Iterator = typename Base::iterator;
    using ConstIterator = typename Base::const_iterator;

    constexpr auto hash(StrView key) const noexcept { return String(key); }

public:
    using typename Base::const_iterator;

    bool contains(StrView key) const noexcept { return Base::find(hash(key)) != Base::end(); }

    auto find(StrView key) noexcept { return Base::find(hash(key)); }

    auto find(StrView key) const noexcept { return Base::find(hash(key)); }

    auto& operator[](StrView key) noexcept { return Base::operator[](hash(key)); }

    auto emplace(StrView key, Value&& val) {
        return Base::emplace(hash(key), std::forward<Value>(val));
    }

    auto emplace(StrView key, const Value& val) {
        return Base::emplace(hash(key), val);
    }


    auto insert(const_iterator hint, const std::pair<StrView, Value>& val) {
        return Base::insert(hint, {hash(val.first), val.second});
    }

    auto insert(const std::pair<StrView, Value>& val) {
        return Base::insert({hash(val.first), val.second});
    }

    //template <class V>
    //auto insert(iterator hint, V&& val) { return Base::insert(hint, std::forward<V>(val)); }


    auto erase(StrView key) noexcept { return Base::erase(hash(key)); }
};
#endif


#if 0
// Based on unordered_map 64-bit hash container
template <class Value>
//class StrValCont: public std::unordered_map<apl::Hash64, Value> {
class StrValCont: public std::map<apl::Hash64, Value> {
    using Key = apl::Hash64;
    //using Base = std::unordered_map<Key, Value>;
    using Base = std::map<Key, Value>;
    using Iterator = typename Base::iterator;
    using ConstIterator = typename Base::const_iterator;
    //using Hash = StringHash; //std::hash<std::string_view>;


    //constexpr auto hash(StrView key) const noexcept {  return apl::hashAsciiHorner(key);  }
    //constexpr auto hash(StrView key) const noexcept {  return std::hash<MStringView>{}(key);  }
    constexpr auto hash(StrView key) const noexcept { return apl::hashDataFnv(key); }

public:
    using typename Base::const_iterator;

    bool contains(StrView key) const noexcept {  return Base::find(hash(key)) != Base::end();  }

    auto find(StrView key) noexcept { return Base::find(hash(key)); }

    auto find(StrView key) const noexcept { return Base::find(hash(key)); }

    auto &operator[](StrView key) noexcept { return Base::operator[](hash(key)); }

    auto emplace(StrView key, Value&& val) {
        return Base::emplace(hash(key), std::forward<Value>(val)); 
    }

    auto emplace(StrView key, const Value& val) {
        return Base::emplace(hash(key), val);
    }


    auto insert(const_iterator hint, const std::pair<StrView, Value> &val) {
        return Base::insert(hint, {hash(val.first), val.second});
    }

    auto insert(const std::pair<StrView, Value> &val) {
        return Base::insert({hash(val.first), val.second});
    }

    //template <class V>
    //auto insert(iterator hint, V&& val) { return Base::insert(hint, std::forward<V>(val)); }


    auto erase(StrView key) noexcept { return Base::erase(hash(key)); }
};
#endif

#if 0 // backup
template <class Value>
class StrValCont: public std::unordered_map<String, Value> {
    using Base = std::unordered_map<String, Value>;
    using Iterator = typename Base::iterator;
    using ConstIterator = typename Base::const_iterator;

public:
    auto find(StrView key) { return Base::find(String{key}); }

    auto find(StrView key) const { return Base::find(String{key}); }

    Value &operator[](StrView key) { return Base::operator[](String{key}); }
};
#endif


template <class T>
struct StringPointerContainer: apl::NonCopyable {
    using Key = StrView;
    using Hash = apl::Hash64;

    // Instantiation/Destruction
    ~StringPointerContainer() noexcept {
        clear();
    }

    // Utilities
    static auto hash(Key key) noexcept {
        return apl::hashAsciiHorner(key);
    }

    // Data insertion/removal
    void erase(Key key) noexcept { 
        auto it = data_.find(key);
        if (it != data_.end()) {
            freeItem(it.second);
            data_.erase(it);
        }
    }

    void clear() noexcept {
        for (auto& it: data_)
            delete it.second;
        data_.clear();
    }


    auto add(Key key) noexcept {
        auto obj = allocItem();
        data_.insert({hash(key), obj});
        return obj;
    }

    //
    // Searching, access
    //
    bool containsByHash(Hash hash) const noexcept {
        return data_.contains(hash);
    }

    bool contains(Key key) const noexcept { return containsByHash(hash(key)); }


    T* getByHash(Hash hash) noexcept {
        if (auto it = data_.find(hash); it != data_.end())
            return it->second;
        return nullptr;
    }

    const T* getByHash(Hash hash) const noexcept {
        if (auto it = data_.find(hash); it != data_.end())
            return it->second;
        return nullptr;
    }


    auto get(Key key) noexcept { return getByHash(hash(key)); }
    auto get(Key key) const noexcept { return getByHash(hash(key)); }


    auto begin() noexcept { return data_.begin(); }
    auto begin() const noexcept { return data_.begin(); }

    auto end() noexcept { return data_.end(); }
    auto end() const noexcept { return data_.end(); }

    auto ssize() const noexcept { return std::ssize(data_); }
    auto size() const noexcept { return std::size(data_); }
    auto empty() const noexcept { return std::empty(data_); }

private:
    std::unordered_map<apl::Hash64, T*> data_;

    T* allocItem() {  return new T; }
    void freeItem(T* p) noexcept { delete p; }
};




template <class Value>
using VectorCont = std::vector<Value>;


// Mutex
using Mutex = std::mutex;
//using ScopedLock = std::lock_guard<MMutex>;
using ScopedLock = std::scoped_lock<Mutex>;

using UniqueLock = std::unique_lock<Mutex>;
using ConditionVar = std::condition_variable;


namespace M {

constexpr auto DefaultTextureAtlasExt = "yaml";
constexpr auto DefaultTextureImageExt = "png";


template <class Cont, class Key>
auto& mapItem(Cont& c, Key&& key) noexcept {
    auto it = c.find(key);
    ADebugTrapAssert(it != c.end());
    return it->second;
}

template <class Cont, class Key>
const auto& mapItem(const Cont& c, Key&& key) noexcept {
    auto it = c.find(key);
    ADebugTrapAssert(it != c.end());
    return it->second;
}


//template <class Container, class ...Args>
//auto& mapItem(Container& map, MStringView key, Args&&...msg) {
//    auto it = map.find(MString{key});  // TODO: remove when implemented
//    apl::verify(it != map.end(), std::forward<Args>(msg)...);
//    return it->second;
//}


template <class Cont, class Key, class ...Args>
auto& mapItem(Cont& dict, Key&& key, Args&&...msg)  {
    auto&& it = dict.find(std::forward<Key>(key));
    apl::verify(it != dict.end(), std::forward<Args>(msg)...);
    return it->second;
}

template <class Container, class Key, class ...Args>
const auto& mapItem(const Container& dict, Key&& key, Args&&...msg)  {
    auto&& it = dict.find(std::forward<Key>(key));
    apl::verify(it != dict.end(), std::forward<Args>(msg)...);
    return it->second;
}



inline auto parseCoord(StrView s) { return apl::parseReal<Cord>(s); }
inline auto parseCoord(StrView s, Cord def) noexcept { return apl::parseReal<Cord>(s, def); }

inline auto parseCord(StrView s) { return apl::parseReal<Cord>(s); }
inline auto parseCord(StrView s, Cord def) noexcept { return apl::parseReal<Cord>(s, def); }

inline auto parseRealGL(StrView s) { return apl::parseReal<RealGL>(s); }
inline auto parseRealGL(StrView s, RealGL def) noexcept { return apl::parseReal<RealGL>(s, def); }

}  // namespace M


// Some networking declarations
enum class PeerState {
    InvalidPeerState = 0,

    Found           = 'F',
    Accepted        = 'A',
    Lost            = 'L',
    Error           = 'E',
    
    Connecting      = '~',
    
    Connected       = 'C',
    Disconnected    = 'D',
};

constexpr bool operator !(PeerState s) noexcept { return !static_cast<int>(s); }
constexpr bool connected(PeerState s) noexcept { return PeerState::Connected == s; }

namespace M {
constexpr inline auto getPeerStateString(PeerState st) {
    switch (st) {
        using enum PeerState;
    case Found: return "Found";
    case Accepted: return "Accepted";
    case Lost: return "Lost";
    case Error: return "Error";
    case Connecting: return "Connecting";
    case Connected: return "Connected";
    case Disconnected: return "Disconnected";
    default: return "UNKNOWN";
    }
}

Cord sizeCord(const std::ranges::range auto& container) {
    return static_cast<Cord>(std::ssize(container));
}

}  // namespace mangl::M


enum class PeerType {
    InvalidPeerType = 0,

    Network         = 'N',
    Host            = 'H',
    Local           = 'L',
    Routed          = 'R',
};

constexpr bool operator !(PeerType s) { return !int(s); }


enum class InterfaceIdiom: char {
    Unknown = 0, 
    Phone = 'P', 
    Pad = 'T',
    Tablet = 'T',
    Desktop = 'D',
};

enum class ScreenOrientation: char {
    Unknown = 0,
    FaceDown = 'D',
    FaceUp = 'U',
    Portrait = 'P',
    PortraitUpside = 'S',
    Landscape = 'L',
    LandscapeLeft = Landscape,
    LandscapeRight = 'R',
};

enum class DeviceType: char { 
    Unknown = 0, 
    Phone = 'P', 
    Pad = 'T', 
    Tablet = 'T', 
    Desktop = 'D',
};




namespace M {

//
// Flags
//

constexpr auto Visible      = 1<<0;
constexpr auto Hidden       = 1<<1;

constexpr auto AlignLeft    = 1<<2;
constexpr auto AlignCenter  = 1<<3;
constexpr auto AlignRight   = 1<<4;
constexpr auto AlignTop     = 1<<5;
constexpr auto AlignVCenter = 1<<6;
constexpr auto AlignBottom  = 1<<7;

constexpr auto AlignMask = (AlignLeft | AlignCenter | AlignRight | AlignTop | AlignVCenter | AlignBottom);


constexpr auto AlignJustified       = AlignLeft | AlignRight;

constexpr auto AlignCenterAll       = AlignCenter | AlignVCenter;
constexpr auto AlignCenterCenter    = AlignCenter | AlignVCenter;
constexpr auto AlignCenterTop       = AlignCenter | AlignTop;
constexpr auto AlignCenterBottom    = AlignCenter | AlignBottom;

constexpr auto AlignLeftTop         = AlignLeft | AlignTop;
constexpr auto AlignRightTop        = AlignRight | AlignTop;

constexpr auto AlignLeftCenter      = AlignLeft | AlignVCenter;
constexpr auto AlignRightCenter     = AlignRight | AlignVCenter;

constexpr auto AlignLeftBottom      = AlignLeft | AlignBottom;
constexpr auto AlignRightBottom     = AlignRight | AlignBottom;



enum OrientationFlag {
    OrientationHorz = 0x00000100,
    OrientationVert = 0x00000200,
};

enum {
    TiledHorz = 0x00040000,
    TiledVert = 0x00080000,
    Tiled = TiledHorz | TiledVert,

    FlipVert = 0x00100000,
    FlipHorz = 0x00200000,
};



enum Orientation {
    Orientation0 = 0,
    Orientation90 = 1,
    Orientation180 = 2,
    Orientation270 = 3,
};

inline
Orientation inc(Orientation o) noexcept {
    switch (o) {
    case Orientation0:   return Orientation90;
    case Orientation90:  return Orientation180;
    case Orientation180: return Orientation270;
    case Orientation270: return Orientation0;
    }
    return o;
}

inline
Orientation dec(Orientation o) noexcept {
    switch (o) {
    case Orientation0:   return Orientation270;
    case Orientation90:  return Orientation0;
    case Orientation180: return Orientation90;
    case Orientation270: return Orientation180;
    }
    return o;
}


} // namespace M

} // namespace mangl


