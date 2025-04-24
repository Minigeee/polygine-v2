#pragma once

namespace ply {

namespace Mouse {

    ////////////////////////////////////////////////////////////
    /// \brief Mouse buttons
    ///
    ////////////////////////////////////////////////////////////
    enum class Button {
        Left = 1, //!< The left mouse button
        Middle,   //!< The middle (wheel) mouse button
        Right,    //!< The right mouse button
        Extra1,   //!< The first extra mouse button
        Extra2    //!< The second extra mouse button
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse wheels
    ///
    ////////////////////////////////////////////////////////////
    enum class Wheel {
        Vertical,  //!< The vertical mouse wheel
        Horizontal //!< The horizontal mouse wheel
    };

} // namespace Mouse

using MouseButton = Mouse::Button; //!< Alias for Mouse::Button
using MouseWheel = Mouse::Wheel;   //!< Alias for Mouse::Wheel

} // namespace ply