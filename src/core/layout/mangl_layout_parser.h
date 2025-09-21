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
#include "../mangl_texture_atlas.h"

#include <apeal/parse/parse_value.hpp>
#include <apeal/parse.hpp>
#include <apeal/parse/preprocessor.hpp>
#include <apeal/parse/ini.hpp>
#include <apeal/parse/split.hpp>


#include <map>
#include <vector>
#include <functional>

namespace mangl {


struct MLayoutCoordinateValue {
    enum class Type: int { VALUE, PERCENT, TOP, LEFT, BOTTOM, RIGHT, CENTER, REF, END_ } type{};
    Real value{};

    enum class RefType: int { TEXTURE, END_ } refType{};

    enum class RefComponent {
        X, Y, WIDTH, HEIGHT, RIGHT, TOP
    };
    RefComponent refComponent{};

    String refName;
};

struct MLayoutCoordinateToken {
    enum class Type { VALUE, OPERATOR } type{Type::VALUE};

    MLayoutCoordinateValue value;

    enum class Operator {
        NONE = 0,
        PLUS = '+',
        MINUS = '-',
        MUL = '*',
        DIV = '/',
        PARENTHESIS_OPEN = '(',
        PARENTHESIS_CLOSE = ')',
        COMMA = ',',
        COLON = ':',
        AT = '@',
        UNARY_PLUS = 10000,
        UNARY_MINUS,
    };

    Operator op{};


    MLayoutCoordinateToken() noexcept {}
    MLayoutCoordinateToken(Operator o) noexcept: type(Type::OPERATOR), op(o) {}

    //
    // Runtime variables
    //
    bool rtValid{};
    Real rtValue{};
};

struct MLayoutCoordinate {

    typedef std::vector<MLayoutCoordinateToken> Tokens;

    Tokens tokens;

    bool null() const
    {
        if (tokens.empty())
            return true;
        const auto& v = tokens[0].value;

        return MLayoutCoordinateValue::Type::VALUE == v.type && 0 == v.value;
    }

    void operator = (Cord val)
    {
        tokens.clear();
        tokens.resize(1);
        auto& v = tokens.back().value;
        v.type = MLayoutCoordinateValue::Type::VALUE;
        v.value = val;
    }
};


struct MLayoutPoint {
    MLayoutCoordinate x, y;

    MLayoutPoint() {}
    MLayoutPoint(Cord ix, Cord iy) { x = ix, y = iy; }
    MLayoutPoint(const MLayoutCoordinate& _x, const MLayoutCoordinate& _y) { x = _x, y = _y; }
};

struct MLayoutSize {
    MLayoutCoordinate w, h;
};

struct MLayoutRect {
    MLayoutCoordinate x, y, w, h;
};

struct MLayoutAlign {
    int horz, vert;
};


class MLayoutParser {
    using Char = typename String::value_type;

public:
    StrValCont<String> variables;

    template <class ParametersList>
    void setParameters(const ParametersList& ps) {
        for (auto& p: ps)
            variables[p.first] = p.second;
    }

    void setParameter(StrView name, const apl::AVariant& value) {
        // TODO: to avoid misaligned pointer. Need to do better
        String names{name};
        variables[names] = value.stringValue();
    }

    void removeParameter(StrView name) {
        variables.erase(name);
    }


public:
    String textureDir;
    String atlasFile;

    MLayoutParser();
    virtual ~MLayoutParser(){}

    void setAtlas(const TextureAtlas* atlas) { atlas_ = atlas; }

    void setViewport(RectA viewport, PaddingA pad = {}) {
        viewport_ = viewport;
        padding_ = pad;
    }

    RectA viewport() const { return viewport_; }
    void setScale(Real scale) { scale_ = scale; }

    Rect layout{};
    bool layoutFlag{};
    int versionHi{1}, versionLo{}, versionPatch{}, versionDecimal{};

protected:
    Rect viewport_{}; //, paddedViewport_ ;
    Padding padding_{};
    TextureAtlas const* atlas_{};
    Real scale_{1};
    Real scaleTextures_{1};

    MLayoutPoint defaultOrigin_{};
    MLayoutAlign defaultAlign_{M::AlignLeft, M::AlignBottom};

    Point defaultAxis_{};


    void clear()
    {
        scale_ = 1;
        scaleTextures_ = 1;
        defaultOrigin_ = {};
        defaultAxis_ = {1, 1};
        versionHi = 1, versionLo = 0, versionPatch = 0;
        versionDecimal = versionHi * 10000 + versionLo * 100 + versionPatch;
        layoutFlag = false;
    }

    struct ExpressionValue {
        enum class DataType { STRING, FLOAT, } type;

        String stringValue;
        Real floatValue;

        char separator = ',';
    };

    struct Expression: std::vector<ExpressionValue> {};

