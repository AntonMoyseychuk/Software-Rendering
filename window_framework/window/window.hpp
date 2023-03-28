#pragma once
#include <SDL2/SDL.h>
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
        
        UP_ARROW = SDL_Scancode::SDL_SCANCODE_UP,
        RIGHT_ARROW = SDL_Scancode::SDL_SCANCODE_RIGHT,
        LEFT_ARROW = SDL_Scancode::SDL_SCANCODE_LEFT,
        DOWN_ARROW = SDL_Scancode::SDL_SCANCODE_DOWN,

        SPASE = SDL_Scancode::SDL_SCANCODE_SPACE,
        ENTER = SDL_Scancode::SDL_SCANCODE_KP_ENTER,
        TAB = SDL_Scancode::SDL_SCANCODE_TAB,
        
        LSHIFT = SDL_Scancode::SDL_SCANCODE_LSHIFT,
        RSHIFT = SDL_Scancode::SDL_SCANCODE_RSHIFT,
        LCTRL = SDL_Scancode::SDL_SCANCODE_LCTRL,
        RCTRL = SDL_Scancode::SDL_SCANCODE_RCTRL,
        LALT = SDL_Scancode::SDL_SCANCODE_LALT,
        RALT = SDL_Scancode::SDL_SCANCODE_RALT,
    };

    class Window {
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

        std::uint32_t GetPixelColor(std::size_t x, std::size_t y) noexcept;
        void SetPixelColor(std::size_t x, std::size_t y, std::uint32_t color) noexcept;

       
        void SetBackgroundColor(std::uint32_t color) noexcept;
        std::uint32_t GetBackgroundColor() const noexcept;

        void SetWidth(std::uint32_t width) noexcept;
        std::uint32_t GetWidth() const noexcept;

        void SetHeight(std::uint32_t height) noexcept;
        std::uint32_t GetHeight() const noexcept;

        const SDL_Surface* GetSDLSurfaceHandle() const noexcept;
        SDL_Surface* GetSDLSurfaceHandle() noexcept;
    #pragma endregion getters-setters

    private:
        static std::uint32_t _ConvertToBigEndian(std::uint32_t color) noexcept;
        static std::uint32_t _MapRGBA(SDL_PixelFormat* format, std::uint32_t color) noexcept;
        static bool _InitializeSDL();

    private:
        Window() = default;
        
        bool _UpdateSurface() const noexcept;
        void _UpdateVerticalIterator(std::uint32_t new_height) noexcept;

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
        std::unique_ptr<SDL_Window, WindowDeleter> m_window_ptr = nullptr;
        mutable SDL_Surface* m_surface_ptr = nullptr;
        SDL_Event m_event;

        std::string m_title;
        std::uint32_t m_width;
        std::uint32_t m_height;
        bool m_is_quit = false;

        std::uint32_t m_background_color = 0;
        
        std::vector<std::uint32_t> m_vertical_it;
    };
}
