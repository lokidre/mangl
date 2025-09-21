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
#include <apeal/parse/json.hpp>
#include <string>
#include <string_view>

namespace sample::mp {

inline void pack(const SampleGamePacket& obj, apl::JsonBuilder* builder);
inline void unpack(const apl::Json& packet, SampleGamePacket* obj);

inline auto packStringJson(const SampleGamePacket& obj) {
    apl::JsonBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringJson(std::string_view str, SampleGamePacket* obj) {
    apl::Json packet;
    apl::JsonParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto unpackDataJson(DataParamA data, SampleGamePacket* obj) {
    unpackStringJson({(const char *)data.data(), data.size()}, obj);
}



namespace impl_SampleGamePacket_json {

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


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyJson(T name) { return false; }

inline bool isEmptyJson(std::string_view name) { return name.empty(); }

inline void packObject(const SampleGamePacket& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, SampleGamePacket* obj);

inline void packObject(const mmp::MpPacketPlayer& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, mmp::MpPacketPlayer* obj);

inline void packObject(const mmp::MpPacketElement& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, mmp::MpPacketElement* obj);

inline void packObject(const Point& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, Point* obj);

inline void packObject(const PointI& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, PointI* obj);

inline void packObject(const Size& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, Size* obj);

inline void packObject(const SizeI& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, SizeI* obj);

inline void packObject(const Rect& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, Rect* obj);

inline void packObject(const RectI& obj, std::string_view name, apl::JsonBuilder* builder);
inline void unpackObject(const apl::JsonNode& node, RectI* obj);


inline void packObject(const SampleGamePacket& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (static_cast<char>(obj.type) != 0)
        builder->field("type", static_cast<char>(obj.type));
    if (static_cast<char>(obj.cat) != 0)
        builder->field("cat", static_cast<char>(obj.cat));
    if (static_cast<char>(obj.ord) != 0)
        builder->field("ord", static_cast<char>(obj.ord));
    if (!Packet::isPropEmpty(obj.messageId))
        builder->field("messageId", obj.messageId);
    if (!Packet::isPropEmpty(obj.messageTime))
        builder->field("messageTime", obj.messageTime);
    if (!Packet::isPropEmpty(obj.messageHash))
        builder->field("messageHash", obj.messageHash);
    if (static_cast<char>(obj.error) != 0)
        builder->field("error", static_cast<char>(obj.error));
    if (!Packet::isPropEmpty(obj.code))
        builder->field("code", obj.code);
    if (!Packet::isPropEmpty(obj.message))
        builder->field("message", obj.message);
    if (!Packet::isPropEmpty(obj.value))
        builder->field("value", obj.value);
    if (!Packet::isPropEmpty(obj.text))
        builder->field("text", obj.text);
    if (!Packet::isPropEmpty(obj.time))
        builder->field("time", obj.time);
    if (static_cast<int>(obj.matchStage) != 0)
        builder->field("matchStage", static_cast<int>(obj.matchStage));
    if (!Packet::isPropEmpty(obj.matchKey))
        builder->field("matchKey", obj.matchKey);
    if (!Packet::isPropEmpty(obj.matchId))
        builder->field("matchId", obj.matchId);
    if (!obj.players.empty()) {
        builder->array("players");
        for (auto& val: obj.players) {
            packObject(val, {}, builder);
        }
        builder->pop();  // players

    }

    if (!isEmptyObject(obj.element))
        packObject(obj.element, "element", builder);
    if (!obj.elements.empty()) {
        builder->array("elements");
        for (auto& val: obj.elements) {
            packObject(val, {}, builder);
        }
        builder->pop();  // elements

    }

    builder->pop();  // SampleGamePacket

}

inline void unpackObject(const apl::JsonNode& node, SampleGamePacket* obj) {

    for (auto& objNode: node) {
        if ("type" == objNode.key) {
            char val;
            objNode.get(&val);
            obj->type = static_cast<mmp::MessageType>(val);
            continue;
        }

        if ("cat" == objNode.key) {
            char val;
            objNode.get(&val);
            obj->cat = static_cast<mmp::MessageCategory>(val);
            continue;
        }

        if ("ord" == objNode.key) {
            char val;
            objNode.get(&val);
            obj->ord = static_cast<mmp::MessageOrder>(val);
            continue;
        }

        if ("messageId" == objNode.key) {
            objNode.get(&obj->messageId);
            continue;
        }

        if ("messageTime" == objNode.key) {
            objNode.get(&obj->messageTime);
            continue;
        }

        if ("messageHash" == objNode.key) {
            objNode.get(&obj->messageHash);
            continue;
        }

        if ("error" == objNode.key) {
            char val;
            objNode.get(&val);
            obj->error = static_cast<mmp::MessageErrorCat>(val);
            continue;
        }

        if ("code" == objNode.key) {
            objNode.get(&obj->code);
            continue;
        }

        if ("message" == objNode.key) {
            objNode.get(&obj->message);
            continue;
        }

        if ("value" == objNode.key) {
            objNode.get(&obj->value);
            continue;
        }

        if ("text" == objNode.key) {
            objNode.get(&obj->text);
            continue;
        }

        if ("time" == objNode.key) {
            objNode.get(&obj->time);
            continue;
        }

        if ("matchStage" == objNode.key) {
            int val;
            objNode.get(&val);
            obj->matchStage = static_cast<mmp::MatchStage>(val);
            continue;
        }

        if ("matchKey" == objNode.key) {
            objNode.get(&obj->matchKey);
            continue;
        }

        if ("matchId" == objNode.key) {
            objNode.get(&obj->matchId);
            continue;
        }

        if ("players" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->players.resize(size);
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->players[i]);
            }
            continue;
        }

        if ("element" == objNode.key) {
            unpackObject(objNode, &obj->element);
            continue;
        }

        if ("elements" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->elements.resize(size);
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->elements[i]);
            }
            continue;
        }

    }  // for nodes

}

