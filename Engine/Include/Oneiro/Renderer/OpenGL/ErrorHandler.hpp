//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Core/Logger.hpp"

namespace oe::Renderer::GL::ErrorHandler
{
    void GLClearError()
    {
        while (gl::GetError() != gl::NO_ERROR_);
    }

    bool GLLogCall()
    {
        while (GLenum error = gl::GetError())
        {
            log::get("log")->error(&std::string("[OpenGL]: ") [error]);
            return false;
        }
        return true;
    }
}
