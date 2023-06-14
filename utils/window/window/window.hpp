#pragma once
#include "thread_pool/thread_pool.hpp"

#include <SDL.h>

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>


namespace win_framewrk {
    enum class Key { 
        NUMBER_0 = SDL_Scancode::SDL_SCANCODE_0,
        NUMBER_1 = SDL_Scancode::SDL_SCANCODE_1,
        NUMBER_2 = SDL_Scancode::SDL_SCANCODE_2,
        NUMBER_3 = SDL_Scancode::SDL_SCANCODE_3,
        NUMBER_4 = SDL_Scancode::SDL_SCANCODE_4,
        NUMBER_5 = SDL_Scancode::SDL_SCANCODE_5,
        NUMBER_6 = SDL_Scancode::SDL_SCANCODE_6,
        NUMBER_7 = SDL_Scancode::SDL_SCANCODE_7,
        NUMBER_8 = SDL_Scancode::SDL_SCANCODE_8,
        NUMBER_9 = SDL_Scancode::SDL_SCANCODE_9,

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
        union _InternalColor {
            _InternalColor() = default;
            _InternalColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
                : r(_r), g(_g), b(_b), a(_a) {}

            explicit _InternalColor(uint32_t _rgba)
                : rgba(_rgba) {}

            struct {
                uint8_t r, g, b, a;
            };

            uint32_t rgba;
        };

    public:
        static Window* Get() noexcept;
        bool Init(const std::string& title, uint32_t width, uint32_t height);
        
        bool IsOpen() const noexcept;
        void FillPixelBuffer(const std::vector<uint32_t>& pixels) const noexcept;
        void FillPixelBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const noexcept;
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

        uint32_t GetPixelColor(size_t x, size_t y) noexcept;
        
        void SetPixelColor(size_t x, size_t y, uint32_t color) noexcept;
        void SetPixelColor(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;

        void SetWidth(uint32_t width) noexcept;
        uint32_t GetWidth() const noexcept;

        void SetHeight(uint32_t height) noexcept;
        uint32_t GetHeight() const noexcept;

        const SDL_Surface* GetSDLSurfaceHandle() const noexcept;
        SDL_Surface* GetSDLSurfaceHandle() noexcept;

        const SDL_Window* GetSDLWindowHandle() const noexcept;
        SDL_Window* GetSDLWindowHandle() noexcept;

        void SetResizeCallback(const std::function<void(uint32_t width, uint32_t height)>& callback) const noexcept;
    #pragma endregion getters-setters

    private:
        static uint32_t _MapRGBA(SDL_PixelFormat* format, _InternalColor color) noexcept;
        static bool _InitializeSDL();

    private:
        Window() = default;
        
        bool _UpdateSurface() const noexcept;

    private:
        void _OnResize(uint32_t new_width, uint32_t new_height) noexcept;
        void _OnQuit() noexcept;

    private:
        static void _ThreadBufferFillingFunc(uint32_t x0, uint32_t y0, uint32_t x_end, uint32_t y_end, 
            SDL_Surface* surface, const uint32_t* in_pixels) noexcept;

    private:
        struct SDLDeinitializer {
            void operator()(bool* is_initialized) const;
        };

        struct WindowDestroyer {
            void operator()(SDL_Window* window) const;
        };

    private:
        static std::unique_ptr<bool, SDLDeinitializer> is_sdl_initialized_ptr;

    private:
        std::unique_ptr<SDL_Window, WindowDestroyer> m_window_ptr = nullptr;
        mutable SDL_Surface* m_surface_ptr = nullptr;
        SDL_Event m_event;

        std::string m_title = "";
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        bool m_is_quit = false;

        mutable std::function<void(uint32_t width, uint32_t height)> _ResizeCallback;
        
        mutable util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };
    };
}