/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
 /*** Automatically Generated *** DO NOT EDIT ***
 * Generated Date: 2024-07-18
 */
#pragma once

#include "sample_game_packet.hpp"

#include <apeal/index.hpp>
#include <mangl/packet.h>

namespace sample::mp {

inline void pack(const SampleGamePacket& obj, mangl::PacketPacker* packer);
inline void unpack(mangl::PacketLoader& loader, SampleGamePacket* obj);


inline void clear(SampleGamePacket* obj);


inline auto packStringPacket(const SampleGamePacket& obj) {
    mangl::PacketPacker packer;
    pack(obj, &packer);
    std::string packet{packer.makeStdStringView()};
    return packet;
}

inline auto unpackStringPacket(std::string_view packet, SampleGamePacket* obj) {
    mangl::PacketLoader loader;
    loader.load(packet);
    unpack(loader, obj);
}



namespace impl_SampleGamePacket_packet {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(SampleGamePacket* obj);
inline bool isEmptyObject(const SampleGamePacket& obj);
inline void clearObject(mmp::MpPacketPlayer* obj);
inline bool isEmptyObject(const mmp::MpPacketPlayer& obj);
inline void clearObject(mmp::MpPacketElement* obj);
inline bool isEmptyObject(const mmp::MpPacketElement& obj);
inline void clearObject(Point* obj);
inline bool isEmptyObject(const Point& obj);
inline void clearObject(PointI* obj);
inline bool isEmptyObject(const PointI& obj);
inline void clearObject(Size* obj);
inline bool isEmptyObject(const Size& obj);
inline void clearObject(SizeI* obj);
inline bool isEmptyObject(const SizeI& obj);
inline void clearObject(Rect* obj);
inline bool isEmptyObject(const Rect& obj);
inline void clearObject(RectI* obj);
inline bool isEmptyObject(const RectI& obj);


inline void clearObject(SampleGamePacket* obj) {
    obj->type = {};
    obj->cat = {};
    obj->ord = {};
    Packet::clearProp(&(obj->messageId));
    Packet::clearProp(&(obj->messageTime));
    Packet::clearProp(&(obj->messageHash));
    obj->error = {};
    Packet::clearProp(&(obj->code));
    Packet::clearProp(&(obj->message));
    Packet::clearProp(&(obj->value));
    Packet::clearProp(&(obj->text));
    Packet::clearProp(&(obj->time));
    obj->matchStage = {};
    Packet::clearProp(&(obj->matchKey));
    Packet::clearProp(&(obj->matchId));
    obj->players.clear();
    clearObject(&(obj->element));
    obj->elements.clear();
}
inline bool isEmptyObject(const SampleGamePacket& obj) {
    if (int(obj.type) != int(0)) return false;
    if (int(obj.cat) != int(0)) return false;
    if (int(obj.ord) != int(0)) return false;
    if (!Packet::isPropEmpty(obj.messageId)) return false;
    if (!Packet::isPropEmpty(obj.messageTime)) return false;
    if (!Packet::isPropEmpty(obj.messageHash)) return false;
    if (int(obj.error) != int(0)) return false;
    if (!Packet::isPropEmpty(obj.code)) return false;
    if (!Packet::isPropEmpty(obj.message)) return false;
    if (!Packet::isPropEmpty(obj.value)) return false;
    if (!Packet::isPropEmpty(obj.text)) return false;
    if (!Packet::isPropEmpty(obj.time)) return false;
    if (int(obj.matchStage) != int(0)) return false;
    if (!Packet::isPropEmpty(obj.matchKey)) return false;
    if (!Packet::isPropEmpty(obj.matchId)) return false;
    if (!obj.players.empty()) return false;
    isEmptyObject(obj.element);
    if (!obj.elements.empty()) return false;
    return true;
}

inline void clearObject(mmp::MpPacketPlayer* obj) {
    Packet::clearProp(&(obj->key));
    Packet::clearProp(&(obj->id));
    Packet::clearProp(&(obj->alias));
    Packet::clearProp(&(obj->name));
}
inline bool isEmptyObject(const mmp::MpPacketPlayer& obj) {
    if (!Packet::isPropEmpty(obj.key)) return false;
    if (!Packet::isPropEmpty(obj.id)) return false;
    if (!Packet::isPropEmpty(obj.alias)) return false;
    if (!Packet::isPropEmpty(obj.name)) return false;
    return true;
}

inline void clearObject(mmp::MpPacketElement* obj) {
    Packet::clearProp(&(obj->key));
    Packet::clearProp(&(obj->player));
    Packet::clearProp(&(obj->type));
    Packet::clearProp(&(obj->state));
    Packet::clearProp(&(obj->color));
    clearObject(&(obj->rect));
}
inline bool isEmptyObject(const mmp::MpPacketElement& obj) {
    if (!Packet::isPropEmpty(obj.key)) return false;
    if (!Packet::isPropEmpty(obj.player)) return false;
    if (!Packet::isPropEmpty(obj.type)) return false;
    if (!Packet::isPropEmpty(obj.state)) return false;
    if (!Packet::isPropEmpty(obj.color)) return false;
    isEmptyObject(obj.rect);
    return true;
}

inline void clearObject(Point* obj) {
    Packet::clearProp(&(obj->x));
    Packet::clearProp(&(obj->y));
}
inline bool isEmptyObject(const Point& obj) {
    if (!Packet::isPropEmpty(obj.x)) return false;
    if (!Packet::isPropEmpty(obj.y)) return false;
    return true;
}

inline void clearObject(PointI* obj) {
    Packet::clearProp(&(obj->x));
    Packet::clearProp(&(obj->y));
}
inline bool isEmptyObject(const PointI& obj) {
    if (!Packet::isPropEmpty(obj.x)) return false;
    if (!Packet::isPropEmpty(obj.y)) return false;
    return true;
}

inline void clearObject(Size* obj) {
    Packet::clearProp(&(obj->w));
    Packet::clearProp(&(obj->h));
}
inline bool isEmptyObject(const Size& obj) {
    if (!Packet::isPropEmpty(obj.w)) return false;
    if (!Packet::isPropEmpty(obj.h)) return false;
    return true;
}

inline void clearObject(SizeI* obj) {
    Packet::clearProp(&(obj->w));
    Packet::clearProp(&(obj->h));
}
inline bool isEmptyObject(const SizeI& obj) {
    if (!Packet::isPropEmpty(obj.w)) return false;
    if (!Packet::isPropEmpty(obj.h)) return false;
    return true;
}

inline void clearObject(Rect* obj) {
    Packet::clearProp(&(obj->x));
    Packet::clearProp(&(obj->y));
    Packet::clearProp(&(obj->w));
    Packet::clearProp(&(obj->h));
}
inline bool isEmptyObject(const Rect& obj) {
    if (!Packet::isPropEmpty(obj.x)) return false;
    if (!Packet::isPropEmpty(obj.y)) return false;
    if (!Packet::isPropEmpty(obj.w)) return false;
    if (!Packet::isPropEmpty(obj.h)) return false;
    return true;
}

inline void clearObject(RectI* obj) {
    Packet::clearProp(&(obj->x));
    Packet::clearProp(&(obj->y));
    Packet::clearProp(&(obj->w));
    Packet::clearProp(&(obj->h));
}
inline bool isEmptyObject(const RectI& obj) {
    if (!Packet::isPropEmpty(obj.x)) return false;
    if (!Packet::isPropEmpty(obj.y)) return false;
    if (!Packet::isPropEmpty(obj.w)) return false;
    if (!Packet::isPropEmpty(obj.h)) return false;
    return true;
}


inline void packObject(const SampleGamePacket& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, SampleGamePacket* obj);
inline void packObject(const mmp::MpPacketPlayer& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, mmp::MpPacketPlayer* obj);
inline void packObject(const mmp::MpPacketElement& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, mmp::MpPacketElement* obj);
inline void packObject(const Point& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, Point* obj);
inline void packObject(const PointI& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, PointI* obj);
inline void packObject(const Size& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, Size* obj);
inline void packObject(const SizeI& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, SizeI* obj);
inline void packObject(const Rect& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, Rect* obj);
inline void packObject(const RectI& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, RectI* obj);

inline void packObject(const SampleGamePacket& obj, mangl::PacketPacker* packer) {
    if (static_cast<char>(obj.type) != 0)
        packer->pack(1, static_cast<char>(obj.type));
    if (static_cast<char>(obj.cat) != 0)
        packer->pack(2, static_cast<char>(obj.cat));
    if (static_cast<char>(obj.ord) != 0)
        packer->pack(3, static_cast<char>(obj.ord));
    if (!Packet::isPropEmpty(obj.messageId))
        packer->pack(4, obj.messageId);
    if (!Packet::isPropEmpty(obj.messageTime))
        packer->pack(5, obj.messageTime);
    if (!Packet::isPropEmpty(obj.messageHash))
        packer->pack(6, obj.messageHash);
    if (static_cast<char>(obj.error) != 0)
        packer->pack(7, static_cast<char>(obj.error));
    if (!Packet::isPropEmpty(obj.code))
        packer->pack(8, obj.code);
    if (!Packet::isPropEmpty(obj.message))
        packer->pack(9, obj.message);
    if (!Packet::isPropEmpty(obj.value))
        packer->pack(10, obj.value);
    if (!Packet::isPropEmpty(obj.text))
        packer->pack(11, obj.text);
    if (!Packet::isPropEmpty(obj.time))
        packer->pack(12, obj.time);
    if (static_cast<int>(obj.matchStage) != 0)
        packer->pack(13, static_cast<int>(obj.matchStage));
    if (!Packet::isPropEmpty(obj.matchKey))
        packer->pack(14, obj.matchKey);
    if (!Packet::isPropEmpty(obj.matchId))
        packer->pack(15, obj.matchId);
    if (!obj.players.empty()) {
        Index size = std::ssize(obj.players);
        packer->packArrayBegin<Index>(16, size);
        for (auto& val: obj.players) {
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
    if (!isEmptyObject(obj.element)) {
        packer->beginObject(17);
        packObject(obj.element, packer);
        packer->endObject(17);
    }

    if (!obj.elements.empty()) {
        Index size = std::ssize(obj.elements);
        packer->packArrayBegin<Index>(18, size);
        for (auto& val: obj.elements) {
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
}

inline void packObject(const mmp::MpPacketPlayer& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.key))
        packer->pack(1, obj.key);
    if (!Packet::isPropEmpty(obj.id))
        packer->pack(2, obj.id);
    if (!Packet::isPropEmpty(obj.alias))
        packer->pack(3, obj.alias);
    if (!Packet::isPropEmpty(obj.name))
        packer->pack(4, obj.name);
}

inline void packObject(const mmp::MpPacketElement& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.key))
        packer->pack(1, obj.key);
    if (!Packet::isPropEmpty(obj.player))
        packer->pack(2, obj.player);
    if (!Packet::isPropEmpty(obj.type))
        packer->pack(3, obj.type);
    if (!Packet::isPropEmpty(obj.state))
        packer->pack(4, obj.state);
    if (!Packet::isPropEmpty(obj.color))
        packer->pack(5, obj.color);
    if (!isEmptyObject(obj.rect)) {
        packer->beginObject(6);
        packObject(obj.rect, packer);
        packer->endObject(6);
    }

}

inline void packObject(const Point& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.x))
        packer->pack(1, obj.x);
    if (!Packet::isPropEmpty(obj.y))
        packer->pack(2, obj.y);
}

