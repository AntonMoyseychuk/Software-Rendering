#include "window.hpp"

#include <algorithm>
#include <execution> 

#if defined(_DEBUG)
    #include <iostream>
    #define LOG(tag, msg) std::cerr << '[' << (tag) << "]\t" << (msg) << '\n'

    #define LOG_WIN_ERROR(condition, tag, msg) if (!(condition)) { \
        LOG((tag), (msg)); \
        exit(-1); \
    }

    #define LOG_WIN_INFO(msg) LOG("window info", (msg))
    #define LOG_WIN_EVENT(event_type, msg) LOG("event", "type: " + std::string(event_type) + ". " + msg)
    #define LOG_SDL_ERROR(condition, msg) LOG_WIN_ERROR((condition), "sdl error", msg)
#else
    #define LOG(tag, msg) (void)0
    
    #define LOG_WIN_ERROR(condition, tag, msg) if (!(condition)) { \
        LOG((tag), (msg)); \
        exit(-1); \
    }

    #define LOG_WIN_INFO(msg) (void)0
    #define LOG_WIN_EVENT(event_type, msg) (void)0
    #define LOG_SDL_ERROR(condition, msg) LOG_WIN_ERROR((condition), "sdl error", msg)
#endif


namespace win_framewrk {
    std::unique_ptr<bool, Window::SDLDeinitializer> Window::is_sdl_initialized_ptr = nullptr;

    Window* Window::Get() noexcept {
        LOG_WIN_INFO(__FUNCTION__);

        if (is_sdl_initialized_ptr == nullptr || *is_sdl_initialized_ptr == false) {
            is_sdl_initialized_ptr.reset(new bool(_InitializeSDL()));
        }

        LOG_SDL_ERROR(*is_sdl_initialized_ptr, SDL_GetError());
        
        static Window window;
        return &window;
    }

    Window::Window(Window &&window)
        : m_title(std::move(window.m_title)), m_width(window.m_width), m_height(window.m_height), m_is_quit(window.m_is_quit),
            m_window_ptr(std::move(window.m_window_ptr)), m_surface_ptr(window.m_surface_ptr), m_event(window.m_event), m_background_color(0)
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
        m_background_color = window.m_background_color;

        window.m_surface_ptr = nullptr;
        memset(&window.m_event, 0, sizeof(window.m_event));