    Expression evaluateExpressionValue(MLayoutCoordinate::Tokens& tokens);

    Cord evaluateCoordinateValue(const MLayoutCoordinateValue& c, Cord total) const;
    Cord evaluateCoordinate(MLayoutCoordinate& c, Cord total);

    Cord xVal(MLayoutCoordinate& c) { return evaluateCoordinate(c, layout.w) * scale_; }
    Cord yVal(MLayoutCoordinate& c) { return evaluateCoordinate(c, layout.h) * scale_; }
    Point pointVal(MLayoutPoint& p) { return {xVal(p.x), yVal(p.y)}; }
    Size sizeVal(MLayoutSize& s) { return {xVal(s.w), yVal(s.h)}; }


    void setupParameters();


protected:
    // parsing
    String filename_, sec_name_, value_, name_;
    Index line_;
    apl::Preprocessor::Deps deps_;

    KeyValCont<StrView, uint32_t> colorMap_;


    struct Token {
        typedef MLayoutCoordinateToken::Operator Operator;

        enum class Type { VALUE, OPERATOR };
        Type type;

        String value;
        int priority;

        Operator op;

        void clear() {
            value.clear();
        }

        void setOperator(Operator _op) {
            type = Type::OPERATOR;
            op = _op;
        }
    };

    //Cord parseFloat ( const MString &s ) {  return apl::parseFP<Cord>(s) ;  }
    //int parseInt ( const MString &s ) {  return apl::parseInt(s) ;  }

    bool parseReadToken(StrView::const_iterator& current, StrView::const_iterator end, Token& token);
    bool parseIsSpace(StrView::const_iterator current) const { return ' ' == *current || '\t' == *current || '\r' == *current || '\n' == *current; }
    bool parseIsAlpha(StrView::const_iterator current) const { return (*current>='A'&&*current<='Z') || (*current>='a' && *current<='z') || *current == '_'; }
    bool parseIsDigit(StrView::const_iterator current) const { return *current>='0'&&*current<='9'; }
    bool parseIsAlnum(StrView::const_iterator current) const { return parseIsAlpha(current) || parseIsDigit(current); }

    bool parseIsAlpha(Char c) const { return (c>='A'&&c<='Z') || (c>='a' && c<='z') || c == '_'; }

    MLayoutCoordinate::Tokens parseExpression(StrView value);

    MLayoutCoordinate parseCoordinate(StrView s)
    {
        if (s.empty())
            return {};

        MLayoutCoordinate c;
        c.tokens = parseExpression(s);
        return c;
    }

    MLayoutCoordinateValue parseCoordinateValue(StrView s);

    MLayoutPoint parseLayoutPoint(StrView sx, StrView sy) {
        return {parseCoordinate(sx), parseCoordinate(sy)};
    }

    MLayoutPoint parseLayoutPoint(StrView s) {
        auto [sx, sy] = apl::str_split_trim(s, ',');
        return parseLayoutPoint(sx, sy);
    }

    MLayoutSize parseLayoutSize(StrView value) {
        auto [sx, sy] = apl::str_split_trim(value, ',');
        return {parseCoordinate(sx), parseCoordinate(sy)};
    }

    void parseLayoutRectPair(StrView s, MLayoutPoint& point, MLayoutSize& size);
    MLayoutRect parseLayoutRect(StrView s);
    Color parseColor(StrView s);

    //bool parseAlign ( const MString &value, int &align_horz, int &align_vert ) ;
    int parseHorzAlign(StrView s);
    int parseVertAlign(StrView s);
    MLayoutAlign parseAlign(StrView s) {
        auto [hs, vs] = apl::str_split_trim(s, ',');
        return {parseHorzAlign(hs), parseVertAlign(vs)};
    }

    bool parseVar(StrView name, StrView value);


    enum class ComparisonOperator { EQUAL, NOT_EQUAL, GREATER, GREATER_OR_EQUAL, LESS, LESS_OR_EQUAL };

    void processElement(apl::AIni::Element& el);
    void processExpression(StrView input, String& output);
    void expandVariables(StrView input, String& output);
    bool processCondition(StrView name, String& output);
    bool evaluateCondition(StrView conditionValue, StrView variableValue, const ComparisonOperator& op);
    bool processVersion(StrView value);


protected:
    virtual Cord onXVal(const MLayoutCoordinateValue& u) const { return 0; }
    virtual Cord onYVal(const MLayoutCoordinateValue& u) const { return 0; }
    virtual bool onRefParse(StrView tag, MLayoutCoordinateValue& c) const { return false; }


    [[noreturn]]
    void raiseError(StrView msg);

    template <class ...Args>
    [[noreturn]]
    void raiseError(StrView fmt, Args&&... args) {
        raiseError(apl::fmt(fmt, std::forward<Args>(args)...));
    }

};

}  // namespace mangl

