//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "LuauFunction.hpp"
#include "Oneiro/Core/Logger.hpp"

#include "Luau/AstQuery.h"
#include "Luau/BytecodeBuilder.h"
#include "Luau/Compiler.h"
#include "Luau/Config.h"
#include "Luau/FileResolver.h"
#include "Luau/Frontend.h"
#include "Luau/IostreamHelpers.h"
#include "Luau/Linter.h"
#include "Luau/Location.h"
#include "Luau/ModuleResolver.h"
#include "Luau/Parser.h"
#include "lua.h"
#include "lualib.h"

#include <memory>
#include <type_traits>
#include <utility>

namespace
{
    struct FileResolver : Luau::FileResolver
    {
        std::optional<Luau::SourceCode> readSource(const Luau::ModuleName& name) override;

        std::optional<Luau::ModuleInfo> resolveModule(const Luau::ModuleInfo* context, Luau::AstExpr* expr) override;

        [[nodiscard]] std::string getHumanReadableModuleName(const Luau::ModuleName& name) const override;

        [[nodiscard]] std::optional<std::string> getEnvironmentForModule(const Luau::ModuleName& name) const override;

        std::unordered_map<Luau::ModuleName, std::string> source;
    };

    struct ConfigResolver : Luau::ConfigResolver
    {
        ConfigResolver();

        [[nodiscard]] const Luau::Config& getConfig(const Luau::ModuleName& name) const override;

        Luau::Config defaultConfig;
    };
} // namespace

namespace oe::Luau
{
    class State
    {
    public:
        State();

        [[nodiscard]] lua_State* GetState() const;

        [[nodiscard]] const FileResolver* GetFileResolver() const;

        [[nodiscard]] const ConfigResolver* GetConfigResolver() const;

        [[nodiscard]] ::Luau::SourceModule* GetSourceModule() const;

        [[nodiscard]] const ::Luau::Frontend* GetFrontend() const;

        [[nodiscard]] const ::Luau::TypeChecker* GetTypeChecker() const;

        ::Luau::LintResult LintScript(const std::string& scriptSource);

        ::Luau::CheckResult CheckScript(const std::string& scriptSource);

        std::vector<::Luau::ParseError> ParseScript(const std::string& scriptSource, ::Luau::ParseOptions options = {});

        bool CompileScript(::Luau::CompileOptions options = {});
        bool Load();
        bool Call();
        bool Script(const std::string& scriptSource);

        template <class T>
        static T* NewUserDataFunc(lua_State* state, const std::string& typeName);

        template <class T>
        void AddUserType(std::string typeName, const FunctionsFactory& regsFactory);

    private:
        lua_State* mState{};
        FileResolver mFileResolver;
        ConfigResolver mConfigResolver;
        std::unique_ptr<::Luau::SourceModule> mSourceModule;
        ::Luau::Frontend mFrontend;
        ::Luau::TypeChecker& mTypeChecker;
        ::Luau::BytecodeBuilder mBytecodeBuilder;
        static std::unordered_map<std::string, AnyCallable<void>> mLuauFunctions;
    };

    State::State() : mFrontend(&mFileResolver, &mConfigResolver), mTypeChecker(mFrontend.typeChecker)
    {
        mConfigResolver.defaultConfig.mode = ::Luau::Mode::Strict;
        mConfigResolver.defaultConfig.enabledLint.warningMask = ~0ULL;
        mConfigResolver.defaultConfig.parseOptions.captureComments = true;

        freeze(mFrontend.typeChecker.globalTypes);
        freeze(mFrontend.typeCheckerForAutocomplete.globalTypes);

        mState = luaL_newstate();
        luaL_openlibs(mState);
    }

    lua_State* State::GetState() const
    {
        return mState;
    }

    const FileResolver* State::GetFileResolver() const
    {
        return &mFileResolver;
    }

    const ConfigResolver* State::GetConfigResolver() const
    {
        return &mConfigResolver;
    }

    ::Luau::SourceModule* State::GetSourceModule() const
    {
        return mSourceModule.get();
    }

    const ::Luau::Frontend* State::GetFrontend() const
    {
        return &mFrontend;
    }

    const ::Luau::TypeChecker* State::GetTypeChecker() const
    {
        return &mTypeChecker;
    }

    ::Luau::LintResult State::LintScript(const std::string& scriptSource)
    {
        return {};
    }

    ::Luau::CheckResult State::CheckScript(const std::string& scriptSource)
    {
        return {};
    }

