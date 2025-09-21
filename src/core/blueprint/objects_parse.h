/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once 

#include <mangl/mangl.h>

#include <list>
#include <map>

namespace mangl {

enum class ArgType {
    unknown,
    Expression,
    Value,
    Variable,
};
enum class OperatorType {
    unknown,
    UnaryMinus,
    Minus,
    Plus,
    Multiplication,
    Division,
};

struct Coords {
    RealGL x{};
    RealGL y{};

};

struct Args {
    String id;
    Coords by{};
    Coords to{};
};

struct Expression;

struct Operator {
    OperatorType type;
    std::unique_ptr<Expression> arg1;
    std::unique_ptr<Expression> arg2;
};

enum class ExpType {
    unknown,
    Value,
    Variable,
    Operator,
};

struct Expression {
    ArgType argType;
    ExpType expType;
    RealGL value;
    String variable;
    Operator oper;
};

struct Condition {
    Expression expression;

};

struct Block;

struct Op {
    String type;
    Condition condition;
    Args args;   

    std::unique_ptr<Block> block; // = std::make_unique<Block>(); //Block block;
    std::unique_ptr<Block> elseBlock; // = std::make_unique<Block>(); //Block else_;
};

struct Block {
    //std::map<int, std::unique_ptr<Op>> ops;

    std::vector<Op> ops;
    //std::list<std::unique_ptr<Op>> ops;
    //std::vector<std::unique_ptr<Op>> ops;

    /*Block(std::vector<Op> ops)
    {

    }
    ~Block()
    {

    }*/

};



struct Function {
    Block block;
};



//void loadFunction(StrView filename);

}