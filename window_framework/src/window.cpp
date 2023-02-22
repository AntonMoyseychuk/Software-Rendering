#include "window.hpp"
#include "math_3d/color.hpp"
#include <iostream>

#define LOG(tag, msg) std::cerr << '[' << (tag) << "] " << (msg) << '\n' \
    << __FILE__ << " : " << __LINE__ << "\n\n"

#define LOG_ERROR(condition, tag, msg) if (!(condition)) { \
    LOG((tag), (msg)); \
    exit(-1); \
}

#define MAP_RGBA(format, color) SDL_MapRGBA(format, color.r, color.g, color.b, color.a)

namespace window_framework {
    std::unique_ptr<bool, Window::SDLDeinitializer> Window::is_sdl_initialized = false;

    Window* Window::Get() noexcept {
        LOG("log info", "Window::Get()");

        if (is_sdl_initialized == false) {
            is_sdl_initialized.reset(new bool(InitializeSDL()));
        }

        LOG_ERROR(*is_sdl_initialized, "sdl error", SDL_GetError());
        
        static Window window;
        return &window;
    }

    Window::Window(Window &&window)
        : m_title(std::move(window.m_title)), m_width(window.m_width), m_height(window.m_height), m_is_quit(window.m_is_quit),
            m_window_ptr(std::move(window.m_window_ptr)), m_surface(window.m_surface), m_event(window.m_event)
    {
        LOG("log info", "Window::Window(Window &&window)");

        window.m_surface = nullptr;
        memset(&window.m_event, 0, sizeof(window.m_event));
    }

    Window &Window::operator=(Window &&window) noexcept {
        LOG("log info", "Window::operator=(Window &&window)");

        m_title = std::move(window.m_title);
        m_width = window.m_width;
        m_height = window.m_height;
        m_is_quit = window.m_is_quit;

        m_window_ptr = std::move(window.m_window_ptr);
        m_surface = window.m_surface;
        m_event = window.m_event;

        window.m_surface = nullptr;
        memset(&window.m_event, 0, sizeof(window.m_event));

        return *this;
    }

    bool Window::InitializeSDL() {
        LOG("log info", "Window::InitializeSDL()");
        return SDL_Init(SDL_INIT_EVERYTHING) == 0;
    }

    bool Window::UpdateSurface() const noexcept {
        m_surface = SDL_GetWindowSurface(m_window_ptr.get());
        return m_surface != nullptr;
    }

    bool Window::Init(const std::string_view title, std::uint32_t width, std::uint32_t height) {
        LOG("log info", "Window::Init(const std::string_view title, std::uint32_t width, std::uint32_t height)");

        if (m_window_ptr != nullptr) {
            LOG("log info", "window is already exists. Don't use Init() method");
            return false;
        }

        m_width = width;
        m_height = height;

        m_window_ptr.reset(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0));
        LOG_ERROR(m_window_ptr != nullptr, "sdl error",  SDL_GetError());
        SDL_SetWindowResizable(m_window_ptr.get(), SDL_TRUE);

        LOG_ERROR(UpdateSurface() == true, "sdl error",  SDL_GetError());

        return true;
    }

    bool Window::IsOpen() const noexcept {
        #ifdef LOG_ALL
            LOG("log info", "Window::IsOpen()");
        #endif
        return !m_is_quit;
    }

    void Window::FillPixelBuffer(const std::vector<math::Color> &pixels) const noexcept {
        #ifdef LOG_ALL
            LOG("log info", "Window::FillPixelBuffer(const std::vector<math::Color> &pixels)");
        #endif

        auto pixel_buffer = static_cast<std::uint32_t*>(m_surface->pixels);
        for (std::size_t y = 0; y < m_height; ++y) {
            for (std::size_t x = 0; x < m_width; ++x) {
                pixel_buffer[x + y * m_width] = MAP_RGBA(m_surface->format, pixels[x + y * m_width]);
            }
        }
    }

    void Window::PresentPixelBuffer() const noexcept {
        #ifdef LOG_ALL
            LOG("log info", "Window::PresentPixelBuffer()");
        #endif

        SDL_UpdateWindowSurface(m_window_ptr.get());
    }

    void Window::PollEvent() noexcept {
        #ifdef LOG_ALL
            LOG("log info", "Window::PollEvent()");
        #endif

        while (SDL_PollEvent(&m_event)) {
            switch (m_event.type)  {
            case SDL_QUIT:
                m_is_quit = true;
                break;
            case SDL_WINDOWEVENT:
                if(m_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    LOG("log info", "SDL_WINDOWEVENT_SIZE_CHANGED");
                    SDL_GetWindowSize(m_window_ptr.get(), (int*)&m_width, (int*)&m_height);
                    LOG_ERROR(UpdateSurface(), "sdl error", SDL_GetError());
                }
            }
        }
    }

    void Window::SetTitle(const std::string_view title) noexcept {
        LOG("log info", "Window::SetTitle(const std::string_view title)");

        m_title = title;
        SDL_SetWindowTitle(m_window_ptr.get(), m_title.c_str());
    }

    const std::string_view Window::GetTitle() const noexcept {
        LOG("log info", "Window::GetTitle()");
        return m_title;
    }

    void Window::SetWidth(std::uint32_t width) noexcept {
        LOG("log info", "Window::SetWidth(std::uint32_t width)");
        m_width = width;
        SDL_SetWindowSize(m_window_ptr.get(), m_width, m_height);

        LOG_ERROR(UpdateSurface() == true, "sdl error",  SDL_GetError());
    }

    std::uint32_t Window::GetWidth() const noexcept {
        LOG("log info", "Window::GetWidth()");
        return m_width;
    }

    void Window::SetHeight(std::uint32_t height) noexcept {
        LOG("log info", "Window::SetHeight(std::uint32_t height)");
        m_height = height;
        SDL_SetWindowSize(m_window_ptr.get(), m_width, m_height);
        
        LOG_ERROR(UpdateSurface() == true, "sdl error",  SDL_GetError());
    }

    std::uint32_t Window::GetHeight() const noexcept {
        LOG("log info", "Window::SetHeight(std::uint32_t height)");
        return m_height;
    }

    const SDL_Surface* Window::GetSDLSurface() const noexcept {
        #ifdef LOG_ALL
            LOG("log info", "Window::GetSDLSurface() const");
        #endif
        return m_surface;
    }

    SDL_Surface* Window::GetSDLSurface() noexcept {
        #ifdef LOG_ALL
            LOG("log info", "Window::GetSDLSurface()");
        #endif
        return m_surface;
    }
    
    void Window::SDLDeinitializer::operator()(bool *is_sdl_initialized) const {
        if (is_sdl_initialized) {
            LOG("log info", "SDLDeinitializer::SDL_Quit()");
            SDL_Quit();
            *is_sdl_initialized = false;
        }
    }
    
    void Window::WindowDeleter::operator()(SDL_Window *window) const {
        if (window != nullptr) {
            LOG("log info", "SDLDeinitializer::SDL_DestroyWindow(window)");
            SDL_DestroyWindow(window);
            window = nullptr;
        }
    }
}