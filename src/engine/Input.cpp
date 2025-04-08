#include <ply/engine/Input.h>

#include <ply/engine/Gamepad.h>
#include <ply/engine/Window.h>

#include <SDL3/SDL.h>
#include <loguru.hpp>

namespace ply {

void Input::poll() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
            Window* window = Window::s_windows[e.window.windowID];
            window->m_shouldClose = true;
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED:
            break;

        // Mouse
        case SDL_EVENT_MOUSE_MOTION: {
            Window* window = Window::s_windows[e.motion.windowID];
            window->sendEvent<Event::MouseMove>(
                {.x = e.motion.x, .y = e.motion.y, .dx = e.motion.xrel, .dy = e.motion.yrel}
            );
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            Window* window = Window::s_windows[e.button.windowID];
            window->sendEvent<Event::MouseButton>(
                {.button = (Mouse::Button)e.button.button,
                 .action = InputAction::Press,
                 .x = e.button.x,
                 .y = e.button.y}
            );
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            Window* window = Window::s_windows[e.button.windowID];
            window->sendEvent<Event::MouseButton>(
                {.button = (Mouse::Button)e.button.button,
                 .action = InputAction::Release,
                 .x = e.button.x,
                 .y = e.button.y}
            );
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            Window* window = Window::s_windows[e.wheel.windowID];
            window->sendEvent<Event::MouseScroll>({.dx = e.wheel.x, .dy = e.wheel.y});
            break;
        }

        // Keyboard
        case SDL_EVENT_KEY_DOWN: {
            Window* window = Window::s_windows[e.key.windowID];
            window->sendEvent<Event::Key>(
                {.key = (Keyboard::Key)e.key.key,
                 .scan = (Keyboard::Scancode)e.key.scancode,
                 .action = e.key.repeat ? InputAction::Repeat : InputAction::Press}
            );
            break;
        }
        case SDL_EVENT_KEY_UP: {
            Window* window = Window::s_windows[e.key.windowID];
            window->sendEvent<Event::Key>(
                {.key = (Keyboard::Key)e.key.key,
                 .scan = (Keyboard::Scancode)e.key.scancode,
                 .action = InputAction::Release}
            );
            break;
        }

        // Gamepad
        case SDL_EVENT_GAMEPAD_ADDED: {
            // Try to open gamepad
            SDL_Gamepad* gamepad = SDL_OpenGamepad(e.gdevice.which);
            if (!gamepad) {
                LOG_F(ERROR, "Failed to open gamepad %d", e.gdevice.which);
                break;
            }

            Gamepad::getHandler().sendEvent<Event::GamepadConnection>(
                {.id = e.gdevice.which, .connected = true}
            );
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            Gamepad::getHandler().sendEvent<Event::GamepadConnection>(
                {.id = e.gdevice.which, .connected = false}
            );

            // Close gamepad
            SDL_CloseGamepad(SDL_GetGamepadFromID(e.gdevice.which));
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
            Gamepad::getHandler().sendEvent<Event::GamepadButton>(
                {.id = e.gbutton.which,
                 .button = (Gamepad::Button)e.gbutton.button,
                 .action = InputAction::Press}
            );
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_UP: {
            Gamepad::getHandler().sendEvent<Event::GamepadButton>(
                {.id = e.gbutton.which,
                 .button = (Gamepad::Button)e.gbutton.button,
                 .action = InputAction::Release}
            );
            break;
        }
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            Gamepad::getHandler().sendEvent<Event::GamepadAxis>(
                {.id = e.gaxis.which, .axis = (Gamepad::Axis)e.gaxis.axis, .value = e.gaxis.value}
            );
            break;
        }

        default:
            break;
        }
    }
}

bool Input::isKeyPressed(Keyboard::Scancode key) {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    return keys[(SDL_Keycode)key];
}

bool Input::isMousePressed(Mouse::Button button) {
    uint32_t state = SDL_GetMouseState(nullptr, nullptr);
    return (bool)(SDL_BUTTON_MASK((uint32_t)button) & state);
}

Vector2f Input::getMousePosition() {
    Vector2f pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

} // namespace ply
