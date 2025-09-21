/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#include "mangl_blueprint.h"

namespace mangl::blueprint {

Real Value::getVariableReal() const noexcept
{
    return std::get<VariableP>(value)->value.getReal();
}

String Value::getVariableString() const noexcept
{
    return std::get<VariableP>(value)->value.getString();
}


void Value::setVariable(VariableP var) noexcept 
{
    type = Type::Variable;
    value = var;
    valid = var->value.valid;
    evaluated = var->value.evaluated;
}



}  // namespace mangl::blueprint