inline void packObject(const mmp::MpPacketPlayer& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.key))
        builder->field("key", obj.key);
    if (!Packet::isPropEmpty(obj.id))
        builder->field("id", obj.id);
    if (!Packet::isPropEmpty(obj.alias))
        builder->field("alias", obj.alias);
    if (!Packet::isPropEmpty(obj.name))
        builder->field("name", obj.name);
    builder->pop();  // mmp::MpPacketPlayer

}

inline void unpackObject(const apl::JsonNode& node, mmp::MpPacketPlayer* obj) {

    for (auto& objNode: node) {
        if ("key" == objNode.key) {
            objNode.get(&obj->key);
            continue;
        }

        if ("id" == objNode.key) {
            objNode.get(&obj->id);
            continue;
        }

        if ("alias" == objNode.key) {
            objNode.get(&obj->alias);
            continue;
        }

        if ("name" == objNode.key) {
            objNode.get(&obj->name);
            continue;
        }

    }  // for nodes

}

inline void packObject(const mmp::MpPacketElement& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.key))
        builder->field("key", obj.key);
    if (!Packet::isPropEmpty(obj.player))
        builder->field("player", obj.player);
    if (!Packet::isPropEmpty(obj.type))
        builder->field("type", obj.type);
    if (!Packet::isPropEmpty(obj.state))
        builder->field("state", obj.state);
    if (!Packet::isPropEmpty(obj.color))
        builder->field("color", obj.color);
    if (!isEmptyObject(obj.rect))
        packObject(obj.rect, "rect", builder);
    builder->pop();  // mmp::MpPacketElement

}

inline void unpackObject(const apl::JsonNode& node, mmp::MpPacketElement* obj) {

    for (auto& objNode: node) {
        if ("key" == objNode.key) {
            objNode.get(&obj->key);
            continue;
        }

        if ("player" == objNode.key) {
            objNode.get(&obj->player);
            continue;
        }

        if ("type" == objNode.key) {
            objNode.get(&obj->type);
            continue;
        }

        if ("state" == objNode.key) {
            objNode.get(&obj->state);
            continue;
        }

        if ("color" == objNode.key) {
            objNode.get(&obj->color);
            continue;
        }

        if ("rect" == objNode.key) {
            unpackObject(objNode, &obj->rect);
            continue;
        }

    }  // for nodes

}

inline void packObject(const Point& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.x))
        builder->field("x", obj.x);
    if (!Packet::isPropEmpty(obj.y))
        builder->field("y", obj.y);
    builder->pop();  // Point

}

