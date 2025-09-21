/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

#include "mangl_layout_parser.h"
#include "../mangl_prim.h"

namespace mangl {


template <class T>
struct MItemValue {
    bool flag = false;
    T value;
    
    void set(const T& v) noexcept {
        value = v;
        flag = true;
    }
    void operator = (const T& v) noexcept {
        set(v);
    }

    T &operator () () noexcept {
        return value;
    }
};

struct MLayoutItem {
    String id;

    MItemValue<MLayoutPoint> position;
    MItemValue<MLayoutPoint> origin;

    MItemValue<Point> axis;
    //bool axis_flag = false ;
    //Point axis ;

    MItemValue<MLayoutAlign> align;
    //bool align_flag = false ;
    //int align_horz = M::AlignLeft, align_vert = M::AlignBottom ;

    MItemValue<MLayoutSize> size;
    //bool size_flag = false ;
    //MLayoutSize size ;

    MItemValue<Real> alpha;
    //bool alpha_flag = false ;
    //MFloat alpha = 1.0f ;

    bool texture_flag = false;
    String textureName;
    int textureIndex = 0;

    MItemValue<Real> rotation;
    //bool rotation_flag = false ;
    //MFloat rotation_value = 0 ;
};

class MLayout: public MLayoutParser {
    using Base = MLayoutParser;

private:
    using ItemList = std::vector<MLayoutItem>;

    std::map<String, int> itemsIndex_;
    ItemList items_;

    void clear()
    {
        Base::clear();

        itemsIndex_.clear();
        items_.clear();
    }


public:

    //
    // Loading and configuration
    //
    void preload(StrView filename);
    void load(StrView filename);

    void update(Prim *prim, MLayoutItem &item);

public:
    auto& items() { return items_; }

    auto& item(StringA name) const
    {
        return items_.at(itemsIndex_.at(name));
        //const auto it = itemsIndex_.find(name) ;
        //if ( it == itemsIndex_.end() )
        //	throw std::runtime_error("cannot find layout item: " + name) ;
        //return items_[it->second] ;
    }

    auto& operator()(StringA name) const { return item(name); }

};

}  // namespace mangl
