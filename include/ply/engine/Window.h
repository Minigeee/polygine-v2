#pragma once

#include <ply/core/Platform.h>
#include <ply/core/Types.h>
#include <ply/engine/Events.h>
#include <ply/engine/Input.h>
#include <ply/math/Types.h>

#include <cstdint>
#include <string>

#ifdef PLY_PLATFORM_WINDOWS
    #include <wtypes.h>
#endif

namespace ply {

class Editor;

#ifdef PLY_PLATFORM_WINDOWS
typedef HWND WindowHandle;
#endif

typedef uint32_t WindowId;

class Window
    : public EventDispatcher<
          Event::Key,
          Event::MouseButton,
          Event::MouseMove,
          Event::MouseScroll,
          Event::TextInput> {
    friend Input;
    friend Editor;

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
    Window(
        uint32_t w,
        uint32_t h,
        const std::string& title,
        bool fullscreen = false
    );

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
    ///////////////////////////////////////////////////////////
    /// \brief Gamepad axis motion event
    ///
    ///////////////////////////////////////////////////////////
    Window& operator=(Window&&);

    ///////////////////////////////////////////////////////////
    /// \brief Create a new window using the given settings
    ///
    /// \param w Width of the window in pixels
    /// \param h Height of the window in pixels
    /// \param title The title string
    /// \param fullscreen Should the window be fullscreen or not
    ///
    ///////////////////////////////////////////////////////////
    bool create(
        uint32_t w,
        uint32_t h,
        const std::string& title,
        bool fullscreen = false
    );

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
    /// \brief Set whether the cursor should be grabbed to this window
    /// \param grab True to grab the cursor
    ///
    /// Grabbing cursor locks the cursor to the boundaries of the window
    /// while the window is focused.
    ///
    ///////////////////////////////////////////////////////////
    void setCursorGrabbed(bool grab);

    ///////////////////////////////////////////////////////////
    /// \brief Set whether the cursor should be locked and hidden when focused to this window
    /// \param relative True to lock the cursor
    ///
    ///////////////////////////////////////////////////////////
    void setCursorLocked(bool locked);

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

    ///////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param key The key to check
    /// \return True if the specified key is pressed
    ///
    ///////////////////////////////////////////////////////////
    bool isKeyPressed(Keyboard::Scancode key) const;

    ///////////////////////////////////////////////////////////
    /// \brief Check if a mouse button is pressed
    ///
    /// \param button The mouse button to check
    /// \return True if the specified mouse button is pressed
    ///
    ///////////////////////////////////////////////////////////
    bool isMousePressed(Mouse::Button button) const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the current mouse position
    ///
    /// \return The current mouse position relative to the top-left of the
    /// window
    ///
    ///////////////////////////////////////////////////////////
    Vector2f getMousePosition() const;
    
    ///////////////////////////////////////////////////////////
    /// \brief Check if mouse is grabbed
    /// \return True if mouse is grabbed
    ///
    ///////////////////////////////////////////////////////////
    bool isCursorGrabbed() const;
    
    ///////////////////////////////////////////////////////////
    /// \brief Check if cursor is locked
    /// \return True if cursor is locked
    ///
    ///////////////////////////////////////////////////////////
    bool isCursorLocked() const;

private:
    void* m_window;     //!< Internal window pointer
    bool m_shouldClose; //!< Should window close

    static uint32_t
        s_numWindows; //!< Track number of windows for proper cleanup
    static HashMap<WindowId, Window*> s_windows; //!< Track window pointers
};

} // namespace ply