inline void unpackObject(const apl::JsonNode& node, Point* obj) {

    for (auto& objNode: node) {
        if ("x" == objNode.key) {
            objNode.get(&obj->x);
            continue;
        }

        if ("y" == objNode.key) {
            objNode.get(&obj->y);
            continue;
        }

    }  // for nodes

}

inline void packObject(const PointI& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.x))
        builder->field("x", obj.x);
    if (!Packet::isPropEmpty(obj.y))
        builder->field("y", obj.y);
    builder->pop();  // PointI

}

inline void unpackObject(const apl::JsonNode& node, PointI* obj) {

    for (auto& objNode: node) {
        if ("x" == objNode.key) {
            objNode.get(&obj->x);
            continue;
        }

        if ("y" == objNode.key) {
            objNode.get(&obj->y);
            continue;
        }

    }  // for nodes

}

inline void packObject(const Size& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.w))
        builder->field("w", obj.w);
    if (!Packet::isPropEmpty(obj.h))
        builder->field("h", obj.h);
    builder->pop();  // Size

}

inline void unpackObject(const apl::JsonNode& node, Size* obj) {

    for (auto& objNode: node) {
        if ("w" == objNode.key) {
            objNode.get(&obj->w);
            continue;
        }

        if ("h" == objNode.key) {
            objNode.get(&obj->h);
            continue;
        }

    }  // for nodes

}

inline void packObject(const SizeI& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.w))
        builder->field("w", obj.w);
    if (!Packet::isPropEmpty(obj.h))
        builder->field("h", obj.h);
    builder->pop();  // SizeI

}

inline void unpackObject(const apl::JsonNode& node, SizeI* obj) {

    for (auto& objNode: node) {
        if ("w" == objNode.key) {
            objNode.get(&obj->w);
            continue;
        }

        if ("h" == objNode.key) {
            objNode.get(&obj->h);
            continue;
        }

    }  // for nodes

}

inline void packObject(const Rect& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.x))
        builder->field("x", obj.x);
    if (!Packet::isPropEmpty(obj.y))
        builder->field("y", obj.y);
    if (!Packet::isPropEmpty(obj.w))
        builder->field("w", obj.w);
    if (!Packet::isPropEmpty(obj.h))
        builder->field("h", obj.h);
    builder->pop();  // Rect

}

inline void unpackObject(const apl::JsonNode& node, Rect* obj) {

    for (auto& objNode: node) {
        if ("x" == objNode.key) {
            objNode.get(&obj->x);
            continue;
        }

        if ("y" == objNode.key) {
            objNode.get(&obj->y);
            continue;
        }

        if ("w" == objNode.key) {
            objNode.get(&obj->w);
            continue;
        }

        if ("h" == objNode.key) {
            objNode.get(&obj->h);
            continue;
        }

    }  // for nodes

}

inline void packObject(const RectI& obj, std::string_view name, apl::JsonBuilder* builder) {

    if (isEmptyJson(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.x))
        builder->field("x", obj.x);
    if (!Packet::isPropEmpty(obj.y))
        builder->field("y", obj.y);
    if (!Packet::isPropEmpty(obj.w))
        builder->field("w", obj.w);
    if (!Packet::isPropEmpty(obj.h))
        builder->field("h", obj.h);
    builder->pop();  // RectI

}

inline void unpackObject(const apl::JsonNode& node, RectI* obj) {

    for (auto& objNode: node) {
        if ("x" == objNode.key) {
            objNode.get(&obj->x);
            continue;
        }

        if ("y" == objNode.key) {
            objNode.get(&obj->y);
            continue;
        }

        if ("w" == objNode.key) {
            objNode.get(&obj->w);
            continue;
        }

        if ("h" == objNode.key) {
            objNode.get(&obj->h);
            continue;
        }

    }  // for nodes

}

}  // namespace impl_SampleGamePacket_json


inline void pack(const SampleGamePacket& obj, apl::JsonBuilder* builder) {
    builder->begin();
    impl_SampleGamePacket_json::packObject(obj, "SampleGamePacket", builder);
    builder->end();
}

inline void unpack(const apl::Json& root, SampleGamePacket* obj) {
    impl_SampleGamePacket_json::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_SampleGamePacket_json::unpackObject(docNode, obj);
    }
}

}  // namespace sample::mp

