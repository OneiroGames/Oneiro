//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Buffer.hpp"

namespace oe::Renderer::GL
{
    void VertexAttribPointer(int index, int size, int stride, int offset)
    {
        gl::EnableVertexAttribArray(index);
        gl::VertexAttribPointer(index, size, gl::FLOAT, gl::FALSE_, static_cast<int>(stride * sizeof(float)),
                                offset == 0 ? nullptr : (void*)(offset * sizeof(float)));
    }
} // namespace oe::Renderer::GL