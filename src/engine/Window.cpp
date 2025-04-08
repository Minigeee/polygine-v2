#include <ply/engine/Window.h>

#include <SDL3/SDL.h>
#include <X11/X.h>
#include <loguru.hpp>

#define WINDOW_CAST(x) (reinterpret_cast<SDL_Window*>(x))

namespace ply {

///////////////////////////////////////////////////////////
uint32_t Window::s_numWindows = 0;
HashMap<WindowId, Window*> Window::s_windows;

///////////////////////////////////////////////////////////
Window::Window() :
    m_window(nullptr),
    m_shouldClose(false) {}

///////////////////////////////////////////////////////////
Window::Window(
    uint32_t w,
    uint32_t h,
    const std::string& title,
    bool fullscreen
) :
    m_window(0),
    m_shouldClose(false) {
    create(w, h, title, fullscreen);
}

///////////////////////////////////////////////////////////
Window::Window(Window&& other) :
    m_window(other.m_window),
    m_shouldClose(other.m_shouldClose) {
    other.m_window = 0;

    // Update tracking
    if (m_window)
        s_windows[SDL_GetWindowID(WINDOW_CAST(m_window))] = this;
}

///////////////////////////////////////////////////////////
Window& Window::operator=(Window&& other) {
    if (&other != this) {
        m_window = other.m_window;
        m_shouldClose = other.m_shouldClose;
        other.m_window = 0;

        // Update tracking
        if (m_window)
            s_windows[SDL_GetWindowID(WINDOW_CAST(m_window))] = this;
    }

    return *this;
}

///////////////////////////////////////////////////////////
Window::~Window() {
    // Remove tracking
    if (m_window) {
        s_windows.erase(SDL_GetWindowID(WINDOW_CAST(m_window)));
    }

    if (!--s_numWindows)
        // If this is the last window, terminate SDL
        SDL_Quit();

    else
        // Otherwise, just close the window
        close();

    m_window = nullptr;
}

///////////////////////////////////////////////////////////
bool Window::create(
    uint32_t w,
    uint32_t h,
    const std::string& title,
    bool fullscreen
) {
    // Don't create if window is already open
    if (m_window)
        return false;

    LOG_F(INFO, "Creating new window");
    LOG_F(INFO, "Window resolution: %dx%d", w, h);
    LOG_F(INFO, "Window title: \"%s\"", title.c_str());

    // Initialize SDL
    if (!s_numWindows) {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            LOG_F(ERROR, "Failed to initialize SDL");
            return false;
        }
    }

    // Create window
    SDL_WindowFlags flags = 0;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    SDL_Window* window = SDL_CreateWindow(title.c_str(), w, h, flags);
    m_window = window;
    if (!m_window) {
        LOG_F(ERROR, "Failed to create window");

        // Terminate SDL if there are no other windows
        if (!s_numWindows)
            SDL_Quit();

        return false;
    }

    // Update member variables
    ++s_numWindows;

    // Track window
    auto windowId = SDL_GetWindowID(window);
    CHECK_F(windowId < 100, "window id too large");
    s_windows[windowId] = this;

    return true;
}

///////////////////////////////////////////////////////////
bool Window::shouldClose() const {
    // The pointer exists, the window is open
    return !(bool)m_window || m_shouldClose;
}

///////////////////////////////////////////////////////////
void Window::close() {
    if (m_window) {
        // Destroy window
        SDL_DestroyWindow(WINDOW_CAST(m_window));

        // Reset pointer
        m_window = nullptr;
        m_shouldClose = false;
    }
}

///////////////////////////////////////////////////////////
void Window::setSize(const Vector2u& resolution) {
    // Check if window is open
    if (!m_window)
        return;

    SDL_SetWindowSize(WINDOW_CAST(m_window), resolution.x, resolution.y);
}

///////////////////////////////////////////////////////////
void Window::setTitle(const std::string& title) {
    // Check if window is open
    if (!m_window)
        return;

    SDL_SetWindowTitle(WINDOW_CAST(m_window), title.c_str());
}

///////////////////////////////////////////////////////////
WindowHandle Window::getNativeHandle() const {
    SDL_Window* window = WINDOW_CAST(m_window);

#ifdef PLY_PLATFORM_WINDOWS
    return (WindowHandle)SDL_GetPointerProperty(
        SDL_GetWindowProperties(window),
        SDL_PROP_WINDOW_WIN32_HWND_POINTER,
        NULL
    );
#else
    return NULL;
#endif
}

///////////////////////////////////////////////////////////
Vector2i Window::getSize() const {
    Vector2i size;
    SDL_GetWindowSize(WINDOW_CAST(m_window), &size.x, &size.y);
    return size;
}

///////////////////////////////////////////////////////////
bool Window::isKeyPressed(Keyboard::Scancode key) const {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    return keys[(SDL_Keycode)key];
}

///////////////////////////////////////////////////////////
bool Window::isMousePressed(Mouse::Button button) const {
    uint32_t state = SDL_GetMouseState(nullptr, nullptr);
    return (bool)(SDL_BUTTON_MASK((uint32_t)button) & state);
}

///////////////////////////////////////////////////////////
Vector2f Window::getMousePosition() const {
    Vector2f pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

} // namespace ply