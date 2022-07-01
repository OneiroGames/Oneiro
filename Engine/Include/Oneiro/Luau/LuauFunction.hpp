//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "LuauCore.hpp"

#include "lua.h"

#include <any>
#include <utility>
#include <unordered_map>
#include <functional>
#include "Oneiro/Core/Logger.hpp"

namespace
{
    template <class Ret>
    struct AnyCallable
    {
        AnyCallable() = default;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
        template <class... Args>
        AnyCallable(std::function<Ret(Args...)> fun) : mAny(fun) {}
#pragma clang diagnostic pop

        template <class... Args>
        Ret operator()(Args... args)
        {
            return std::invoke(std::any_cast<std::function<Ret(Args...)>>(mAny), std::forward<Args>(args)...);
        }
        std::any mAny;
    };
}


namespace oe::Luau
{
    template <class T>
    void CallFunction(lua_State* state, const char* name, T val);

    template <class RType = void, class T, class... Args>
    RType CallFunction(lua_State* state, const char* name, T val, Args... args);

    template <class Ret, class... Args>
    class Function
    {
    public:
        template <class... FArgs>
        static lua_CFunction Create(std::function<Ret(Args..., FArgs...)> func, FArgs... fArgs);

        static lua_CFunction Create(std::function<Ret(Args...)> func);

        static void Create(lua_State* state, const char* name, std::function<Ret(Args...)> func);
    private:
        template <int... Idx, class... FArgs>
        static lua_CFunction CreateLuaCFunction(std::integer_sequence<int, Idx...> /*unused*/, std::function<Ret(Args..., FArgs...)> func, FArgs... fargs);

        template <int... Idx>
        static lua_CFunction CreateLuaCFunction(std::integer_sequence<int, Idx...> /*unused*/, std::function<Ret(Args...)> func);
    };

    class FunctionsFactory
    {
    public:
        void PushFunction(const char* name, lua_CFunction function);

        template<class... Args>
        void PushFunction(const char* name, std::function<void(Args...)> function);

        [[nodiscard]] constexpr const std::vector<std::pair<const char*, lua_CFunction>>& GetLuaFunctions() const;
        [[nodiscard]] constexpr const std::vector<std::pair<const char*, AnyCallable<void>>>& GetFunctions() const;
    private:
        std::vector<std::pair<const char*, lua_CFunction>> mLuaFunctions{};
        std::vector<std::pair<const char*, AnyCallable<void>>> mFunctions{};
    };

    template <class T>
    void CallFunction(lua_State* state, const char* name, T val)
    {
        lua_getglobal(state, name);
        PushValue(state, val);
        if (lua_pcall(state, 1, 0, 0) != 0)
        {
            log::get("log")->warn("Failed to call " + std::string(name) + " function! Error: " + lua_tostring(state, -1));
            return;
        }
    }

    template <class RType, class T, class... Args>
    RType CallFunction(lua_State* state, const char* name, T val, Args... args)
    {
        lua_getglobal(state, name);
        PushValues(state, val, args...);
        if (lua_pcall(state, sizeof...(args) + 1, 1, 0) != 0)
        {
            log::get("log")->warn("Failed to call " + std::string(name) + " function! Error: " + lua_tostring(state, -1));
            return RType();
        }

        return GetValue<RType>(state, -1);
    }

    template <class Ret, class... Args>
    template <class... FArgs>
    lua_CFunction Function<Ret, Args...>::Create(std::function<Ret(Args..., FArgs...)> func, FArgs... fArgs)
    {
        using IS = std::make_integer_sequence<int, sizeof...(Args)>;

        return CreateLuaCFunction(IS{}, func, fArgs...);
    }

    template <class Ret, class... Args>
    lua_CFunction Function<Ret, Args...>::Create(std::function<Ret(Args...)> func)
    {
        using IS = std::make_integer_sequence<int, sizeof...(Args)>;

        return CreateLuaCFunction(IS{}, func);
    }

    template <class Ret, class... Args>
    void Function<Ret, Args...>::Create(lua_State* state, const char* name, std::function<Ret(Args...)> func)
    {
        using IS = std::make_integer_sequence<int, sizeof...(Args)>;

        static luaL_Reg reg{name, CreateLuaCFunction(IS{}, func)};
        lua_pushvalue(state, LUA_GLOBALSINDEX);
        luaL_register(state, nullptr, &reg);
        lua_pop(state, 1);
    }

    template <class Ret, class... Args>
    template <int... Idx, class... FArgs>
    lua_CFunction Function<Ret, Args...>::CreateLuaCFunction(std::integer_sequence<int, Idx...>, std::function<Ret(Args..., FArgs...)> func,
                                                             FArgs... fargs)
    {
        static std::function<Ret(Args..., FArgs...)> function = func;
        static auto pack = std::make_tuple(fargs...);
        return [](lua_State* L) -> int
        {
            if constexpr (std::is_same_v<Ret, void>)
            {
                function(GetValue<Args>(L, 1 + Idx)..., std::get<FArgs...>(pack));
                return 0;
            }
            else
            {
                Ret r = function(GetValue<Args>(L, 1 + Idx)..., std::get<FArgs...>(pack));
                PushValue(L, r);
                return 1;
            }
        };
    }

    template <class Ret, class... Args>
    template <int... Idx>
    lua_CFunction Function<Ret, Args...>::CreateLuaCFunction(std::integer_sequence<int, Idx...>, std::function<Ret(Args...)> func)
    {
        static std::function<Ret(Args...)> fn = func;
        return [](lua_State* L) -> int
        {
            if constexpr (std::is_same_v<Ret, void>)
            {
                fn(GetValue<Args>(L, 1 + Idx)...);
                return 0;
            }
            else
            {
                Ret r = fn(GetValue<Args>(L, 1 + Idx)...);
                PushValue(L, r);
                return 1;
            }
        };
    }

    void FunctionsFactory::PushFunction(const char* name, lua_CFunction function)
    {
        mLuaFunctions.emplace_back(name, function);
    }

    template <class... Args>
    void FunctionsFactory::PushFunction(const char* name, std::function<void(Args...)> function)
    {
        mFunctions.push_back({name, function});
    }

    constexpr const std::vector<std::pair<const char*, lua_CFunction>>& FunctionsFactory::GetLuaFunctions() const
    {
        return mLuaFunctions;
    }

    constexpr const std::vector<std::pair<const char*, AnyCallable<void>>>& FunctionsFactory::GetFunctions() const
    {
        return mFunctions;
    }
} // namespace oe::Script::Luau