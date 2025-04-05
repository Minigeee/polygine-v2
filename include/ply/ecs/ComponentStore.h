#pragma once

#include <cstdint>

namespace ply {

namespace priv {
///////////////////////////////////////////////////////////
/// \brief Array similar to std::vector but untyped
///
///////////////////////////////////////////////////////////
class ComponentStore {
   public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    ComponentStore();

    ///////////////////////////////////////////////////////////
    /// \brief Constructor that sets type size
    ///
    ///////////////////////////////////////////////////////////
    ComponentStore(size_t typeSize, size_t typeAlign);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor frees allocated memory
    ///////////////////////////////////////////////////////////
    ~ComponentStore();

#ifndef DOXYGEN_SKIP
    ComponentStore(const ComponentStore&);
    ComponentStore& operator=(const ComponentStore&);
    ComponentStore(ComponentStore&& other) noexcept;
    ComponentStore& operator=(ComponentStore&& other) noexcept;
#endif

    ///////////////////////////////////////////////////////////
    /// \brief Add data to the array, by repeating the given data the given number of times
    ///
    /// Adds data using memcpy(), so types with custom copy constructors
    /// are not allowed.
    ///
    /// \param data A pointer to the element to add to the array
    /// \param instances The number of times to repeat the given element
    ///
    /// \return A pointer to the beggining of the added data
    ///
    ///////////////////////////////////////////////////////////
    void* push(void* data, size_t instances);

    ///////////////////////////////////////////////////////////
    /// \brief Remove the element at the given index using a swap-pop
    ///
    /// Frees data using memcpy() to copy the last element into the specified
    /// index, so types with custom destructors are not allowed.
    ///
    /// \param index The index of the element to remove
    ///
    ///////////////////////////////////////////////////////////
    void remove(size_t index);

    ///////////////////////////////////////////////////////////
    /// \brief Get pointer to data at the specified index
    ///
    /// \param index The index of the element to get
    ///
    /// \return A pointer to the data
    ///
    ///////////////////////////////////////////////////////////
    void* data(size_t index = 0) const;

    ///////////////////////////////////////////////////////////
    /// \brief Reserve a certain amount of space in array
    ///
    /// \param size The number of elements to reserve space for
    ///
    ///////////////////////////////////////////////////////////
    void reserve(size_t size);

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of elements in array
    ///
    ///////////////////////////////////////////////////////////
    size_t size() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the size of elements in array
    ///
    ///////////////////////////////////////////////////////////
    size_t getTypeSize() const;

   private:
    uint8_t* m_start;      //!< Start of array
    uint8_t* m_last;       //!< Last element in array
    uint8_t* m_end;        //!< End of reserved memory
    size_t m_typeSize;   //!< Size of type this array holds
    size_t m_typeAlign;  //!< ALign of type this array holds
};
}  // namespace priv

}  // namespace ply
