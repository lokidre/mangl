/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_layout.h"

namespace mangl {

auto MLayoutParser::evaluateExpressionValue(MLayoutCoordinate::Tokens &tokens)-> Expression
{
    using TT = MLayoutCoordinateToken::Type;
    using OP = MLayoutCoordinateToken::Operator;
    using DT = ExpressionValue::DataType;

    Real currentValue{};

    Expression expression;

    // initizalize the tokens and evaluate all the values
    for (auto &t : tokens) {
        t.rtValid = true;
        if (TT::VALUE == t.type)
            t.rtValue = t.value.value;
    }

    auto end = tokens.end();

    auto a = end, b = end;

    bool wasOperator;
    do {
        wasOperator = false;

        for (auto t_it = tokens.begin(); t_it != end; ++t_it) {
            auto &t = *t_it;
            if (!t.rtValid)
                continue;
            if (TT::VALUE == t.type) {
                a = b;
                b = t_it;
                currentValue = b->rtValue;
            } else if (TT::OPERATOR == t.type) {

                if (OP::COMMA == t.op || OP::COLON == t.op || OP::AT == t.op) {
                    expression.resize(expression.size()+1);
                    auto &expr = expression.back();
                    expr.floatValue = currentValue;
                    expr.type = DT::FLOAT;
                    expr.separator = (char)t.op;
                    currentValue = 0;
                    t.rtValid = false;
                } else {
                    switch (t.op) {
                    case OP::PLUS:	a->rtValue += b->rtValue;	break;
                    case OP::MINUS:	a->rtValue -= b->rtValue;	break;
                    case OP::UNARY_MINUS:	b->rtValue = -b->rtValue;	break;
                    case OP::UNARY_PLUS:	break;
                    case OP::MUL:	a->rtValue *= b->rtValue;	break;
                    case OP::DIV:	a->rtValue /= b->rtValue;	break;
                    default:
                        break;
                    }

                    if (OP::UNARY_MINUS != t.op && OP::UNARY_PLUS != t.op)
                        b->rtValid = false;

                    t.rtValid = false;

                    wasOperator = true;
                    break;
                }
            }
        }
    } while (wasOperator);

    expression.resize(expression.size()+1);
    auto &expr = expression.back();
    expr.floatValue = currentValue;
    expr.type = DT::FLOAT;

    return expression;
}


Cord MLayoutParser::evaluateCoordinateValue(const MLayoutCoordinateValue &c, Cord total) const
{
    using TYPE = MLayoutCoordinateValue::Type;
    using RT = MLayoutCoordinateValue::RefType;
    using RC = MLayoutCoordinateValue::RefComponent;

    switch (c.type) {
    case TYPE::VALUE:   return c.value; //* scale_ ;
    case TYPE::PERCENT:	return c.value * total / 100;
    case TYPE::TOP:
    case TYPE::RIGHT:	return total;
    case TYPE::BOTTOM:
    case TYPE::LEFT:	return 0;
    case TYPE::CENTER:	return total/2;
    case TYPE::REF:
        if (RT::TEXTURE == c.refType) {
            if (RC::WIDTH == c.refComponent)
                return atlas_->texture(c.refName).w();
            if (RC::HEIGHT == c.refComponent)
                return atlas_->texture(c.refName).h();
        } else {
            return onXVal(c);
        }
    default:
        break;
    }

    return c.value;
}

Cord MLayoutParser::evaluateCoordinate(MLayoutCoordinate &c, Cord total)
{
    using TOKEN = MLayoutCoordinateToken;
    using TT = MLayoutCoordinateToken::Type;
    using OP = MLayoutCoordinateToken::Operator;
    using DT = ExpressionValue::DataType;

    Cord currentValue{};

    // initialize the tokens and evaluate all the values
    for (auto &t : c.tokens) {
        t.rtValid = true;
        if (TT::VALUE == t.type)
            t.rtValue = evaluateCoordinateValue(t.value, total);
    }

    const auto end = c.tokens.end();

    auto a = end, b = end;

    bool wasOperator;
    do {
        wasOperator = false;

        for (auto t_it = c.tokens.begin(); t_it != end; ++t_it) {
            auto &t = *t_it;
            if (!t.rtValid)
                continue;
            if (TT::VALUE == t.type) {
                a = b;
                b = t_it;
                currentValue = b->rtValue;
            } else if (TT::OPERATOR == t.type) {
                switch (t.op) {
                case OP::PLUS:	a->rtValue += b->rtValue;	break;
                case OP::MINUS:	a->rtValue -= b->rtValue;	break;
                case OP::UNARY_MINUS:	b->rtValue = -b->rtValue;	break;
                case OP::UNARY_PLUS:	break;
                case OP::MUL:	a->rtValue *= b->rtValue;	break;
                case OP::DIV:	a->rtValue /= b->rtValue;	break;
                default: break;
                }

                if (t.op != OP::UNARY_MINUS && OP::UNARY_PLUS != t.op)
                    b->rtValid = false;

                t.rtValid = false;

                wasOperator = true;
                break;
            }
        }
    } while (wasOperator);

    return currentValue;
}

bool MLayoutParser::evaluateCondition(StrView variableValue, StrView conditionValue, const ComparisonOperator &op)
{
    using namespace apl;

    using OP = MLayoutCoordinateToken::Operator;
    using CO = ComparisonOperator;

    // if the value is empty, we need to evaluate it with regards to boolean
    if (conditionValue.empty()) {

        bool boolResult = false;
        if (parse_bool(variableValue, &boolResult))
            return boolResult;

        int intResult{};
        if (parse_int(variableValue, intResult))
            return intResult != 0;

        Real floatResult{};
        if (parse_real(variableValue, floatResult))
            return floatResult != 0;

        return false;
    }

    {
        bool varValue = false, condValue = false;
        if (parse_bool(variableValue, &varValue) && parse_bool(conditionValue, &condValue)) {
            switch (op) {
            case CO::GREATER: return varValue > condValue;
            case CO::GREATER_OR_EQUAL: return varValue >= condValue;
            case CO::LESS: return varValue < condValue;
            case CO::LESS_OR_EQUAL: return varValue <= condValue;
            case CO::NOT_EQUAL: return varValue != condValue;
            default: return varValue == condValue;
            }
        }
    }

    {
        int varValue = 0, condValue = 0;
        if (parse_int(variableValue, varValue) && parse_int(conditionValue, condValue)) {
            switch (op) {
            case CO::GREATER: return varValue > condValue;
            case CO::GREATER_OR_EQUAL: return varValue >= condValue;
            case CO::LESS: return varValue < condValue;
            case CO::LESS_OR_EQUAL: return varValue <= condValue;
            case CO::NOT_EQUAL: return varValue != condValue;
            default: return varValue == condValue;
            }
        }
    }

    {
        Cord varValue = 0, condValue = 0;
        if (parse_real(variableValue, varValue) && parse_real(conditionValue, condValue)) {
            switch (op) {
            case CO::GREATER: return varValue > condValue;
            case CO::GREATER_OR_EQUAL: return varValue >= condValue;
            case CO::LESS: return varValue < condValue;
            case CO::LESS_OR_EQUAL: return varValue <= condValue;
            case CO::NOT_EQUAL: return varValue != condValue;
            default: return varValue == condValue;
            }
        }
    }

    // if nothing worked, then just comparing them as strings
    switch (op) {
    case CO::GREATER: return variableValue > conditionValue;
    case CO::GREATER_OR_EQUAL: return variableValue >= conditionValue;
    case CO::LESS: return variableValue < conditionValue;
    case CO::LESS_OR_EQUAL: return variableValue <= conditionValue;
    case CO::NOT_EQUAL: return variableValue != conditionValue;
    default:
        break;
    }

    return variableValue == conditionValue;
}

}
