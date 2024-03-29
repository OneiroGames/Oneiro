//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_API __declspec(dllexport)
#define OE_MODULE_API __declspec(dllexport)
#define OE_MAKE_ASSET_HASH(x) std::hash<std::string>{}(x)

#define OE_CORE_TRACE(...) ::oe::EngineApi::GetLogger()->GetEngineLogger()->trace("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_INFO(...) ::oe::EngineApi::GetLogger()->GetEngineLogger()->info("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_WARN(...) ::oe::EngineApi::GetLogger()->GetEngineLogger()->warn("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_ERROR(...) ::oe::EngineApi::GetLogger()->GetEngineLogger()->error("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_CRITICAL(...) ::oe::EngineApi::GetLogger()->GetEngineLogger()->critical("[ENGINE] " + fmt::format(__VA_ARGS__))

#define OE_TRACE(...) ::oe::EngineApi::GetLogger()->GetClientLogger()->trace("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_INFO(...) ::oe::EngineApi::GetLogger()->GetClientLogger()->info("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_WARN(...) ::oe::EngineApi::GetLogger()->GetClientLogger()->warn("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_ERROR(...) ::oe::EngineApi::GetLogger()->GetClientLogger()->error("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_CRITICAL(...) ::oe::EngineApi::GetLogger()->GetClientLogger()->critical(fmt::format(__VA_ARGS__))

#define OE_MAIN(applicationFunc)                                 \
int main(int /*argc*/, char** /*argv*/)                      \
{                                                            \
auto application = applicationFunc();                    \
auto engine = std::make_unique<oe::Engine>();            \
try                                                      \
{                                                        \
engine->PreInit(application.get());                  \
engine->Init();                                      \
engine->Run();                                       \
}                                                        \
catch (const std::exception& exception)                  \
{                                                        \
OE_CORE_CRITICAL("Exception: {}", exception.what()); \
engine->Shutdown();                                  \
return 1;                                            \
}                                                        \
engine->Shutdown();                                      \
application.reset();                                     \
engine.reset();                                          \
return 0;                                                \
}                                                            \
\

#define OE_DECLARE_FLAG_TYPE(FLAG_TYPE, FLAG_BITS, BASE_TYPE)                                 \
                                                                                              \
	struct OE_API FLAG_TYPE                                                                   \
	{                                                                                         \
		BASE_TYPE flags = static_cast<BASE_TYPE>(0);                                          \
                                                                                              \
		constexpr FLAG_TYPE() noexcept = default;                                             \
		constexpr explicit FLAG_TYPE(BASE_TYPE in) noexcept : flags(in) {}                    \
		constexpr FLAG_TYPE(FLAG_BITS in) noexcept : flags(static_cast<BASE_TYPE>(in)) {}     \
		constexpr bool operator==(FLAG_TYPE const& right) const                               \
		{                                                                                     \
			return flags == right.flags;                                                      \
		}                                                                                     \
		constexpr bool operator!=(FLAG_TYPE const& right) const                               \
		{                                                                                     \
			return flags != right.flags;                                                      \
		}                                                                                     \
		constexpr explicit operator BASE_TYPE() const                                         \
		{                                                                                     \
			return flags;                                                                     \
		}                                                                                     \
		constexpr explicit operator bool() const noexcept                                     \
		{                                                                                     \
			return flags != 0;                                                                \
		}                                                                                     \
	};                                                                                        \
	constexpr FLAG_TYPE operator|(FLAG_TYPE a, FLAG_TYPE b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(a.flags | b.flags);                                     \
	}                                                                                         \
	constexpr FLAG_TYPE operator&(FLAG_TYPE a, FLAG_TYPE b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(a.flags & b.flags);                                     \
	}                                                                                         \
	constexpr FLAG_TYPE operator^(FLAG_TYPE a, FLAG_TYPE b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(a.flags ^ b.flags);                                     \
	}                                                                                         \
	constexpr FLAG_TYPE operator~(FLAG_TYPE a) noexcept                                       \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(~a.flags);                                              \
	}                                                                                         \
	constexpr FLAG_TYPE& operator|=(FLAG_TYPE& a, FLAG_TYPE b) noexcept                       \
	{                                                                                         \
		a.flags = (a.flags | b.flags);                                                        \
		return a;                                                                             \
	}                                                                                         \
	constexpr FLAG_TYPE& operator&=(FLAG_TYPE& a, FLAG_TYPE b) noexcept                       \
	{                                                                                         \
		a.flags = (a.flags & b.flags);                                                        \
		return a;                                                                             \
	}                                                                                         \
	constexpr FLAG_TYPE operator^=(FLAG_TYPE& a, FLAG_TYPE b) noexcept                        \
	{                                                                                         \
		a.flags = (a.flags ^ b.flags);                                                        \
		return a;                                                                             \
	}                                                                                         \
	constexpr FLAG_TYPE operator|(FLAG_BITS a, FLAG_BITS b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(static_cast<BASE_TYPE>(a) | static_cast<BASE_TYPE>(b)); \
	}                                                                                         \
	constexpr FLAG_TYPE operator&(FLAG_BITS a, FLAG_BITS b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(static_cast<BASE_TYPE>(a) & static_cast<BASE_TYPE>(b)); \
	}                                                                                         \
	constexpr FLAG_TYPE operator~(FLAG_BITS key) noexcept                                     \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(~static_cast<BASE_TYPE>(key));                          \
	}                                                                                         \
	constexpr FLAG_TYPE operator^(FLAG_BITS a, FLAG_BITS b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(static_cast<BASE_TYPE>(a) ^ static_cast<BASE_TYPE>(b)); \
	}\

#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

#include "fmt/core.h"
#include "flecs.h"
#include "flecs/addons/cpp/flecs.hpp"
#include "spdlog/spdlog.h"

namespace oe
{
	template <class T>
	using Ref = std::shared_ptr<T>;

	template <class T, class... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
} // namespace oe