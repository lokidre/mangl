/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_layout_parser.h"

#include "../mangl_environment.h"

#include <apeal/parse.hpp>

namespace mangl {

MLayoutParser::MLayoutParser()
{
    colorMap_ = decltype(colorMap_) {
        { "black", 0x000000 }, {"navy",0x000080}, {"blue",0x0000FF}, {"green",0x008000}, {"teal",0x008080}, {"lime",0x00FF00}, {"aqua",0x00FFFF},
        {"maroon",0x800000}, {"olive",0x800080}, {"gray",0x808080}, {"gray",0xC0C0C0},
        {"red",0xFF0000}, {"orange",0xFFA500}, {"fuchsia",0xFF00FF}, {"yellow",0xFFFF00}, {"white", 0xFFFFFF}
    };

}


void MLayoutParser::setupParameters()
{
    setParameter("VIEWPORTX", viewport_.x);
    setParameter("VIEWPORTY", viewport_.y);
    setParameter("WIDTH", viewport_.w);
    setParameter("HEIGHT", viewport_.h);

    setParameter("TOP", padding_.top());
    setParameter("RIGHT", padding_.right());
    setParameter("BOTTOM", padding_.bottom());
    setParameter("LEFT", padding_.left());

    setParameter("SCALE", scale_);

    auto env = Env::instance();
    String device;
    switch (env->interfaceIdiom) {
    case InterfaceIdiom::Phone:  device = "PHONE"; break;
    case InterfaceIdiom::Pad:  device = "PAD";  break;
    case InterfaceIdiom::Desktop:  device = "DESKTOP"; break;
    default:
        MDebugTrap();
        break;
    }

    setParameter(device, 1);
    //setParameter("UPSCALE", env->upscale);
}



void MLayoutParser::processElement(apl::AIni::Element &el)
{
    // condition if the element was parsed incorrectly
    size_t separatorPos = el.value.find_first_of('=');
    if (el.name.find_first_of('(') == el.name.npos || (el.name.find_first_of('(') != el.name.npos && el.name.find_first_of(')') != el.name.npos) || separatorPos == el.value.npos)
        return;

    el.name += '=';
    el.name.append(el.value.substr(0, separatorPos));
    el.value.erase(0, separatorPos + 1);
    el.value = apl::str_trim(el.value);
}


MLayoutCoordinateValue MLayoutParser::parseCoordinateValue(StrView s)
{
    using namespace apl;

    using T = MLayoutCoordinateValue::Type;
    using RT = MLayoutCoordinateValue::RefType;
    using RC = MLayoutCoordinateValue::RefComponent;

    MLayoutCoordinateValue c;

    // check if this is a texture reference
    if (std::isalpha(s.front())) {
        if (auto dot_pos = s.find_first_of('.'); dot_pos != s.npos) {
            auto&& refStack = parse_std_vector_view_trim(s, '.');

            if ("texture" == refStack[0]) c.type = T::REF, c.refType = RT::TEXTURE; 
            else if (!onRefParse(refStack[0], c))
                raiseError("Invalid reference type: '%s'", refStack[0]);

            c.refName = refStack[1];
            auto refType = str_trim(refStack[2]);
            if ("w" == refType || "width" == refType) c.refComponent = RC::WIDTH;
            else if ("h" == refType || "height" == refType) c.refComponent = RC::HEIGHT;
            else if ("x" == refType || "left" == refType) c.refComponent = RC::X;
            else if ("y" == refType || "bottom" == refType) c.refComponent = RC::Y;
            else if ("right" == refType) c.refComponent = RC::RIGHT;
            else if ("top" == refType) c.refComponent = RC::TOP;
            else
                raiseError("Invalid texture reference '%s'", refType);

            return c;
        }
    }

    if ("right" == s) { c.type = T::RIGHT;	return c; }
    if ("bottom" == s) { c.type = T::BOTTOM;  return c; }

    if ("left" == s) { c.type = T::LEFT;  return c; }
    if ("top" == s) { c.type = T::TOP; return c; }

    if ("center" == s) { c.type = T::CENTER;	return c; }

    if (s.back() == '%') {
        c.type = T::PERCENT;
        c.value = M::parseCoord(s.substr(0, s.size()-1));
        return c;
    }

    c.type = T::VALUE;
    c.value = M::parseCoord(s);

    return c;
}


bool MLayoutParser::parseReadToken(StrView::const_iterator &current, StrView::const_iterator end, Token &token)
{
    using OP = Token::Operator;

    token.clear();

    while (current != end && parseIsSpace(current))
        ++current;

    if (current == end)
        return false;

    if (parseIsAlpha(current)) {
        token.type = Token::Type::VALUE;
        int dotCount = 0;
        while (current != end && (parseIsAlnum(current) || *current == '.' || *current == '-')) {
            if (*current == '.')
                dotCount++;
            if (*current == '-' && dotCount >= 2)
                break;
            token.value += *current++;
        }
        return true;
    }

    if (parseIsDigit(current)) {
        token.type = Token::Type::VALUE;

        while (current != end && (parseIsDigit(current) || *current == '.' || *current == '%')) {
            token.value += *current;
            if (*current == '%') {
                ++current;
                break;
            }
            ++current;
        }

        return true;
    }


    switch (*current) {
    case '$':
        token.value += *current++;
        token.type = Token::Type::VALUE;
        while (current != end && parseIsAlnum(current)) {
            token.value += *current++;
        }
        if (token.value.empty())
            return false;
        return true;

    case '(':
    case ')':
    case ',':
    case ':':
    case '@':
        token.priority = 0;
        token.setOperator((OP)*current); ++current;
        return true;

    case '*': token.setOperator(OP::MUL); token.priority = 3; ++current; return true;
    case '/': token.setOperator(OP::DIV); token.priority = 4; ++current; return true;

    case '+': token.setOperator(OP::PLUS); token.priority = 2; ++current; return true;
    case '-': token.setOperator(OP::MINUS); token.priority = 2; ++current; return true;
    default:
        raiseError("Invalid Character: %c", *current);
    }
}


MLayoutCoordinate::Tokens MLayoutParser::parseExpression(StrView value)
{
    using OP = Token::Operator;

    Token token;
    auto current = value.begin(), end = value.end();

    MLayoutCoordinate::Tokens tokens;
    //tokens.clear() ;

    std::stack<Token> operatorStack;

    Token lastToken;
    lastToken.type = Token::Type::OPERATOR;

    while (parseReadToken(current, end, token)) {
        if (Token::Type::VALUE == token.type) {
            MLayoutCoordinateToken ct;
            ct.type = MLayoutCoordinateToken::Type::VALUE;
            ct.value = parseCoordinateValue(token.value);
            tokens.push_back(ct);

        } else if (Token::Type::OPERATOR == token.type) {

            switch (token.op) {
            case OP::PARENTHESIS_OPEN:
                operatorStack.push(token);
                break;
            case OP::PARENTHESIS_CLOSE:
                {
                    bool parenthesisFound = false;
                    while (!operatorStack.empty()) {
                        auto op = operatorStack.top();
                        operatorStack.pop();
                        if (OP::PARENTHESIS_OPEN == op.op) {
                            parenthesisFound = true;
                            break;
                        }
                        tokens.push_back(MLayoutCoordinateToken(op.op));
                    }
                    if (!parenthesisFound)
                        raiseError("Mismatched parenthesis");
                }
                break;

            case OP::COMMA:
            case OP::COLON:
            case OP::AT:
                // pop all the operators
                while (!operatorStack.empty()) {
                    const auto &op = operatorStack.top();
                    if (OP::PARENTHESIS_OPEN == op.op)
                        raiseError("Mismatched parenthesis");
                    tokens.push_back(MLayoutCoordinateToken(op.op));
                    operatorStack.pop();
                }

                tokens.push_back(MLayoutCoordinateToken(token.op));
                break;

            case OP::MINUS:
            case OP::PLUS:
                if (lastToken.type == Token::Type::OPERATOR) {
                    if (OP::MINUS == token.op)
                        token.op = OP::UNARY_MINUS;
                    if (OP::PLUS == token.op)
                        token.op = OP::UNARY_PLUS;
                    token.priority = 4;
                }
                // falling through
            default:
                while (!operatorStack.empty()) {
                    const auto &op = operatorStack.top();
                    if (op.priority >= token.priority) {
                        tokens.push_back(MLayoutCoordinateToken(op.op));
                        operatorStack.pop();
                    } else
                        break;
                }
                operatorStack.push(token);
                break;
            }
        }

        lastToken = token;
    }


    // pop all the operators
    while (!operatorStack.empty()) {
        const auto &op = operatorStack.top();
        if (OP::PARENTHESIS_OPEN == op.op)
            raiseError("Mismatched parenthesis");
        tokens.push_back(MLayoutCoordinateToken(op.op));
        operatorStack.pop();
    }

    return tokens;
}


void MLayoutParser::processExpression(StrView input, String &output)
{
    using DT = ExpressionValue::DataType;

    output.clear();

    if (input.empty())
        return;

    expandVariables(input, output);

    // check if this is string expression, then we don't evaluate it
    for (auto&& c: output) {
        if (parseIsAlpha(c) || c == '%')
            return;
    }

    auto tokens = parseExpression(output);
    auto expression = evaluateExpressionValue(tokens);

    output.clear();

    bool wasValue = false;
    char separator = ',';
    for (auto &value : expression) {
        if (wasValue)
            output += separator;

        separator = value.separator;

        if (DT::STRING == value.type) {
            output += value.stringValue;
            wasValue = true;
        } else
            if (DT::FLOAT == value.type) {
                if (value.floatValue == 0)
                    output += '0';
                else {
                    apl::str_format_append(output, "%f", value.floatValue);
                    while (output[output.size()-1] == '0')
                        output.erase(output.size()-1, 1);
                    if (output[output.size()-1] == '.')
                        output.erase(output.size()-1, 1);
                }
                wasValue = true;
            }

    }
}

void MLayoutParser::expandVariables(StrView inputView, String &output)
{
    if (inputView.find_first_of('$') == inputView.npos) {
        output = inputView;
        return;
    }

    String input{inputView};
    String variable;

    for (;;) {
        output.clear();
        bool was_var = false;

        for (auto c_it = input.cbegin(), c_end = input.cend(); c_it != c_end; ) {

            if (*c_it != '$') {
                output.append(1, *c_it);
                ++c_it;
                continue;
            }

            was_var = true;

            char end_delimiter = ' ';
            ++c_it;
            if (c_it == c_end)
                break;
            if (*c_it == '(') {
                ++c_it;
                end_delimiter = ')';
            } else if (*c_it == '{') {
                ++c_it;
                end_delimiter = '}';
            }

            variable.clear();
            for (; c_it != c_end; ++c_it) {
                if (end_delimiter != ' ' && *c_it == end_delimiter) {
                    ++c_it;
                    break;
                }
                if (isalnum(*c_it) || *c_it == '_') {
                    variable.append(1, *c_it);
                    continue;
                }
                break;
            }

            auto varIt = variables.find(variable);
            if (varIt == variables.end())
                raiseError("Invalid variable: '%s'", value_);

            output.append(varIt->second);
        }

        if (!was_var)
            break;

        input = output;
    }
}


bool MLayoutParser::processCondition(StrView name, String &output)
{
    using namespace apl;

    using CO = ComparisonOperator;

    auto [condition, out] = str_split_trim(name, ':');
    output = out;

    //condition = str_trim(condition);
    //output = str_trim(output);
    if (output.empty()) {
        output = condition;
        return true;
    }
    if (condition.empty())
        return true;

    struct Condition {
        bool result{};
        bool negate{};
        String source;

        std::vector<Condition> andConditions;
    };

    std::vector<Condition> conditions;

    auto&& orConditionStrings = parse_std_vector_trim(condition, '|');

    for (auto &orString: orConditionStrings) {

        auto&& andConditionStrings = parse_std_vector_trim(orString, '&');

        auto& orCondition = conditions.emplace_back();

        for (auto& andString: andConditionStrings) {

            auto& andCondition = orCondition.andConditions.emplace_back();

            auto [conditionName, conditionValue] = str_split_trim(andString, '(');
            if (!conditionValue.empty()) {
                // the condition must end on ')'
                if (conditionValue[conditionValue.size()-1] != ')')
                    raiseError("')' is missing in condition");
                conditionValue.remove_suffix(1); //erase(conditionValue.size()-1);
            }

            andCondition.negate = false;
            if (conditionName.front() == '!' || conditionName.front() == '~') {
                andCondition.negate = true;
                conditionName.remove_prefix(1); //erase(0, 1);
                conditionName = str_trim(conditionName);
            }


            // extract the operator
            ComparisonOperator op = CO::EQUAL;

            if (!conditionValue.empty()) {
                char firstChar = conditionValue.front(); //[0];
                if (firstChar == '!' || firstChar == '<' || firstChar == '>' || firstChar == '=') {
                    conditionValue.remove_prefix(1); //erase(0, 1);
                    if (!conditionValue.empty()) {
                        char secondChar = conditionValue.front();//[0];
                        if (secondChar == '=') {
                            conditionValue.remove_prefix(1); //erase(0, 1);
                        }

                        if (firstChar == '!')
                            op = CO::NOT_EQUAL;
                        else if (firstChar == '=')
                            op = CO::EQUAL;
                        else if (firstChar == '>')
                            op = secondChar == '=' ? CO::GREATER_OR_EQUAL : CO::GREATER;
                        else if (firstChar == '<')
                            op = secondChar == '=' ? CO::LESS_OR_EQUAL : CO::LESS;
                    }
                }

            }

            // get the variable value
            StrView variableValue;

            // check among the parameters first
            if (auto it = variables.find(conditionName); it != variables.end())
                variableValue = it->second;

            andCondition.result = variableValue.empty() ? false : evaluateCondition(variableValue, conditionValue, op);

            if (andCondition.negate)
                andCondition.result = !andCondition.result;
        }
    }

    // evaluate all the conditions
    bool result = false;

    for (auto& orCondition : conditions) {
        orCondition.result = true;
        for (auto &andCondition : orCondition.andConditions) {
            if (andCondition.result == false) {
                orCondition.result = false;
                break;
            }
        }

        if (orCondition.result == true) {
            result = true;
            break;
        }
    }

    return result;
}

int MLayoutParser::parseHorzAlign(StrView s)
{
    if ("left" == s || "LEFT" == s)
        return M::AlignLeft;

    if ("center" == s || "CENTER" == s)
        return M::AlignCenter;

    if ("right" == s || "RIGHT" == s)
        return M::AlignRight;

    raiseError("Invalid horizontal align value: %s", s);
}

int MLayoutParser::parseVertAlign(StrView s)
{
    if ("bottom" == s || "BOTTOM" == s)
        return M::AlignBottom;

    if ("center" == s || "middle" == s || "CENTER" == s || "MIDDLE" == s)
        return M::AlignVCenter;

    if ("top" == s || "TOP" == s)
        return M::AlignTop;

    raiseError("Invalid vertical align value: %s", s);
}


void MLayoutParser::parseLayoutRectPair(StrView s, MLayoutPoint &pt, MLayoutSize &sz)
{
    auto v = apl::parse_std_vector_trim(s, ',');
    for (auto&& i: v) {
        if (i.empty())
            raiseError("Invalid Rectangle: %s", s);
    }
    if (v.size() != 4)
        raiseError("Invalid Rectangle: %s", s);

    pt.x = parseCoordinate(v[0]);
    pt.y = parseCoordinate(v[1]);
    sz.w = parseCoordinate(v[2]);
    sz.h = parseCoordinate(v[3]);

    //return true ;
}

MLayoutRect MLayoutParser::parseLayoutRect(StrView s)
{
    auto&& v = apl::parse_std_vector_trim(s, ',');
    for (auto& i : v) {
        if (i.empty())
            mthrowError("Invalid Rectangle: %s", s);
    }
    if (v.size() != 4)
        mthrowError("Invalid Rectangle: %s", s);

    return {parseCoordinate(v[0]), parseCoordinate(v[1]), parseCoordinate(v[2]), parseCoordinate(v[3])};
}

bool MLayoutParser::processVersion(StrView value)
{
    using namespace apl;

    auto vs = parse_std_vector_trim(value, '.');

    versionHi = versionLo = versionPatch = 0;

    if (vs.size() >= 1)
        parse_int(vs[0], versionHi);
    if (vs.size() >= 2)
        parse_int(vs[1], versionLo);
    if (vs.size() >= 3)
        parse_int(vs[2], versionPatch);

    versionDecimal = versionHi * 10000 + versionLo * 100 + versionPatch;

    return true;
}

Color MLayoutParser::parseColor(StrView value)
{
    using namespace apl;

    Color color;

    auto v = str_trim(value);
    auto colorIt = colorMap_.find(v);
    if (colorIt != colorMap_.end()) {
        color.setRGB(colorIt->second);
        return color;
    }

    const auto &c = v[0];

    if (c != '#' && (c < '0' || c > '9') && (c < 'a' && c > 'f') && (c < 'A' && c > 'F'))
        raiseError("Invalid Color value: %s", value);

    auto l = v.length();
    auto vIt = v.begin(), vEnd = v.end();
    if (*vIt == '#')
        ++vIt, --l;

    auto colorRGB = parseUIntHexIt(vIt, vEnd);

    //if ( !parse_uint_basic(vIt,vEnd,colorRGB,16) )
    //	return raiseError("Invalid Color Value: %s", value ) ;

    // deduce if this was RGB or RGBA
    if (l == 8)
        color.setRGBA(colorRGB);
    else
        color.setRGB(colorRGB);

    return color;
}

void MLayoutParser::raiseError(StrView error)
{
    auto adjustedLine = line_;
    String adjustedFilename = filename_;

    for (auto&& dep: deps_) {
        if (line_ < dep.line)
            break;
        if (line_ >= dep.line + dep.count) {
            adjustedLine -= dep.count;
        } else {
            adjustedFilename = dep.filepath;
            adjustedLine = line_ - dep.line;
        }
    }

    adjustedFilename = apl::extractFileBase(adjustedFilename);

    mthrowError("%s : %d : %s", adjustedFilename, adjustedLine, error);
}



bool MLayoutParser::parseVar(StrView name, StrView value)
{
    using namespace apl;

    if ("var" == name || "param" == name) {
        char delim = ':';
        if (value.find_first_of(delim) == value.npos)
            delim = ',';
        auto [varName, varValue] = str_split_trim(value, delim);
        variables[varName] = varValue;
        return true;
    }

    auto [nameBase, nameEx] = str_split_trim(name, ' ');

    if ("var" == nameBase || "param" == nameBase) {
        
        // Trick to avoid misaligned block
        // Need to come up with a better solution
        String nameExStr{nameEx};
        variables[nameExStr] = value_;
        return true;
    }

    return false;
}

}
