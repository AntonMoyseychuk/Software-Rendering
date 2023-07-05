#include "window.hpp"

#include <algorithm>
#include <execution> 

#ifdef _DEBUG
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
    Window* Window::Get() noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        
        static Window window;
        return &window;
    }

    Window::Window(Window &&window)
        : m_window_ptr(std::move(window.m_window_ptr)), 
            m_surface_ptr(window.m_surface_ptr), 
            m_event(window.m_event),
            m_title(std::move(window.m_title)), 
            m_width(window.m_width), 
            m_height(window.m_height), 
            m_is_quit(window.m_is_quit)
    {
        LOG_WIN_INFO(__FUNCTION__);

        window.m_surface_ptr = nullptr;
        memset(&window.m_event, 0, sizeof(window.m_event));
    }

    Window &Window::operator=(Window &&window) noexcept {
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
        
    uint32_t Window::_MapRGBA(SDL_PixelFormat *format, _InternalColor color) noexcept {
        return SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
    }

    Window::Window() {
        LOG_WIN_INFO(__FUNCTION__);

        LOG_SDL_ERROR(SDL_Init(SDL_INIT_EVERYTHING) == 0, SDL_GetError());
    }

    Window::~Window() {
        LOG_WIN_INFO(__FUNCTION__);

        SDL_DestroyWindow(m_window_ptr);
        SDL_Quit();
    }

    bool Window::_UpdateSurface() const noexcept {
        m_surface_ptr = SDL_GetWindowSurface(m_window_ptr);
        return m_surface_ptr != nullptr;
    }

    void Window::_OnResize(uint32_t new_width, uint32_t new_height) noexcept {
        LOG_WIN_EVENT("SDL_WINDOWEVENT_RESIZED", 
            "New size -> [" + std::to_string(new_width) + ", " + std::to_string(new_height) + "]");
            
        m_width = new_width;
        m_height = new_height;

        if (_ResizeCallback) {
            _ResizeCallback(new_width, new_height);
        }

        LOG_SDL_ERROR(_UpdateSurface(), SDL_GetError());
    }

    void Window::_OnQuit() noexcept {
        LOG_WIN_INFO(__FUNCTION__);
        m_is_quit = true;
    }


    bool Window::Init(const std::string& title, uint32_t width, uint32_t height) {
        LOG_WIN_INFO(__FUNCTION__);

        if (m_window_ptr != nullptr) {
            LOG_WIN_INFO("There was an atempt to secondory initialize window");
            return false;
        }

        m_width = width;
        m_height = height;

        m_window_ptr = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        LOG_SDL_ERROR(m_window_ptr != nullptr, SDL_GetError());
        SDL_SetWindowResizable(m_window_ptr, SDL_TRUE);

        LOG_SDL_ERROR(_UpdateSurface() == true, SDL_GetError());

        return true;
    }

    bool Window::IsOpen() const noexcept {
        return !m_is_quit;
    }
    
    void Window::_ThreadBufferFillingFunc(uint32_t x0, uint32_t y0, uint32_t x_end, uint32_t y_end, 
        SDL_Surface* surface, const uint32_t* in_pixels
    ) noexcept {

        const auto lenght = x_end - x0;
        auto buffer = static_cast<uint32_t*>(surface->pixels);

        for (size_t y = y0; y < y_end; ++y) {
            for (size_t x = x0; x < x_end; ++x) {
                buffer[x + y * lenght] = _MapRGBA(surface->format, _InternalColor(in_pixels[x + y * lenght]));
            }
        }
    }

    void Window::FillPixelBuffer(const std::vector<uint32_t>& in_pixels) const noexcept {
        const uint32_t step = m_height & 1 ? 1 : 2;
        for (uint32_t y = 0; y < m_height; y += step) {
            m_thread_pool.AddTask(&Window::_ThreadBufferFillingFunc, 0, y, m_width, y + step, m_surface_ptr, in_pixels.data());
        }

        m_thread_pool.WaitAll();
    }

    void Window::FillPixelBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const noexcept {
        LOG_SDL_ERROR(SDL_FillRect(m_surface_ptr, NULL, SDL_MapRGBA(m_surface_ptr->format, r, g, b, a)) == 0, SDL_GetError());
    }

    void Window::PresentPixelBuffer() const noexcept {
        LOG_SDL_ERROR(_UpdateSurface(), SDL_GetError());
        LOG_SDL_ERROR(SDL_UpdateWindowSurface(m_window_ptr) == 0, SDL_GetError());
        memset(m_surface_ptr->pixels, 0, sizeof(uint32_t) * m_surface_ptr->w * m_surface_ptr->h);
    }

    void Window::PollEvent() noexcept {
        while (SDL_PollEvent(&m_event)) {
            switch (m_event.type)  {
            case SDL_QUIT:
                this->_OnQuit();
                break;

            case SDL_WINDOWEVENT:
                if (m_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    this->_OnResize(m_event.window.data1, m_event.window.data2);
                }
                break;
            }
        }
    }

    bool Window::IsKeyPressed(Key key) const noexcept {
        static const uint8_t* keyboard = SDL_GetKeyboardState(nullptr);

        return keyboard[static_cast<SDL_Scancode>(key)];
    }

    bool Window::IsMousePressed(MouseState &state) const noexcept {
        const uint32_t s = SDL_GetMouseState(&state.x, &state.y);

        state.pressed_button = static_cast<MouseState::PressedButton>(s != 0 ? SDL_BUTTON(s) : 0);
        return state.pressed_button != MouseState::PressedButton::NONE;
    }

    uint32_t Window::GetPixelColor(size_t x, size_t y) noexcept {
        if (x >= static_cast<size_t>(m_surface_ptr->w) || y >= static_cast<size_t>(m_surface_ptr->h)) {
            return 0;
        }
    
        const uint32_t* pixels = static_cast<uint32_t*>(m_surface_ptr->pixels);
        uint8_t r, g, b, a;
        SDL_GetRGBA(pixels[y * m_surface_ptr->w + x], m_surface_ptr->format, &r, &g, &b, &a);

        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            return (r << 24) + (g << 16) + (g << 8) + a;
        #else
            return (a << 24) + (b << 16) + (g << 8) + r;
        #endif
    }

    void Window::SetPixelColor(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept {
        if (x < m_width && y < m_height) {
            auto buffer = static_cast<uint32_t*>(m_surface_ptr->pixels);
            buffer[x + y * m_width] = SDL_MapRGBA(m_surface_ptr->format, r, g, b, a);
        }
    }

    void Window::SetPixelColor(size_t x, size_t y, uint32_t color) noexcept {
        const _InternalColor packed_color(color);
        SetPixelColor(x, y, packed_color.r, packed_color.g, packed_color.b, packed_color.a);
    }

    void Window::SetTitle(const std::string_view title) noexcept {
        m_title = title;
        SDL_SetWindowTitle(m_window_ptr, m_title.c_str());
    }

    const std::string& Window::GetTitle() const noexcept {
        return m_title;
    }

    void Window::SetWidth(uint32_t width) noexcept  {
        m_width = width;
        SDL_SetWindowSize(m_window_ptr, m_width, m_height);

        LOG_SDL_ERROR(_UpdateSurface() == true, SDL_GetError());
    }

    uint32_t Window::GetWidth() const noexcept {
        return m_width;
    }

    void Window::SetHeight(uint32_t height) noexcept {
        m_height = height;
        SDL_SetWindowSize(m_window_ptr, m_width, m_height);
        
        LOG_SDL_ERROR(_UpdateSurface() == true, SDL_GetError());
    }

    uint32_t Window::GetHeight() const noexcept {
        return m_height;
    }

    const SDL_Surface* Window::GetSDLSurfaceHandle() const noexcept {
        return m_surface_ptr;
    }

    SDL_Surface* Window::GetSDLSurfaceHandle() noexcept {
        return m_surface_ptr;
    }

    const SDL_Window *Window::GetSDLWindowHandle() const noexcept {
        return m_window_ptr;
    }

    SDL_Window *Window::GetSDLWindowHandle() noexcept {
        return m_window_ptr;
    }

    void Window::SetResizeCallback(const std::function<void(uint32_t width, uint32_t height)> &callback) const noexcept {
        _ResizeCallback = callback;
    }
}