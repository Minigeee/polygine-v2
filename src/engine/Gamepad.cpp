#include <ply/engine/Gamepad.h>

#include <SDL3/SDL.h>

namespace ply {

///////////////////////////////////////////////////////////
GamepadHandler Gamepad::s_handler;

///////////////////////////////////////////////////////////
bool Gamepad::enable() {
    bool success = SDL_InitSubSystem(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK);

    return success;
}

///////////////////////////////////////////////////////////
GamepadHandler& Gamepad::getHandler() {
    return s_handler;
}

///////////////////////////////////////////////////////////
std::vector<GamepadId> Gamepad::getDevices() {
    int count = 0;
    void* ids = SDL_GetGamepads(&count);
    return std::vector<GamepadId>((GamepadId*)ids, (GamepadId*)ids + count);
}

///////////////////////////////////////////////////////////
bool Gamepad::isConnected(GamepadId id) {
    return SDL_GamepadConnected(SDL_GetGamepadFromID(id));
}

///////////////////////////////////////////////////////////
bool Gamepad::hasButton(GamepadId id, Gamepad::Button button) {
    return SDL_GamepadHasButton(
        SDL_GetGamepadFromID(id), (SDL_GamepadButton)button);
}

///////////////////////////////////////////////////////////
bool Gamepad::hasAxis(GamepadId id, Gamepad::Axis axis) {
    return SDL_GamepadHasAxis(SDL_GetGamepadFromID(id), (SDL_GamepadAxis)axis);
}

///////////////////////////////////////////////////////////
bool Gamepad::isButtonPressed(GamepadId id, Gamepad::Button button) {
    return SDL_GetGamepadButton(
        SDL_GetGamepadFromID(id), (SDL_GamepadButton)button);
}

///////////////////////////////////////////////////////////
int16_t Gamepad::getAxisPosition(GamepadId id, Gamepad::Axis axis) {
    return SDL_GetGamepadAxis(SDL_GetGamepadFromID(id), (SDL_GamepadAxis)axis);
}

} // namespace ply