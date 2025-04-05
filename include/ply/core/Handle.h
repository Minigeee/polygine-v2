#pragma once

#include <cstdint>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief The structure used to access elements in a handle array
///
///////////////////////////////////////////////////////////
struct Handle {
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    Handle();

    ///////////////////////////////////////////////////////////
    /// \brief Construct a handle with an index and a counter
    ///
    ///////////////////////////////////////////////////////////
    Handle(uint32_t index, uint32_t counter = 0);

    operator uint32_t() const;

    uint32_t m_index : 24;   //!< Index of handle, used to access correct element
    uint32_t m_counter : 8;  //!< Counter used to ensure the handled element hasn't been removed
};

}  // namespace ply
