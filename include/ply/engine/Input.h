#pragma once

#include <ply/engine/Keyboard.h>
#include <ply/engine/Mouse.h>
#include <ply/math/Types.h>

namespace ply {

class Window;

///////////////////////////////////////////////////////////
/// \brief Input action
///
///////////////////////////////////////////////////////////
enum class InputAction { Release = 0, Press, Repeat };

namespace Event {

    ///////////////////////////////////////////////////////////
    /// \brief Keyboard event
    ///
    ///////////////////////////////////////////////////////////
    struct Key {
        Keyboard::Key key;       //!< The key that was pressed/released/held
        Keyboard::Scancode scan; //!< The scancode of the key
        InputAction action;      //!< The action the occured (pressed/released/held)
    };

    ///////////////////////////////////////////////////////////
    /// \brief Mouse button event
    ///
    ///////////////////////////////////////////////////////////
    struct MouseButton {
        Mouse::Button button; //!< The mouse button that was pressed/released
        InputAction action;   //!< The action the occured pressed/released
        float x; //!< The x-coordinate of the mouse relative to the top-left of the window
        float y; //!< The y-coordinate of the mouse relative to the top-left of the window
    };

    ///////////////////////////////////////////////////////////
    /// \brief Mouse move event
    ///
    ///////////////////////////////////////////////////////////
    struct MouseMove {
        float x;  //!< The x-coordinate of the mouse relative to the top-left of the window
        float y;  //!< The y-coordinate of the mouse relative to the top-left of the window
        float dx; //!< The change in x-coordinate since the last event
        float dy; //!< The change in y-coordinate since the last event
    };

    ///////////////////////////////////////////////////////////
    /// \brief Mouse scroll event
    ///
    ///////////////////////////////////////////////////////////
    struct MouseScroll {
        float dx; //!< The amount scrolled on the x-axis
        float dy; //!< The amount scrolled on the y-axis
    };

    ///////////////////////////////////////////////////////////
    /// \brief A character input event
    ///
    ///////////////////////////////////////////////////////////
    struct TextInput {
        uint32_t character; //!< The UTF8 character code
    };

}

///////////////////////////////////////////////////////////
/// \brief Input handler class
///
///////////////////////////////////////////////////////////
class Input {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Poll events for all windows
    ///
    /// This will process and send events for all input listeners.
    ///
    ///////////////////////////////////////////////////////////
    static void poll();

    ///////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param key The key to check
    /// \return True if the specified key is pressed
    ///
    ///////////////////////////////////////////////////////////
    static bool isKeyPressed(Keyboard::Scancode key);

    ///////////////////////////////////////////////////////////
    /// \brief Check if a mouse button is pressed
    ///
    /// \param button The mouse button to check
    /// \return True if the specified mouse button is pressed
    ///
    ///////////////////////////////////////////////////////////
    static bool isMousePressed(Mouse::Button button);

    ///////////////////////////////////////////////////////////
    /// \brief Get the current mouse position
    ///
    /// \return The current mouse position relative to the top-left of the window
    ///
    ///////////////////////////////////////////////////////////
    static Vector2f getMousePosition();
};

}