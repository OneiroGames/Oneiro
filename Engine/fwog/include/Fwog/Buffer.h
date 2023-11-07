#pragma once
#include <Fwog/BasicTypes.h>
#include <Fwog/Config.h>
#include <Fwog/detail/Flags.h>
#include <span>
#include <string_view>
#include <type_traits>

namespace Fwog
{
  /// @brief Parameters for Buffer::FillData
  struct BufferFillInfo
  {
    uint64_t offset = 0;
    uint64_t size = WHOLE_BUFFER;
    uint32_t data = 0;
  };

  enum class BufferStorageFlag : uint32_t
  {
    NONE = 0,

    /// @brief Allows the user to update the buffer's contents with Buffer::UpdateData
    DYNAMIC_STORAGE = 1 << 0,

    /// @brief Hints to the implementation to place the buffer storage in host memory
    CLIENT_STORAGE = 1 << 1,

    /// @brief Maps the buffer (persistently and coherently) upon creation
    MAP_MEMORY = 1 << 2,
  };
  FWOG_DECLARE_FLAG_TYPE(BufferStorageFlags, BufferStorageFlag, uint32_t)

  /// @brief Encapsulates an OpenGL buffer
  class Buffer
  {
  public:
    explicit Buffer(size_t size, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "");
    explicit Buffer(const void* data,
                    size_t size,
                    BufferStorageFlags storageFlags = BufferStorageFlag::NONE,
                    std::string_view name = "");

    Buffer(Buffer&& old) noexcept;
    Buffer& operator=(Buffer&& old) noexcept;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    ~Buffer();

    void UpdateData(const void* data, size_t size, size_t destOffsetBytes = 0);

    void FillData(const BufferFillInfo& clear = {});

    /// @brief Gets a pointer that is mapped to the buffer's data store
    /// @return A pointer to mapped memory if the buffer was created with BufferStorageFlag::MAP_MEMORY, otherwise nullptr
    [[nodiscard]] void* GetMappedPointer() noexcept
    {
      return mappedMemory_;
    }

    [[nodiscard]] const void* GetMappedPointer() const noexcept
    {
      return mappedMemory_;
    }

    [[nodiscard]] auto Handle() const noexcept
    {
      return id_;
    }

    [[nodiscard]] auto Size() const noexcept
    {
      return size_;
    }

    [[nodiscard]] bool IsMapped() const noexcept
    {
      return mappedMemory_ != nullptr;
    }

    /// @brief Invalidates the content of the buffer's data store
    ///
    /// This call can be used to optimize driver synchronization in certain cases.
    void Invalidate();

  protected:
    size_t size_{};
    BufferStorageFlags storageFlags_{};
    uint32_t id_{};
    void* mappedMemory_{};
  };

  /// @brief A buffer that provides type-safe operations
  /// @tparam T A trivially copyable type
  template<class T>
    requires(std::is_trivially_copyable_v<T>)
  class TypedBuffer : public Buffer
  {
  public:
    explicit TypedBuffer(BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "")
      : Buffer(sizeof(T), storageFlags, name)
    {
    }
    explicit TypedBuffer(size_t count, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "")
      : Buffer(sizeof(T) * count, storageFlags, name)
    {
    }
    explicit TypedBuffer(std::span<const T> data,
                         BufferStorageFlags storageFlags = BufferStorageFlag::NONE,
                         std::string_view name = "")
      : Buffer(data, storageFlags, name)
    {
    }
    explicit TypedBuffer(const T& data, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "")
      : Buffer(&data, sizeof(T), storageFlags, name)
    {
    }

    TypedBuffer(TypedBuffer&& other) noexcept = default;
    TypedBuffer& operator=(TypedBuffer&& other) noexcept = default;
    TypedBuffer(const TypedBuffer& other) = delete;
    TypedBuffer& operator=(const TypedBuffer&) = delete;

    void UpdateData(const T& data, size_t startIndex = 0)
    {
      Buffer::UpdateData(data, sizeof(T) * startIndex);
    }

    void UpdateData(std::span<const T> data, size_t startIndex = 0)
    {
      Buffer::UpdateData(data, sizeof(T) * startIndex);
    }

    [[nodiscard]] T* GetMappedPointer() noexcept
    {
      return static_cast<T*>(mappedMemory_);
    }

    [[nodiscard]] const T* GetMappedPointer() const noexcept
    {
      return static_cast<T*>(mappedMemory_);
    }

  private:
  };
} // namespace Fwog
