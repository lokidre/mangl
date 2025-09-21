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
#include "mangl_debug.h"

#include <apeal/parse.hpp>
#include <apeal/parse/geo.hpp>

#include "storage/mangl_sqlite.h"

namespace mangl {


//
// Three types of storage
//

template <class EngineT>
class StorageBase {
public:
    using KeyValCont = typename EngineT::KeyValCont;

    StorageBase() = default;

    void setLocation(StrView loc) { 
        location_ = loc; 
    }

    void shutdown() noexcept {
        try {
            close();
        } catch (...) {
        }
    }
    void open(StrView loc) { setLocation(loc), engine_.open(loc); }
    void close() { engine_.close(); }


    template <class T>
    void set(StrView key, const T& v) {
        checkEngine().writeString(key, apl::fmt("%z", v));
    }

    template <class... Args>
    void setf(StrView key, StrView fmt, Args&&... args) {
        set(key, apl::fmt("%z", std::forward<Args>(args)...));
    }

    template <class T>
    T get(StrView key, T def = {}) {
        auto s = checkEngine().readString(key);
        if (s.empty())
            return def;
        T val;
        apl::parse(s, &val, def);
        return val;
    }

    template <class T>
    void get(StrView key, T *val, T def = {}) {
        auto s = checkEngine().readString(key);
        if (s.empty()) {
            *val = def;
            return;
        }
        apl::parse(s, val, def);
    }

    KeyValCont readAll() { return checkEngine().readAll(); }

protected:
    EngineT engine_;
    String location_;

private:
    EngineT& checkEngine() {
        if (!engine_.isOpen())
            engine_.open(location_);
        return engine_;
    }
};


#if APEAL_OS_MAC

struct StorageEngineNSUserDefaults {
    using KeyValCont = KeyValCont<String, String>;

    KeyValCont readAll() {
        MDebugTrap();
        return {};
    }

    bool isOpen() noexcept { return true; }
    String readString(StrView key);
    void writeString(StrView key, StrView value);
    void open(StrView) {}
    void close() noexcept {}
};

using SettingsStorageEngine = StorageEngineNSUserDefaults;

#else

using SettingsStorageEngine = StorageEngineSQLiteSettings;

#endif

using StatsStorageEngine = StorageEngineSQLiteStats;


// Storage for user settings, various stats, etc.
// Uses UserDefaults on iOS, Qt Settings, Windows Registry, etc.
// Available immediately at instantiation
class SettingsStorage: public StorageBase<SettingsStorageEngine>, public apl::Singleton<SettingsStorage> {};

// Storage of various stats in key-value manner.
// Can store large amounts of data
class StatsStorage: public StorageBase<StatsStorageEngine>, public apl::Singleton<StatsStorage> {};


template <class T, class Storage>
class StorageItem: public apl::NonCopyable {
public:
    // using TP = const T &;
    using Arg = T; // const T&;

    //
    // Instantiation
    //
    StorageItem() noexcept = default; // no key, no action

    StorageItem(StrView k, Arg d) noexcept: ckey_{k}, def_{d} { key_ = ckey_; }
    explicit StorageItem(StrView k) noexcept: StorageItem{k, {}} {}

    StorageItem(StrView pref, StrView key, StrView suff, Arg def) noexcept
        : prefix_{pref}, ckey_{key}, suffix_{suff}, def_{def} {
        makeKey();
    }

    StorageItem(StrView p, StrView k, StrView s) noexcept: StorageItem(p, k, s, {}) {}

    StorageItem(StorageItem&& o) noexcept {
        using std::swap;
        swap(*this, o);
    }

    StorageItem& operator=(StorageItem&& o) noexcept {
        using std::swap;
        swap(*this, o);
        return *this;
    }


    //
    // Configuring
    //
    auto& setSuffix(StrView suff) noexcept {
        suffix_ = suff;
        loaded_ = false;
        makeKey();
        return *this;
    }

    auto& setPrefix(StrView pref) noexcept {
        prefix_ = pref;
        loaded_ = false;
        makeKey();
        return *this;
    }

    auto& setKey(StrView s) noexcept {
        ckey_ = s;
        loaded_ = false;
        makeKey();
        return *this;
    }

    const auto& key() const noexcept { return key_; }
    [[nodiscard]] bool hasKey() const noexcept { return !key_.empty(); }


    //
    // Access
    //
    void set(Arg v) {
        if (key_.empty()) {
            loaded_ = true;
            value_ = v;
            return;
        }

        Storage::instance()->set(key_, v);
        value_ = v;
        loaded_ = true;
    }

    auto set(StrView text) {
        T val;
        apl::parse(text, &val);
        this->set(val);
    }

    // Defining assignment operator like this allows chain assignment
    // const auto &operator = (Arg v) { this->set(v);  return value_; }
    auto get(Arg def) {
        if (loaded_)
            return value_;

        if (key_.empty()) // The user isn't using it
            return def;

        Storage::instance()->get(key_, &value_, def);
        loaded_ = true;
        return value_;
    }

    auto get() { return this->get(def_); }

    auto operator()() { return get(); }
    auto operator()(Arg def) { return get(def); }


    auto toggle() {
        set(!get());
        return get();
    }

    explicit operator bool() {
        if constexpr (std::is_same_v<T, bool>)
            return get();
        else if constexpr (std::is_scalar_v<T>)
            return !(get() == 0);
        else
            apl::throwError("Settings::operator bool() not implemented");
    }

    // increment
    int inc() {
        if constexpr (std::is_integral_v<T>) {
            auto current = get();
            current += 1;
            set(current);
            return current;
        } else {
            apl::throwError("Storage::inc() not implemented for non integral types");
            return -1;
        }
    }


protected:
    String key_;
    String prefix_, ckey_, suffix_;

    T def_{};
    bool loaded_{false};

    T value_{};

    void makeKey() { key_ = prefix_ + ckey_ + suffix_; }
};


template <class T>
class Setting: public StorageItem<T, SettingsStorage> {
    using Base = StorageItem<T, SettingsStorage>;
    using Base::Base;

public:
    using Base::operator bool;
    using Base::operator();

    // Defining assignment operator like this allows simpler chain assignment
    template <class Arg>
    Setting& operator = (Arg&& val) {
        this->set(std::forward<Arg>(val));
        return *this;
    }

};

template <class T>
class Stat: public StorageItem<T, StatsStorage> {
    using Base = StorageItem<T, StatsStorage>;
    using Base::Base;

public:
    using Base::operator bool;
    using Base::operator();

    // Defining assignment operator like this allows simpler chain assignment
    template <class Arg>
    Stat& operator = (Arg&& val) {
        this->set(std::forward<Arg>(val));
        return *this;
    }
};

} // namespace mangl
