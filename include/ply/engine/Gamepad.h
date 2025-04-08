#pragma once

#include <ply/engine/Events.h>
#include <ply/engine/Input.h>

#include <cstdint>

namespace ply {

class Input;
class GamepadHandler;

typedef uint32_t GamepadId;

class Gamepad {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Gamepad types
    ///
    ///////////////////////////////////////////////////////////
    enum class Type {
        Unknown = 0,
        Standard,
        XBox360,
        XBoxOne,
        PS3,
        PS4,
        PS5,
        NintendoSwitchPro,
        NintendoSwitchJoyconLeft,
        NintendoSwitchJoyconRight,
        NintendoSwitchJoyconPair,
        COUNT
    };

    ///////////////////////////////////////////////////////////
    /// \brief Gamepad buttons
    ///
    /// Equivalent to SDL Gamepad buttons
    ///
    /// For controllers that use a diamond pattern for the face buttons, the
    /// south/east/west/north buttons below correspond to the locations in the
    /// diamond pattern. For Xbox controllers, this would be A/B/X/Y, for Nintendo
    /// Switch controllers, this would be B/A/Y/X, for PlayStation controllers this
    /// would be Cross/Circle/Square/Triangle.
    ///
    /// For controllers that don't use a diamond pattern for the face buttons, the
    /// south/east/west/north buttons indicate the buttons labeled A, B, C, D, or
    /// 1, 2, 3, 4, or for controllers that aren't labeled, they are the primary,
    /// secondary, etc. buttons.
    ///
    /// The activate action is often the south button and the cancel action is
    /// often the east button, but in some regions this is reversed, so your game
    /// should allow remapping actions based on user preferences.
    ///
    ///////////////////////////////////////////////////////////
    enum class Button {
        Unknown = -1,
        South, /**< Bottom face button (e.g. Xbox A button) */
        East,  /**< Right face button (e.g. Xbox B button) */
        West,  /**< Left face button (e.g. Xbox X button) */
        North, /**< Top face button (e.g. Xbox Y button) */
        Back,
        Guide,
        Start,
        LeftStick,
        RightStick,
        LeftShoulder,
        RightShoulder,
        DpadUp,
        DpadDown,
        DpadLeft,
        DpadRight,
        Misc1,        /**< Additional button (e.g. Xbox Series X share button, PS5
                                            microphone button, Nintendo Switch Pro capture button, Amazon
                                            Luna microphone button, Google Stadia capture button) */
        RightPaddle1, /**< Upper or primary paddle, under your right hand (e.g.
                                             Xbox Elite paddle P1) */
        LeftPaddle1,  /**< Upper or primary paddle, under your left hand (e.g.
                                             Xbox Elite paddle P3) */
        RightPaddle2, /**< Lower or secondary paddle, under your right hand
                                             (e.g. Xbox Elite paddle P2) */
        LeftPaddle2,  /**< Lower or secondary paddle, under your left hand (e.g.
                                             Xbox Elite paddle P4) */
        Touchpad,     /**< PS4/PS5 touchpad button */
        Misc2,        /**< Additional button */
        Misc3,        /**< Additional button */
        Misc4,        /**< Additional button */
        Misc5,        /**< Additional button */
        Misc6,        /**< Additional button */

        COUNT
    };

    ///////////////////////////////////////////////////////////
    /// \brief Gamepad axes
    ///
    /// Equivalent to SDL Gamepad axes
    ///
    /// Represents the analog inputs of a gamepad, such as joysticks and triggers.
    ///
    ///////////////////////////////////////////////////////////
    enum class Axis {
        Unknown = -1,
        LeftX,
        LeftY,
        RightX,
        RightY,
        LeftTrigger,
        RightTrigger,

        COUNT
    };

public:
    ///////////////////////////////////////////////////////////
    /// \brief Enable gamepad support
    ///
    /// \return True if gamepad support was successfully enabled
    ///
    ///////////////////////////////////////////////////////////
    static bool enable();

    ///////////////////////////////////////////////////////////
    /// \brief Get the handler to dispatch events
    ///
    /// This will return the handler that is used to dispatch events for all gamepads.
    ///
    ///////////////////////////////////////////////////////////
    static GamepadHandler& getHandler();

    ///////////////////////////////////////////////////////////
    /// \brief Get the list of gamepads
    ///
    /// \return List of gamepads
    ///
    ///////////////////////////////////////////////////////////
    static std::vector<GamepadId> getDevices();

    ///////////////////////////////////////////////////////////
    /// \brief Check if a gamepad is connected
    ///
    /// \param id The gamepad ID
    /// \return True if the gamepad is connected
    ///
    ///////////////////////////////////////////////////////////
    static bool isConnected(GamepadId id);

    ///////////////////////////////////////////////////////////
    /// \brief Check if a gamepad has a button
    ///
    /// \param id The gamepad ID
    /// \param button The button to check
    /// \return True if the gamepad has the button
    ///
    ///////////////////////////////////////////////////////////
    static bool hasButton(GamepadId id, Button button);

    ///////////////////////////////////////////////////////////
    /// \brief Check if a gamepad has an axis
    ///
    /// \param id The gamepad ID
    /// \param axis The axis to check
    /// \return True if the gamepad has the axis
    ///
    ///////////////////////////////////////////////////////////
    static bool hasAxis(GamepadId id, Axis axis);

    ///////////////////////////////////////////////////////////
    /// \brief Check if a gamepad button is pressed
    ///
    /// \param id The gamepad ID
    /// \param button The button to check
    /// \return True if the button is pressed
    ///
    ///////////////////////////////////////////////////////////
    static bool isButtonPressed(GamepadId id, Button button);

    ///////////////////////////////////////////////////////////
    /// \brief Get the position of a gamepad axis
    ///
    /// \param id The gamepad ID
    /// \param axis The axis to get
    /// \return The position of the axis
    ///
    ///////////////////////////////////////////////////////////
    static int16_t getAxisPosition(GamepadId id, Axis axis);

private:
    static GamepadHandler s_handler; //!< The handler to dispatch events
};

namespace Event {

    ///////////////////////////////////////////////////////////
    /// \brief Gamepad connection state change
    ///
    ///////////////////////////////////////////////////////////
    struct GamepadConnection {
        GamepadId id;   //!< The gamepad that the button was pressed/released on
        bool connected; //!< Whether the gamepad was connected or disconnected
    };

    ///////////////////////////////////////////////////////////
    /// \brief Gamepad button event
    ///
    ///////////////////////////////////////////////////////////
    struct GamepadButton {
        GamepadId id;           //!< The gamepad that the button was pressed/released on
        Gamepad::Button button; //!< The gamepad button that was pressed/released
        InputAction action;     //!< The action the occured pressed/released
    };

    ///////////////////////////////////////////////////////////
    /// \brief Gamepad axis motion event
    ///
    ///////////////////////////////////////////////////////////
    struct GamepadAxis {
        GamepadId id;       //!< The gamepad that the axis was moved on
        Gamepad::Axis axis; //!< The gamepad axis that was moved
        int16_t value;      //!< The value of the axis
    };

}

///////////////////////////////////////////////////////////
/// \brief Gamepad input handler class
///
///////////////////////////////////////////////////////////
class GamepadHandler : public EventDispatcher<Event::GamepadConnection, Event::GamepadButton, Event::GamepadAxis> {
    friend Input;
};

}