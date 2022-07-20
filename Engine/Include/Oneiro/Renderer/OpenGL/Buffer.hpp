//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <iostream>

#include "Base.hpp"

namespace oe::Renderer::GL
{
    template <int BufferType, int UsageType> class Buffer
    {
      public:
        Buffer() = default;
        Buffer(const Buffer&) = delete;

        ~Buffer()
        {
            if (mID)
                gl::DeleteBuffers(1, &mID);
        }

        constexpr void Generate()
        {
            gl::GenBuffers(1, &mID);
        }

        constexpr static void BufferData(int64_t size, const void* data)
        {
            gl::BufferData(BufferType, size, data, UsageType);
        }

        template <class T> constexpr static void BufferData(int64_t size, const void* data)
        {
            gl::BufferData(BufferType, size * sizeof(T), data, UsageType);
        }

        constexpr static void BufferSubData(int64_t size, ptrdiff_t offset, const void* data)
        {
            gl::BufferSubData(BufferType, offset, size, data);
        }

        constexpr void Bind() const
        {
            gl::BindBuffer(BufferType, mID);
        }

        static constexpr void UnBind()
        {
            gl::BindBuffer(BufferType, 0);
        }

        [[nodiscard]] uint32_t Get() const
        {
            return mID;
        }

      private:
        uint32_t mID{};
    };

    // ReSharper disable CppCStyleCast

    void VertexAttribPointer(int index, int size, int stride, int offset = 0);

    // ReSharper restore CppCStyleCast
} // namespace oe::Renderer::GL
