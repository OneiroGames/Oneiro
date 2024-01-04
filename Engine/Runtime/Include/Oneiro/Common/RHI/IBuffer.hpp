//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/RHI/RHI_Defines.hpp"

namespace oe::RHI
{
	class IBuffer
	{
	public:
		IBuffer() = default;
		IBuffer(IBuffer&& old) noexcept = default;
		IBuffer& operator=(IBuffer&& old) = default;
		IBuffer(const IBuffer&) = delete;
		IBuffer& operator=(const IBuffer&) = delete;
		~IBuffer() = default;

		virtual void Create(const void* data, size_t size, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "") = 0;

		virtual void UpdateData(const void* data, size_t size, size_t destOffsetBytes = 0) = 0;

		virtual void FillData(const BufferFillInfo& clear = {}) = 0;

		/// @brief Gets a pointer that is mapped to the buffer's data store
		/// @return A pointer to mapped memory if the buffer was created with BufferStorageFlag::MAP_MEMORY, otherwise nullptr
		[[nodiscard]] void* GetMappedPointer() noexcept;

		[[nodiscard]] const void* GetMappedPointer() const noexcept;

		[[nodiscard]] size_t Size() const noexcept;

		[[nodiscard]] bool IsMapped() const noexcept;

		/// @brief Invalidates the content of the buffer's data store
		///
		/// This call can be used to optimize driver synchronization in certain cases.
		virtual void Invalidate() = 0;

	protected:
		size_t m_Size{};
		uint32_t m_BufferStorageFlags{};
		void* m_MappedMemory{};
	};
} // namespace oe::RHI