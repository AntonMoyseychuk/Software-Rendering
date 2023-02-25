#pragma once
#include "math_3d/declarations.hpp"

#include "window_framework/logger.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <stdint.h>

namespace window_framework {
    class Window {
    public:
        static Window* Get() noexcept;
        bool Init(const std::string_view title, std::uint32_t width, std::uint32_t height);
        
        bool IsOpen() const noexcept;
        void FillPixelBuffer(const std::vector<math::Color>& pixels) const noexcept;
        void PresentPixelBuffer() const noexcept;
        void PollEvent() noexcept;

        math::Color GetPixelColor(std::size_t x, std::size_t y) noexcept;

        void SetTitle(const std::string_view title) noexcept;
        const std::string_view GetTitle() const noexcept;

        void SetWidth(std::uint32_t width) noexcept;
        std::uint32_t GetWidth() const noexcept;

        void SetHeight(std::uint32_t height) noexcept;
        std::uint32_t GetHeight() const noexcept;

        const SDL_Surface* GetSDLSurface() const noexcept;
        SDL_Surface* GetSDLSurface() noexcept;

        Window(Window&& window);
        Window& operator=(Window&& window) noexcept;

        Window(const Window& window) = delete;
        Window& operator=(const Window& window) = delete;

    private:
        Window() = default;
        static bool InitializeSDL();
        bool UpdateSurface() const noexcept;

    private:
        struct SDLDeinitializer {
            void operator()(bool* is_initialized) const;
        };

        struct WindowDeleter {
            void operator()(SDL_Window* window) const;
        };

    private:
        static std::unique_ptr<bool, SDLDeinitializer> is_sdl_initialized_ptr;

    private:
        std::string m_title;
        std::uint32_t m_width, m_height;
        bool m_is_quit = false;

        std::unique_ptr<SDL_Window, WindowDeleter> m_window_ptr = nullptr;
        mutable SDL_Surface* m_surface_ptr = nullptr;
        SDL_Event m_event;
    };
}
