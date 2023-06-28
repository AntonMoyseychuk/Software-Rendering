#include "gl_api.hpp"


namespace gl {
    gl_api &gl_api::get() noexcept {
        static gl_api engine;
        return engine;
    }

    gl_api::gl_api() noexcept
    {
    }
}