inline void packObject(const PointI& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.x))
        packer->pack(1, obj.x);
    if (!Packet::isPropEmpty(obj.y))
        packer->pack(2, obj.y);
}

inline void packObject(const Size& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.w))
        packer->pack(1, obj.w);
    if (!Packet::isPropEmpty(obj.h))
        packer->pack(2, obj.h);
}

inline void packObject(const SizeI& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.w))
        packer->pack(1, obj.w);
    if (!Packet::isPropEmpty(obj.h))
        packer->pack(2, obj.h);
}

inline void packObject(const Rect& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.x))
        packer->pack(1, obj.x);
    if (!Packet::isPropEmpty(obj.y))
        packer->pack(2, obj.y);
    if (!Packet::isPropEmpty(obj.w))
        packer->pack(3, obj.w);
    if (!Packet::isPropEmpty(obj.h))
        packer->pack(4, obj.h);
}

inline void packObject(const RectI& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.x))
        packer->pack(1, obj.x);
    if (!Packet::isPropEmpty(obj.y))
        packer->pack(2, obj.y);
    if (!Packet::isPropEmpty(obj.w))
        packer->pack(3, obj.w);
    if (!Packet::isPropEmpty(obj.h))
        packer->pack(4, obj.h);
}


inline void unpackObject(mangl::PacketLoader& loader, SampleGamePacket* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            obj->type = static_cast<mmp::MessageType>(loader.unpackValue<char>());
            break;
        case 2:
            obj->cat = static_cast<mmp::MessageCategory>(loader.unpackValue<char>());
            break;
        case 3:
            obj->ord = static_cast<mmp::MessageOrder>(loader.unpackValue<char>());
            break;
        case 4:
            loader.unpack(&obj->messageId);
            break;
        case 5:
            loader.unpack(&obj->messageTime);
            break;
        case 6:
            loader.unpack(&obj->messageHash);
            break;
        case 7:
            obj->error = static_cast<mmp::MessageErrorCat>(loader.unpackValue<char>());
            break;
        case 8:
            loader.unpack(&obj->code);
            break;
        case 9:
            loader.unpack(&obj->message);
            break;
        case 10:
            loader.unpack(&obj->value);
            break;
        case 11:
            loader.unpack(&obj->text);
            break;
        case 12:
            loader.unpack(&obj->time);
            break;
        case 13:
            obj->matchStage = static_cast<mmp::MatchStage>(loader.unpackValue<int>());
            break;
        case 14:
            loader.unpack(&obj->matchKey);
            break;
        case 15:
            loader.unpack(&obj->matchId);
            break;
        case 16:
            {
                auto size = loader.arraySize();
                obj->players.resize(size);
                for (auto& val: obj->players) {
                    unpackObject(loader, &val);
                }
            }
            break;
        case 17:
            loader.unpackObject();
            if (PacketFieldType::ObjectBegin == field.type) {
                unpackObject(loader, &obj->element);
            }
            break;
        case 18:
            {
                auto size = loader.arraySize();
                obj->elements.resize(size);
                for (auto& val: obj->elements) {
                    unpackObject(loader, &val);
                }
            }
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, mmp::MpPacketPlayer* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->key);
            break;
        case 2:
            loader.unpack(&obj->id);
            break;
        case 3:
            loader.unpack(&obj->alias);
            break;
        case 4:
            loader.unpack(&obj->name);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, mmp::MpPacketElement* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->key);
            break;
        case 2:
            loader.unpack(&obj->player);
            break;
        case 3:
            loader.unpack(&obj->type);
            break;
        case 4:
            loader.unpack(&obj->state);
            break;
        case 5:
            loader.unpack(&obj->color);
            break;
        case 6:
            loader.unpackObject();
            if (PacketFieldType::ObjectBegin == field.type) {
                unpackObject(loader, &obj->rect);
            }
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, Point* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->x);
            break;
        case 2:
            loader.unpack(&obj->y);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, PointI* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->x);
            break;
        case 2:
            loader.unpack(&obj->y);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, Size* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->w);
            break;
        case 2:
            loader.unpack(&obj->h);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, SizeI* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->w);
            break;
        case 2:
            loader.unpack(&obj->h);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, Rect* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->x);
            break;
        case 2:
            loader.unpack(&obj->y);
            break;
        case 3:
            loader.unpack(&obj->w);
            break;
        case 4:
            loader.unpack(&obj->h);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, RectI* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&obj->x);
            break;
        case 2:
            loader.unpack(&obj->y);
            break;
        case 3:
            loader.unpack(&obj->w);
            break;
        case 4:
            loader.unpack(&obj->h);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}


}  // namespace impl_SampleGamePacket_packet


inline void unpack(mangl::PacketLoader& loader, SampleGamePacket* obj) {
    impl_SampleGamePacket_packet::clearObject(obj);
    impl_SampleGamePacket_packet::unpackObject(loader, obj);
}
inline void pack(const SampleGamePacket& obj, mangl::PacketPacker* packet) {
    packet->begin();
    impl_SampleGamePacket_packet::packObject(obj, packet);
    packet->end();
}
inline void clear(SampleGamePacket* obj) {
    impl_SampleGamePacket_packet::clearObject(obj);
}


}  // namespace sample::mp

