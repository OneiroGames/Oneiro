//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Renderer.hpp"

#include <any>

namespace oe::Renderer
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
		[[nodiscard]] void* GetMappedPointer() noexcept
		{
			return m_MappedMemory;
		}

		[[nodiscard]] const void* GetMappedPointer() const noexcept
		{
			return m_MappedMemory;
		}

		[[nodiscard]] size_t Size() const noexcept
		{
			return m_Size;
		}

		[[nodiscard]] bool IsMapped() const noexcept
		{
			return m_MappedMemory != nullptr;
		}

		/// @brief Invalidates the content of the buffer's data store
		///
		/// This call can be used to optimize driver synchronization in certain cases.
		virtual void Invalidate() = 0;

	protected:
		size_t m_Size{};
		uint32_t m_BufferStorageFlags{};
		void* m_MappedMemory{};
	};

	/// @brief A buffer that provides type-safe operations
	/// @tparam T A trivially copyable type
	template <class T>
	requires(std::is_trivially_copyable_v<T>)
	class TypedBuffer : public IBuffer
	{
	public:
		explicit TypedBuffer(BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "")
		{
			Create(sizeof(T), storageFlags, name);
		}
		explicit TypedBuffer(size_t count, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "")
		{
			Create(sizeof(T) * count, storageFlags, name);
		}
		explicit TypedBuffer(std::span<const T> data, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "")
		{
			Create(data, storageFlags, name);
		}
		explicit TypedBuffer(const T& data, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "")
		{
			Create(&data, sizeof(T), storageFlags, name);
		}

		TypedBuffer(TypedBuffer&& other) noexcept = default;
		TypedBuffer& operator=(TypedBuffer&& other) noexcept = default;
		TypedBuffer(const TypedBuffer& other) = delete;
		TypedBuffer& operator=(const TypedBuffer&) = delete;

		void UpdateData(const T& data, size_t startIndex = 0)
		{
			IBuffer::UpdateData(data, sizeof(T) * startIndex);
		}

		void UpdateData(std::span<const T> data, size_t startIndex = 0)
		{
			IBuffer::UpdateData(data, sizeof(T) * startIndex);
		}

		[[nodiscard]] T* GetMappedPointer() noexcept
		{
			return static_cast<T*>(m_MappedMemory);
		}

		[[nodiscard]] const T* GetMappedPointer() const noexcept
		{
			return static_cast<T*>(m_MappedMemory);
		}

	private:
	};
} // namespace oe::Renderer