/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"

#include <apeal/parse/split.hpp>
#include <apeal/parse/vector.hpp>

#include <concepts>

namespace mangl::net {

enum class DataTransferFormat: int {
    Unknown = 0,
    Packet = 1,
    Json = 2,
    Yaml = 3,
};

struct DiscoveryInfo {
    using Params = KeyValCont<String, String>;

    String alias;  // Player name / alias
    String proto;  // protocol name
    String protv;  // protocol version
    String appn;  // app name
    String appv;  // app version
    String uuid;  // App protocol uuid
    Params params;


    char vsep = '.';  // version separator
    char ksep = ':';  // key separator
    char rsep = ';';  // record separator

    void setAppVer(StrView text) noexcept {
        appv = text;
    }

    void setProtoVer(int maj, int min = 0, int fix = 0) noexcept {
        protv = std::to_string(maj);
        if (min) {
            protv += vsep;
            protv += std::to_string(min);
        }

        if (fix) {
            protv += vsep;
            protv += std::to_string(fix);
        }
    }

    template <class I>
    bool parseIt(I begin, I end) noexcept {
        *this = {};

        using namespace apl;

        auto&& v = parse_vector_it<std::vector<StrView>>(begin, end, rsep);

        using namespace apl::literals;

        for (auto&& i: v) {
            auto [key, value] = str_split_trim(i, ksep);
            if (key.empty())
                continue;
            switch (pshHash(key)) {
            case "alin"_psh:  alias = value; break;
            case "prtn"_psh:  proto = value; break;
            case "prtv"_psh:  protv = value; break;
            case "appn"_psh:  appn = value;  break;
            case "appv"_psh:  appv = value;  break;
            case "uuid"_psh:  uuid = value;  break;
            default:
                params[String{key}] = value;
                break;
            }
        }

        // Check if we everything is ok
        if (alias.empty() || proto.empty())
            return false;

        return true;
    }

    bool parse(StrView str) noexcept {
        return parseIt(str.begin(), str.end());
    }

    auto parseData(DataParamA data) noexcept {
        return parse(StrView{reinterpret_cast<const char*>(data.data()), data.size()});
    }

    [[nodiscard]] String build() const noexcept {
        String s;

        auto param = [this, &s](std::string_view name, const auto& value) {
            if (apl::isFalse(value))
                return;
            apl::str_format_append(s, "%s%c%z;", name, ksep, value);
        };

        param("alin", alias);
        param("prtn", proto);
        param("prtv", protv);
        param("appn", appn);
        param("appv", appv);
        param("uuid", uuid);

        for (auto&& p: params)
            param(p.first, p.second);

        return s;
    }

    // Accessing params
    [[nodiscard]] StrView getParam(StrView name) const noexcept {
        auto it = params.find(String{name});
        if (it == params.end())
            return {};
        return it->second;
    }

    void setParam(StrView name, StrView value) noexcept {
        params[String{name}] = value;
    }

    void setParam(StrView name, std::integral auto value) noexcept {
        params[String{name}] = std::to_string(value);
    }


    // Shortcuts to common used functions
    void setScheme(StrView value) noexcept { setParam("schm", value); }
    void setHost(StrView value) noexcept { setParam("host", value); }
    void setPort(StrView value) noexcept { setParam("port", value); }
    void setPort(std::integral auto value) noexcept { setParam("port", value); }

    [[nodiscard]] StrView getScheme() const noexcept { return getParam("schm"); }
    [[nodiscard]] StrView getHost() const noexcept { return getParam("host"); }
    [[nodiscard]] StrView getPort() const noexcept { return getParam("port"); }
    [[nodiscard]] auto getUrl() const noexcept {
        return apl::fmt("%s://%s:%s", getScheme(), getHost(), getPort());
    }


    // Data transfer formats
    static StrView makeFormat(DataTransferFormat fmt, StrView def = {}) noexcept {
        switch (fmt) {
        using enum DataTransferFormat;
        case Packet: return "packet";
        case Json: return "json";
        case Yaml: return "yaml";
        default: break;
        }
        return def;
    }

    void setFormat(DataTransferFormat fmt) { setParam("dfmt", makeFormat(fmt)); }

    static DataTransferFormat parseFormat(StrView val, DataTransferFormat def = DataTransferFormat::Unknown) noexcept {
        using enum DataTransferFormat;
        if ("packet" == val) return Packet;
        if ("json" == val) return Json;
        if ("yaml" == val) return Yaml;
        return def;
    }

    [[nodiscard]] DataTransferFormat getFormat(DataTransferFormat def = DataTransferFormat::Unknown) const noexcept {
        auto val = getParam("dfmt");
        return parseFormat(val, def);
    }
};

using DiscoveryInfoA = const DiscoveryInfo&;


}  // namespace mangl::net
