#pragma once

#include <ply/core/Platform.h>
#include <ply/core/Types.h>
#include <ply/math/Types.h>

#include <cstdint>
#include <string>

#ifdef PLY_PLATFORM_WINDOWS
#include <wtypes.h>
#endif

namespace ply {

#ifdef PLY_PLATFORM_WINDOWS
typedef HWND WindowHandle;
#endif

typedef uint32_t WindowId;

class Window {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    Window();

    ///////////////////////////////////////////////////////////
    /// \brief Construct a new window using the given settings
    ///
    /// \param w Width of the window in pixels
    /// \param h Height of the window in pixels
    /// \param title The title string
    /// \param fullscreen Should the window be fullscreen or not
    ///
    /// \see create
    ///
    ///////////////////////////////////////////////////////////
    Window(uint32_t w, uint32_t h, const std::string& title, bool fullscreen = false);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Closes the window, and if it is the last existing window.
    ///
    ///////////////////////////////////////////////////////////
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&);
    Window& operator=(Window&&);

    ///////////////////////////////////////////////////////////
    /// \brief Poll events for all windows
    ///
    /// This will process and send events for all input listeners.
    ///
    ///////////////////////////////////////////////////////////
    static void poll();

    ///////////////////////////////////////////////////////////
    /// \brief Create a new window using the given settings
    ///
    /// \param w Width of the window in pixels
    /// \param h Height of the window in pixels
    /// \param title The title string
    /// \param fullscreen Should the window be fullscreen or not
    ///
    ///////////////////////////////////////////////////////////
    bool create(uint32_t w, uint32_t h, const std::string& title, bool fullscreen = false);

    ///////////////////////////////////////////////////////////
    /// \brief Check if the window close has been requested
    ///
    /// \return True if the window should close
    ///
    ///////////////////////////////////////////////////////////
    bool shouldClose() const;

    ///////////////////////////////////////////////////////////
    /// \brief Close the window
    ///
    /// Use this function to close the window immediately
    ///
    ///////////////////////////////////////////////////////////
    void close();

    ///////////////////////////////////////////////////////////
    /// \brief Set the window size
    ///
    /// If the window is in fullscreen mode, the ressolution is
    /// changed. Otherwise, the size of the window is changed as well.
    ///
    /// \param size The new size
    ///
    ///////////////////////////////////////////////////////////
    void setSize(const Vector2u& size);

    ///////////////////////////////////////////////////////////
    /// \brief Set the window title
    ///
    /// \param title The new title
    ///
    ///////////////////////////////////////////////////////////
    void setTitle(const std::string& title);

    ///////////////////////////////////////////////////////////
    /// \brief Get the native window handle
    ///
    /// The return type will vary based on OS type: for Windows the
    /// type is HWND. More to come.
    ///
    /// \return The native window handle
    ///
    ///////////////////////////////////////////////////////////
    WindowHandle getNativeHandle() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the window size
    ///
    /// \return The window size
    ///
    ///////////////////////////////////////////////////////////
    Vector2i getSize() const;

private:
    void* m_window;     //!< Internal window pointer
    bool m_shouldClose; //!< Should window close

    static uint32_t s_numWindows;                //!< Track number of windows for proper cleanup
    static HashMap<WindowId, Window*> s_windows; //!< Track window pointers
};

}