    std::vector<::Luau::ParseError> State::ParseScript(const std::string& scriptSource, ::Luau::ParseOptions options)
    {
        mSourceModule = std::make_unique<::Luau::SourceModule>();
        auto parseRes = ::Luau::Parser::parse(scriptSource.c_str(), scriptSource.size(), *mSourceModule->names, *mSourceModule->allocator, options);
        mSourceModule->name = "main";
        mSourceModule->root = parseRes.root;
        mSourceModule->mode = parseMode(parseRes.hotcomments);
        mSourceModule->hotcomments = std::move(parseRes.hotcomments);
        return parseRes.errors;
    }

    bool State::CompileScript(::Luau::CompileOptions options)
    {
        if (!mSourceModule)
            return false;
        try
        {
            ::Luau::compileOrThrow(mBytecodeBuilder, mSourceModule->root, *mSourceModule->names, options);
        }
        catch (const std::exception& ex)
        {
            log::get("log")->warn("Failed to compile luau script. Error description: " - ex.what());
            return false;
        }

        return true;
    }

    bool State::Load()
    {
        if (!mSourceModule)
            return false;
        return luau_load(mState, ("=" + mSourceModule->name).c_str(), mBytecodeBuilder.getBytecode().data(), mBytecodeBuilder.getBytecode().size(),
                         0) == 0;
    }

    bool State::Call()
    {
        if (mState != nullptr)
        {
            lua_call(mState, 0, 0);
            return true;
        }
        return false;
    }

    bool State::Script(const std::string& scriptSource)
    {
        return !(!ParseScript(scriptSource).empty() || !CompileScript() || !Load() || !Call());
    }

    template <class T>
    T* State::NewUserDataFunc(lua_State* state, const std::string& typeName)
    {
        auto vPtr = std::make_shared<T>();
        T** ptr = (T**)lua_newuserdata(state, sizeof(T*));
        luaL_newmetatable(state, "");
        auto func = mLuauFunctions.find(typeName);
        if (func != mLuauFunctions.end())
            (*(func)).second(state, vPtr.get());
        return *ptr;
    }

    template <class T>
    void State::AddUserType(std::string typeName, const FunctionsFactory& regsFactory)
    {
        std::vector<luaL_Reg> regs{};
        auto luaFuncs = regsFactory.GetLuaFunctions();
        auto funcs = regsFactory.GetFunctions();
        for (const auto& luaFunc : luaFuncs)
        {
            if (std::strcmp(luaFunc.first, "new") == 0)
            {
                continue;
            }
            regs.push_back({luaFunc.first, luaFunc.second});
        }

        for (const auto& func : funcs)
        {
            if (std::strcmp(func.first, "new") == 0)
            {
                mLuauFunctions[typeName] = func.second;
                regs.push_back({"new", Function<T*, lua_State*>::template Create<std::string>(NewUserDataFunc<T>, typeName)});
            }
        }
        regs.push_back({nullptr, nullptr});
        luaL_register(mState, typeName.c_str(), regs.data());
    }

    inline std::unordered_map<std::string, AnyCallable<void>> State::mLuauFunctions{};
} // namespace oe::Luau

namespace
{
    std::optional<Luau::SourceCode> FileResolver::readSource(const Luau::ModuleName& name)
    {
        auto iter = source.find(name);
        if (iter == source.end())
            return std::nullopt;

        return Luau::SourceCode{iter->second, Luau::SourceCode::Module};
    }

    std::optional<Luau::ModuleInfo> FileResolver::resolveModule(const Luau::ModuleInfo* context, Luau::AstExpr* expr)
    {
        if (auto* glob = expr->as<Luau::AstExprGlobal>())
            return Luau::ModuleInfo{glob->name.value};

        return std::nullopt;
    }

    std::string FileResolver::getHumanReadableModuleName(const Luau::ModuleName& name) const
    {
        return name;
    }

    std::optional<std::string> FileResolver::getEnvironmentForModule(const Luau::ModuleName& name) const
    {
        return std::nullopt;
    }

    ConfigResolver::ConfigResolver()
    {
        defaultConfig.mode = Luau::Mode::Nonstrict;
        defaultConfig.enabledLint.warningMask = ~0ULL;
        defaultConfig.parseOptions.captureComments = true;
    }

    const Luau::Config& ConfigResolver::getConfig(const Luau::ModuleName& name) const
    {
        return defaultConfig;
    }
} // namespace