        return *this;
    }

    std::uint32_t Window::_ConvertToBigEndian(std::uint32_t color) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        #if SDL_BYTEORDER == SDL_LIL_ENDIAN
            return SDL_SwapBE32(color);
        #else
            return color;
        #endif
    }
        
    std::uint32_t Window::_MapRGBA(SDL_PixelFormat *format, std::uint32_t color) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        color = _ConvertToBigEndian(color);

        std::uint8_t rgba[4];
        for (std::size_t i = 0; i < 4; ++i) {
            rgba[3 - i] = (color >> (8 * i)) & 0xFF;
        }
        return SDL_MapRGBA(format, rgba[0], rgba[1], rgba[2], rgba[3]);
    }

    bool Window::_InitializeSDL() {
        LOG_WIN_INFO(__FUNCTION__);
        
        return SDL_Init(SDL_INIT_EVERYTHING) == 0;
    }

    bool Window::_UpdateSurface() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        m_surface_ptr = SDL_GetWindowSurface(m_window_ptr.get());
        return m_surface_ptr != nullptr;
    }

    void Window::_UpdateVerticalIterator(std::uint32_t new_height) noexcept {
        if (new_height != m_vertical_it.size()) {
            m_vertical_it.resize(m_height);
            for (std::size_t i = 0; i < m_height; ++i) {
                m_vertical_it[i] = i;
            }
        }
    }
    
    void Window::_OnWindowEvent() noexcept
    {
        if(m_event.window.event == SDL_WINDOWEVENT_RESIZED) {
            LOG_WIN_EVENT("SDL_WINDOWEVENT_RESIZED", 
                "New size -> [" + std::to_string(m_surface_ptr->w) + ", " + std::to_string(m_surface_ptr->h) + "]");
            
            SDL_GetWindowSize(m_window_ptr.get(), (int*)&m_width, (int*)&m_height);
            _UpdateVerticalIterator(m_height);

            LOG_SDL_ERROR(_UpdateSurface(), SDL_GetError());
        }
    }

    void Window::_OnQuitEvent() noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        m_is_quit = true;
    }
    
    bool Window::Init(const std::string& title, std::uint32_t width, std::uint32_t height) {
        LOG_WIN_INFO(__FUNCTION__);

        if (m_window_ptr != nullptr) {
            LOG_WIN_INFO("There was an atempt to secondory initialize window");
            return false;
        }

        m_width = width;
        m_height = height;

        _UpdateVerticalIterator(m_height);

        m_window_ptr.reset(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0));
        LOG_SDL_ERROR(m_window_ptr != nullptr, SDL_GetError());
        SDL_SetWindowResizable(m_window_ptr.get(), SDL_TRUE);

        LOG_SDL_ERROR(_UpdateSurface() == true, SDL_GetError());

        return true;
    }

    bool Window::IsOpen() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        return !m_is_quit;
    }

    void Window::FillPixelBuffer(const std::vector<std::size_t> &in_pixels) const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        auto pixel_buffer = static_cast<std::uint32_t*>(m_surface_ptr->pixels);

        std::for_each(std::execution::par, m_vertical_it.cbegin(), m_vertical_it.cend(), [this, &pixel_buffer, &in_pixels](std::uint32_t y) {
            for (std::size_t x = 0; x < m_width; ++x) {
                pixel_buffer[x + y * m_width] = _MapRGBA(m_surface_ptr->format, in_pixels[x + y * m_width]);
            }
        });
    }

    void Window::PresentPixelBuffer() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        _UpdateSurface();

        LOG_SDL_ERROR(SDL_UpdateWindowSurface(m_window_ptr.get()) == 0, SDL_GetError());
    }

    void Window::PollEvent() noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        while (SDL_PollEvent(&m_event)) {
            switch (m_event.type)  {
            case SDL_QUIT:
                this->_OnQuitEvent();
                break;

            case SDL_WINDOWEVENT:
                this->_OnWindowEvent();
                break;
            }
        }
    }

    bool Window::IsKeyPressed(Key key) const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        
        SDL_PumpEvents();
        return SDL_GetKeyboardState(nullptr)[static_cast<SDL_Scancode>(key)];
    }

    std::uint32_t Window::GetPixelColor(std::size_t x, std::size_t y) noexcept
    {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        
        if (x >= static_cast<std::size_t>(m_surface_ptr->w) || y >= static_cast<std::size_t>(m_surface_ptr->h)) {
            return 0;
        }
    
        auto pixels = static_cast<std::uint32_t*>(m_surface_ptr->pixels);
        std::uint8_t r, g, b, a;
        SDL_GetRGBA(pixels[y * m_surface_ptr->w + x], m_surface_ptr->format, &r, &g, &b, &a);

        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            return (r << 24) + (g << 16) + (g << 8) + a;
        #else
            return (a << 24) + (b << 16) + (g << 8) + r;
        #endif
    }

    void Window::SetPixelColor(std::size_t x, std::size_t y, std::uint32_t color) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        if (x < m_width && y < m_height) {
            static_cast<std::uint32_t*>(m_surface_ptr->pixels)[x + y * m_width] = _MapRGBA(m_surface_ptr->format, color);
        }
    }

    void Window::SetTitle(const std::string_view title) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        m_title = title;
        SDL_SetWindowTitle(m_window_ptr.get(), m_title.c_str());
    }

    const std::string& Window::GetTitle() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        return m_title;
    }

    void Window::SetBackgroundColor(std::uint32_t color) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        m_background_color = color;
    }

    std::uint32_t Window::GetBackgroundColor() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        return m_background_color;
    }

    void Window::SetWidth(std::uint32_t width) noexcept  {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        m_width = width;
        SDL_SetWindowSize(m_window_ptr.get(), m_width, m_height);

        LOG_SDL_ERROR(_UpdateSurface() == true, SDL_GetError());
    }

    std::uint32_t Window::GetWidth() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        return m_width;
    }

    void Window::SetHeight(std::uint32_t height) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        m_height = height;
        SDL_SetWindowSize(m_window_ptr.get(), m_width, m_height);
        
        LOG_SDL_ERROR(_UpdateSurface() == true, SDL_GetError());
    }

    std::uint32_t Window::GetHeight() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        return m_height;
    }

    const SDL_Surface* Window::GetSDLSurfaceHandle() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        return m_surface_ptr;
    }

    SDL_Surface* Window::GetSDLSurfaceHandle() noexcept {
        #if defined(LOG_ALL)
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