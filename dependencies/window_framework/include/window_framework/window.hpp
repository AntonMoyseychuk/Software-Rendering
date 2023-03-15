#pragma once
#include "logger.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>
#include <stdint.h>


namespace win_framewrk {
    class Window {
    public:
        static Window* Get() noexcept;
        bool Init(const std::string& title, std::uint32_t width, std::uint32_t height);
        
        bool IsOpen() const noexcept;
        void FillPixelBuffer(const std::vector<std::uint32_t>& pixels) const noexcept;
        void PresentPixelBuffer() const noexcept;
        void PollEvent() noexcept;

        std::uint32_t GetPixelColor(std::size_t x, std::size_t y) noexcept;
        void SetPixelColor(std::size_t x, std::size_t y, std::uint32_t color) noexcept;

        void SetTitle(const std::string_view title) noexcept;
        const std::string& GetTitle() const noexcept;

        void SetBackgroundColor(std::uint32_t color) noexcept;
        std::uint32_t GetBackgroundColor() const noexcept;

        void SetWidth(std::uint32_t width) noexcept;
        std::uint32_t GetWidth() const noexcept;

        void SetHeight(std::uint32_t height) noexcept;
        std::uint32_t GetHeight() const noexcept;

        const SDL_Surface* GetSDLSurfaceHandle() const noexcept;
        SDL_Surface* GetSDLSurfaceHandle() noexcept;

        Window(Window&& window);
        Window& operator=(Window&& window) noexcept;

        Window(const Window& window) = delete;
        Window& operator=(const Window& window) = delete;
        
    private:
        static std::uint32_t _ConvertToBigEndian(std::uint32_t color) noexcept;
        static std::uint32_t _MapRGBA(SDL_PixelFormat* format, std::uint32_t color) noexcept;

    private:
        Window() = default;
        static bool _InitializeSDL();
        bool _UpdateSurface() const noexcept;

        void _OnWindowEvent() noexcept;
        void _OnQuitEvent() noexcept;

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

        std::uint32_t m_background_color = 0;
    };
}
