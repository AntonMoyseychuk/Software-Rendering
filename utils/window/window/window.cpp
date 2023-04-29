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
        : m_window_ptr(std::move(window.m_window_ptr)), 
            m_surface_ptr(window.m_surface_ptr), 
            m_event(window.m_event),
            m_title(std::move(window.m_title)), 
            m_width(window.m_width), 
            m_height(window.m_height), 
            m_is_quit(window.m_is_quit)
    {
        LOG_WIN_INFO(__FUNCTION__);
        memset(&window.m_event, 0, sizeof(window.m_event));
    }

    Window &Window::operator=(Window &&window) noexcept {
        LOG_WIN_INFO(__FUNCTION__);

        m_title = std::move(window.m_title);
        m_width = window.m_width;
        m_height = window.m_height;
        m_is_quit = window.m_is_quit;

        m_window_ptr = std::move(window.m_window_ptr);
        m_renderer_ptr = std::move(window.m_renderer_ptr);
        m_texture_ptr = std::move(window.m_texture_ptr);
        m_pixel_format_ptr = std::move(window.m_pixel_format_ptr);
        
        m_event = window.m_event;

        window.m_surface_ptr = nullptr;
        memset(&window.m_event, 0, sizeof(window.m_event));

        return *this;
    }
        
    std::uint32_t Window::_MapRGBA(SDL_PixelFormat *format, _InternalColor color) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        return SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
    }

    bool Window::_InitializeSDL() {
        LOG_WIN_INFO(__FUNCTION__);
        
        return SDL_Init(SDL_INIT_EVERYTHING) == 0;
    }

    void Window::_OnResize(std::uint32_t new_width, std::uint32_t new_height) noexcept {
        LOG_WIN_EVENT("SDL_WINDOWEVENT_RESIZED", 
            "New size -> [" + std::to_string(new_width) + ", " + std::to_string(new_width) + "]");
            
        m_width = new_width;
        m_height = new_height;

        m_texture_ptr.reset(SDL_CreateTexture(m_renderer_ptr.get(), SDL_PIXELFORMAT_RGBA8888, 
            SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, m_width, m_height));
        LOG_SDL_ERROR(m_texture_ptr != nullptr, SDL_GetError());
    }

    void Window::_OnQuit() noexcept {
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
        m_height = height;//

        m_window_ptr.reset(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0));
        LOG_SDL_ERROR(m_window_ptr != nullptr, SDL_GetError());
        SDL_SetWindowResizable(m_window_ptr.get(), SDL_TRUE);

        const SDL_Surface* window_surface = SDL_GetWindowSurface(m_window_ptr.get());
        LOG_SDL_ERROR(window_surface, SDL_GetError());

        m_pixel_format_ptr = window_surface->format;

        m_renderer_ptr.reset(SDL_CreateRenderer(m_window_ptr.get(), -1, SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE));
        LOG_SDL_ERROR(m_renderer_ptr != nullptr, SDL_GetError());

        m_texture_ptr.reset(SDL_CreateTexture(m_renderer_ptr.get(), SDL_PIXELFORMAT_RGBA8888, 
            SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, width, height));
        LOG_SDL_ERROR(m_texture_ptr != nullptr, SDL_GetError());

        return true;
    }

    bool Window::IsOpen() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        return !m_is_quit;
    }
    
    void Window::_ThreadBufferFillingFunc(
        std::uint32_t x0, 
        std::uint32_t y0, 
        std::uint32_t x_end, 
        std::uint32_t y_end, 
        SDL_Surface* surface, 
        const std::uint32_t* in_pixels
    ) noexcept {
        
        const auto lenght = x_end - x0;
        auto buffer = static_cast<std::uint32_t*>(surface->pixels);

        for (std::size_t y = y0; y < y_end; ++y) {
            for (std::size_t x = x0; x < x_end; ++x) {
                buffer[x + y * lenght] = _MapRGBA(surface->format, _InternalColor(in_pixels[x + y * lenght]));
            }
        }
    }

    void Window::FillPixelBuffer(const std::vector<std::uint32_t>& in_pixels) const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        const std::uint32_t step = 1;
        for (std::uint32_t y = 0; y < m_height; y += step) {
            m_thread_pool.AddTask(&Window::_ThreadBufferFillingFunc, 0, y, m_width, y + step, m_surface_ptr, in_pixels.data());
        }
        m_thread_pool.WaitAll();

        SDL_UnlockTexture(m_texture_ptr.get());
    }

    void Window::PresentPixelBuffer() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        LOG_SDL_ERROR(_UpdateSurface(), SDL_GetError());
        LOG_SDL_ERROR(SDL_UpdateWindowSurface(m_window_ptr.get()) == 0, SDL_GetError());
    }

    void Window::PollEvent() noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        while (SDL_PollEvent(&m_event)) {
            switch (m_event.type)  {
            case SDL_QUIT:
                this->_OnQuit();
                break;

            case SDL_WINDOWEVENT:
                if(m_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    this->_OnResize(m_event.window.data1, m_event.window.data2);
                }
                break;
            }
        }
    }

    bool Window::IsKeyPressed(Key key) const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif
        
        static const std::uint8_t* keyboard = SDL_GetKeyboardState(nullptr);

        return keyboard[static_cast<SDL_Scancode>(key)];
    }

    // std::uint32_t Window::GetPixelColor(std::size_t x, std::size_t y) noexcept
    // {
    //     #if defined(LOG_ALL)
    //         LOG_WIN_INFO(__FUNCTION__);
    //     #endif
        
    //     if (x >= static_cast<std::size_t>(m_surface_ptr->w) || y >= static_cast<std::size_t>(m_surface_ptr->h)) {
    //         return 0;
    //     }
    
    //     auto pixels = static_cast<std::uint32_t*>(m_surface_ptr->pixels);
    //     std::uint8_t r, g, b, a;
    //     SDL_GetRGBA(pixels[y * m_surface_ptr->w + x], m_surface_ptr->format, &r, &g, &b, &a);

    //     #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    //         return (r << 24) + (g << 16) + (g << 8) + a;
    //     #else
    //         return (a << 24) + (b << 16) + (g << 8) + r;
    //     #endif
    // }

    // void Window::SetPixelColor(std::size_t x, std::size_t y, std::uint32_t color) noexcept {
    //     #if defined(LOG_ALL)
    //         LOG_WIN_INFO(__FUNCTION__);
    //     #endif

        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            return (r << 24) + (g << 16) + (g << 8) + a;
        #else
            return (a << 24) + (b << 16) + (g << 8) + r;
        #endif
    }

    void Window::SetPixelColor(
        std::size_t x, 
        std::size_t y, 
        std::uint8_t r, 
        std::uint8_t g, 
        std::uint8_t b, 
        std::uint8_t a
    ) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        if (x < m_width && y < m_height) {
            auto buffer = static_cast<std::uint32_t*>(m_surface_ptr->pixels);
            buffer[x + y * m_width] = _MapRGBA(m_surface_ptr->format, _InternalColor(r, g, b, a));
        }
    }

    void Window::SetPixelColor(std::size_t x, std::size_t y, std::uint32_t color) noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        if (x < m_width && y < m_height) {
            auto buffer = static_cast<std::uint32_t*>(m_surface_ptr->pixels);
            buffer[x + y * m_width] = _MapRGBA(m_surface_ptr->format, _InternalColor(color));
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

    void Window::SetWidth(std::uint32_t width) noexcept  {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        m_width = width;
        SDL_SetWindowSize(m_window_ptr.get(), m_width, m_height);
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
    }

    std::uint32_t Window::GetHeight() const noexcept {
        #if defined(LOG_ALL)
            LOG_WIN_INFO(__FUNCTION__);
        #endif

        return m_height;
    }
    
    void Window::SDLDeinitializer::operator()(bool *is_sdl_initialized_ptr) const {
        LOG_WIN_INFO(__FUNCTION__);
        
        if (is_sdl_initialized_ptr) {
            SDL_Quit();
            *is_sdl_initialized_ptr = false;
        }
    }
    
    void Window::WindowDestroyer::operator()(SDL_Window *window) const {
        LOG_WIN_INFO(__FUNCTION__);
        
        if (window != nullptr) {
            SDL_DestroyWindow(window);
        }
    }
    
    void Window::RendererDestroyer::operator()(SDL_Renderer *renderer) const {
        LOG_WIN_INFO(__FUNCTION__);
        
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
        }
    }
    
    void Window::TextureDestroyer::operator()(SDL_Texture *texture) const {
        LOG_WIN_INFO(__FUNCTION__);
        
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }
}