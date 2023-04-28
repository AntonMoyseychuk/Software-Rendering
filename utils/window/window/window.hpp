#pragma once
#include "thread_pool/thread_pool.hpp"

#include <SDL.h>

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>


namespace win_framewrk {
    enum class Key { 
        W = SDL_Scancode::SDL_SCANCODE_W,
        A = SDL_Scancode::SDL_SCANCODE_A,
        D = SDL_Scancode::SDL_SCANCODE_D,
        S = SDL_Scancode::SDL_SCANCODE_S,
        Z = SDL_Scancode::SDL_SCANCODE_Z,
        X = SDL_Scancode::SDL_SCANCODE_X,
        
        UP_ARROW = SDL_Scancode::SDL_SCANCODE_UP,
        RIGHT_ARROW = SDL_Scancode::SDL_SCANCODE_RIGHT,
        LEFT_ARROW = SDL_Scancode::SDL_SCANCODE_LEFT,
        DOWN_ARROW = SDL_Scancode::SDL_SCANCODE_DOWN,

        SPASE = SDL_Scancode::SDL_SCANCODE_SPACE,
        TAB = SDL_Scancode::SDL_SCANCODE_TAB,
        
        LSHIFT = SDL_Scancode::SDL_SCANCODE_LSHIFT,
        RSHIFT = SDL_Scancode::SDL_SCANCODE_RSHIFT,
        LCTRL = SDL_Scancode::SDL_SCANCODE_LCTRL,
        RCTRL = SDL_Scancode::SDL_SCANCODE_RCTRL,
        LALT = SDL_Scancode::SDL_SCANCODE_LALT,
        RALT = SDL_Scancode::SDL_SCANCODE_RALT,
    };

    class Window {
    private:
        union Color {
            Color() = default;

            Color(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _a)
                : r(_r), g(_g), b(_b), a(_a) {}

            Color(std::uint32_t _rgba)
                : rgba(_rgba) {}
            struct {
                std::uint8_t r, g, b, a;
            };

            std::uint32_t rgba;
        };

    public:
        static Window* Get() noexcept;
        bool Init(const std::string& title, std::uint32_t width, std::uint32_t height);
        
        bool IsOpen() const noexcept;
        void FillPixelBuffer(const std::vector<std::uint32_t>& pixels) const noexcept;
        void PresentPixelBuffer() const noexcept;
        void PollEvent() noexcept;

        bool IsKeyPressed(Key key) const noexcept;

        Window(Window&& window);
        Window& operator=(Window&& window) noexcept;

        Window(const Window& window) = delete;
        Window& operator=(const Window& window) = delete;
        
    #pragma region getters-setters
        void SetTitle(const std::string_view title) noexcept;
        const std::string& GetTitle() const noexcept;

        void SetWidth(std::uint32_t width) noexcept;
        std::uint32_t GetWidth() const noexcept;

        void SetHeight(std::uint32_t height) noexcept;
        std::uint32_t GetHeight() const noexcept;

        const SDL_Surface* GetSDLSurfaceHandle() const noexcept;
        SDL_Surface* GetSDLSurfaceHandle() noexcept;
    #pragma endregion getters-setters

    private:
        static bool _InitializeSDL();

    private:
        Window() = default;
        
        bool _UpdateSurface() const noexcept;

    private:
        void _OnResize(std::uint32_t new_width, std::uint32_t new_height) noexcept;
        void _OnQuit() noexcept;

    private:
        void _ThreadBufferFillingFunc(
            std::uint32_t x0, std::uint32_t y0, 
            std::uint32_t x_end, std::uint32_t y_end,
            std::uint32_t* dist, const std::uint32_t* src
        ) const noexcept;

    private:
        struct SDLDeinitializer {
            void operator()(bool* is_initialized) const;
        };

        struct WindowDestroyer {
            void operator()(SDL_Window* window) const;
        };

        struct RendererDestroyer {
            void operator()(SDL_Renderer* renderer) const;
        };

        struct TextureDestroyer {
            void operator()(SDL_Texture* texture) const;
        };

    private:
        static std::unique_ptr<bool, SDLDeinitializer> is_sdl_initialized_ptr;

    private:
        std::unique_ptr<SDL_Window, WindowDestroyer> m_window_ptr = nullptr;
        std::unique_ptr<SDL_Renderer, RendererDestroyer> m_renderer_ptr = nullptr;
        std::unique_ptr<SDL_Texture, TextureDestroyer> m_texture_ptr = nullptr;

        SDL_PixelFormat* m_pixel_format_ptr = nullptr;

        SDL_Event m_event;

        std::string m_title = "";
        std::uint32_t m_width = 0;
        std::uint32_t m_height = 0;
        bool m_is_quit = false;

        Color m_background_color = 0;
        
        mutable util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };
    };
}
