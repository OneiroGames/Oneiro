#pragma once
#include <Fwog/Config.h>
#include <Fwog/BasicTypes.h>
#include <Fwog/detail/Flags.h>
#include <string_view>
#include <span>
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
    explicit Buffer(const void* data, size_t size, BufferStorageFlags storageFlags = BufferStorageFlag::NONE, std::string_view name = "");
    
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
} // namespace Fwog
