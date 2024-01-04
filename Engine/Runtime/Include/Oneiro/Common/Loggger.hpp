//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Utils.hpp"

#include "spdlog/spdlog.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
constexpr inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector);

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
constexpr inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix);

template <typename OStream, typename T, glm::qualifier Q>
constexpr inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion);

#define DEFINE_FORMATTER(type, ...)                                                     \
	template <>                                                                         \
	struct fmt::formatter<type>                                                         \
	{                                                                                   \
		char presentation = 'f';                                                        \
		constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())        \
		{                                                                               \
			const auto* iter = ctx.begin();                                             \
			const auto* end = ctx.end();                                                \
			if (iter != end && (*iter == 'f' || *iter == 'e'))                          \
				presentation = *iter++;                                                 \
                                                                                        \
			if (iter != end && *iter != '}')                                            \
				throw format_error("invalid format");                                   \
                                                                                        \
			return iter;                                                                \
		}                                                                               \
                                                                                        \
		template <typename FormatContext>                                               \
		auto format(const type& value, FormatContext& ctx) const -> decltype(ctx.out()) \
		{                                                                               \
			return fmt::format_to(ctx.out(), __VA_ARGS__);                              \
		}                                                                               \
	};

namespace oe
{
	class Logger
	{
	public:
		Logger();

		static Logger* Get();

		[[nodiscard]] const Ref<spdlog::logger>& GetEngineLogger() const noexcept;
		[[nodiscard]] const Ref<spdlog::logger>& GetClientLogger() const noexcept;

	private:
		Ref<spdlog::logger> mEngineLogger{};
		Ref<spdlog::logger> mClientLogger{};
	};
} // namespace oe

#define OE_CORE_TRACE(...) ::oe::Logger::Get()->GetEngineLogger()->trace("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_INFO(...) ::oe::Logger::Get()->GetEngineLogger()->info("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_WARN(...) ::oe::Logger::Get()->GetEngineLogger()->warn("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_ERROR(...) ::oe::Logger::Get()->GetEngineLogger()->error("[ENGINE] " + fmt::format(__VA_ARGS__))
#define OE_CORE_CRITICAL(...) ::oe::Logger::Get()->GetEngineLogger()->critical("[ENGINE] " + fmt::format(__VA_ARGS__))

#define OE_TRACE(...) ::oe::Logger::Get()->GetClientLogger()->trace("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_INFO(...) ::oe::Logger::Get()->GetClientLogger()->info("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_WARN(...) ::oe::Logger::Get()->GetClientLogger()->warn("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_ERROR(...) ::oe::Logger::Get()->GetClientLogger()->error("[CLIENT] " + fmt::format(__VA_ARGS__))
#define OE_CRITICAL(...) ::oe::Logger::Get()->GetClientLogger()->critical(fmt::format(__VA_ARGS__))

DEFINE_FORMATTER(glm::vec2, "{}", glm::to_string(value))
DEFINE_FORMATTER(glm::vec3, "{}", glm::to_string(value))
DEFINE_FORMATTER(glm::vec4, "{}", glm::to_string(value))
DEFINE_FORMATTER(glm::mat4, "{}", glm::to_string(value))

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
constexpr OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
constexpr OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
constexpr OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}
