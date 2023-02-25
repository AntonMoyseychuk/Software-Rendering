#include "window_framework/window.hpp"
#include "math_3d/color.hpp"

#define MAP_RGBA(format, color) SDL_MapRGBA(format, color.r, color.g, color.b, color.a)
#define LOG_SDL_ERROR(condition, msg) LOG_WIN_ERROR((condition), "sdl error", msg)

namespace window_framework {
    std::unique_ptr<bool, Window::SDLDeinitializer> Window::is_sdl_initialized_ptr = nullptr;

    Window* Window::Get() noexcept {
        LOG_WIN_INFO(__FUNCTION__);

        if (is_sdl_initialized_ptr == nullptr || *is_sdl_initialized_ptr == false) {
            is_sdl_initialized_ptr.reset(new bool(InitializeSDL()));
        }

        LOG_SDL_ERROR(*is_sdl_initialized_ptr, SDL_GetError());
        
        static Window window;
        return &window;
    }

    Window::Window(Window &&window)
        : m_title(std::move(window.m_title)), m_width(window.m_width), m_height(window.m_height), m_is_quit(window.m_is_quit),
            m_window_ptr(std::move(window.m_window_ptr)), m_surface_ptr(window.m_surface_ptr), m_event(window.m_event)
    {
        LOG_WIN_INFO(__FUNCTION__);

        window.m_surface_ptr = nullptr;
        memset(&window.m_event, 0, sizeof(window.m_event));
    }

    Window &Window::operator=(Window &&window) noexcept {
        LOG_WIN_INFO(__FUNCTION__);

        m_title = std::move(window.m_title);
        m_width = window.m_width;
        m_height = window.m_height;
        m_is_quit = window.m_is_quit;

        m_window_ptr = std::move(window.m_window_ptr);
        m_surface_ptr = window.m_surface_ptr;
        m_event = window.m_event;

        window.m_surface_ptr = nullptr;
        memset(&window.m_event, 0, sizeof(window.m_event));

        return *this;
    }

    bool Window::InitializeSDL() {
        LOG_WIN_INFO(__FUNCTION__);
        return SDL_Init(SDL_INIT_EVERYTHING) == 0;
    }

    bool Window::UpdateSurface() const noexcept {
        m_surface_ptr = SDL_GetWindowSurface(m_window_ptr.get());
        return m_surface_ptr != nullptr;
    }

    bool Window::Init(const std::string_view title, std::uint32_t width, std::uint32_t height) {
        LOG_WIN_INFO(__FUNCTION__);

        if (m_window_ptr != nullptr) {
            LOG_WIN_INFO("There was an atempt to secondory initialize window");
            return false;
        }

        m_width = width;
        m_height = height;

        m_window_ptr.reset(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0));
        LOG_SDL_ERROR(m_window_ptr != nullptr, SDL_GetError());
        SDL_SetWindowResizable(m_window_ptr.get(), SDL_TRUE);

        LOG_SDL_ERROR(UpdateSurface() == true, SDL_GetError());

        return true;
    }

    bool Window::IsOpen() const noexcept {
        #ifdef LOG_ALL
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        return !m_is_quit;
    }

    void Window::FillPixelBuffer(const std::vector<math::Color> &pixels) const noexcept {
        #ifdef LOG_ALL
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        auto pixel_buffer = static_cast<std::uint32_t*>(m_surface_ptr->pixels);
        for (std::size_t y = 0; y < m_height; ++y) {
            for (std::size_t x = 0; x < m_width; ++x) {
                pixel_buffer[x + y * m_width] = MAP_RGBA(m_surface_ptr->format, pixels[x + y * m_width]);
            }
        }
    }

    void Window::PresentPixelBuffer() const noexcept {
        #ifdef LOG_ALL
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        LOG_SDL_ERROR(SDL_UpdateWindowSurface(m_window_ptr.get()) == 0, SDL_GetError());
    }

    void Window::PollEvent() noexcept {
        #ifdef LOG_ALL
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        while (SDL_PollEvent(&m_event)) {
            switch (m_event.type)  {
            case SDL_QUIT:
                m_is_quit = true;
                break;
            case SDL_WINDOWEVENT:
                if(m_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    LOG("event", "SDL_WINDOWEVENT_SIZE_CHANGED");
                    SDL_GetWindowSize(m_window_ptr.get(), (int*)&m_width, (int*)&m_height);
                    LOG_SDL_ERROR(UpdateSurface(), SDL_GetError());
                }
            }
        }
    }

    math::Color Window::GetPixelColor(std::size_t x, std::size_t y) noexcept {
        #ifdef LOG_ALL
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        
        SDL_assert(x < static_cast<std::size_t>(m_surface_ptr->w) && y < static_cast<std::size_t>(m_surface_ptr->h));

        std::uint8_t r, g, b, a;
        auto pixels = (std::uint32_t*)m_surface_ptr->pixels;
        SDL_GetRGBA(pixels[y * m_surface_ptr->w + x], m_surface_ptr->format, &r, &g, &b, &a);

        return math::Color(r, g, b, a);
    }

    void Window::SetTitle(const std::string_view title) noexcept  {
        LOG_WIN_INFO(__FUNCTION__);

        m_title = title;
        SDL_SetWindowTitle(m_window_ptr.get(), m_title.c_str());
    }

    const std::string_view Window::GetTitle() const noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        return m_title;
    }

    void Window::SetWidth(std::uint32_t width) noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        m_width = width;
        SDL_SetWindowSize(m_window_ptr.get(), m_width, m_height);

        LOG_SDL_ERROR(UpdateSurface() == true, SDL_GetError());
    }

    std::uint32_t Window::GetWidth() const noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        return m_width;
    }

    void Window::SetHeight(std::uint32_t height) noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        m_height = height;
        SDL_SetWindowSize(m_window_ptr.get(), m_width, m_height);
        
        LOG_SDL_ERROR(UpdateSurface() == true, SDL_GetError());
    }

    std::uint32_t Window::GetHeight() const noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        return m_height;
    }

    const SDL_Surface* Window::GetSDLSurface() const noexcept {
        #ifdef LOG_ALL
           LOG_WIN_INFO(__FUNCTION__);
        #endif
        return m_surface_ptr;
    }

    SDL_Surface* Window::GetSDLSurface() noexcept {
        #ifdef LOG_ALL
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        return m_surface_ptr;
    }
    
    void Window::SDLDeinitializer::operator()(bool *is_sdl_initialized_ptr) const {
        LOG_WIN_INFO(__FUNCTION__);
        
        if (is_sdl_initialized_ptr) {
            SDL_Quit();
            *is_sdl_initialized_ptr = false;
        }
    }
    
    void Window::WindowDeleter::operator()(SDL_Window *window) const {
        LOG_WIN_INFO(__FUNCTION__);
        
        if (window != nullptr) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
    }
}