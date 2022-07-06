//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "lua.h"
#include "lualib.h"

#include <type_traits>
#include <string>

const char* fieldName{};

namespace oe::Luau
{
    template <class T>
    struct UnsupportedType;

    template <class T>
    auto GetValue(lua_State* state, int idx);

    template <class T>
    void PushValue(lua_State* state, const T& val);

    void PushValues(lua_State* /*unused*/);

    template <class T, class... Args>
    void PushValues(lua_State* state, const T& val, const Args&... args);

    void PushVariables(lua_State* /*unused*/);

    template <class T, class... Args>
    void PushVariables(lua_State* state, T value, Args... args);

    template <class T>
    auto GetValue(lua_State* state, int idx)
    {
        using decayT = std::decay_t<T>;

        if constexpr (std::is_same_v<decayT, bool>)
        {
            return bool(luaL_checkboolean(state, idx));
        }
        else if constexpr (std::is_same_v<decayT, const char*> || std::is_same_v<decayT, std::string>)
        {
            return luaL_checkstring(state, idx);
        }
        else if constexpr (std::is_arithmetic_v<decayT>)
        {
            return decayT(luaL_checknumber(state, idx));
        }
        else if constexpr (std::is_same_v<T, lua_State*>)
        {
            return state;
        }
        else if constexpr (std::is_same_v<T, void>)
        {
            return;
        }
        else
        {
            static_assert(UnsupportedType<T>::value);
        }
    }

    template <class T>
    void PushValue(lua_State* state, const T& val)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            lua_pushboolean(state, val);
        }
        else if constexpr (std::is_same_v<T, const char*>)
        {
            lua_pushstring(state, val);
        }
        else if constexpr (std::is_arithmetic_v<T>)
        {
            lua_pushnumber(state, double(val));
        }
        else
        {
            int idx = lua_gettop(state);
            lua_pushvalue(state, idx);
        }
    }

    void PushValues(lua_State*)
    {
    }

    template <class T, class... Args>
    void PushValues(lua_State* state, const T& val, const Args&... args)
    {
        PushValue(state, val);
        PushValues(state, args...);
    }

    void PushVariables(lua_State*)
    {
    }

    template <class T, class... Args>
    void PushVariables(lua_State* state, T value, Args... args)
    {
        if constexpr (std::is_same<T, const char*>::value)
        {
            fieldName = value;
        }
        else
        {
            PushValue(state, value);
            lua_setfield(state, -2, fieldName);
        }
        PushVariables(state, args...);
        fieldName = {};
    }
} // namespace oe::Luau