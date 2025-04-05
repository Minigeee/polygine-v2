#pragma once

#include <ply/core/Handle.h>

#include <cstdint>
#include <vector>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief An array that is accessed by handles instead of by index
///
///////////////////////////////////////////////////////////
template <typename T>
class HandleArray {
   public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Initializes all internal arrays with no size
    ///
    ///////////////////////////////////////////////////////////
    HandleArray();

    ///////////////////////////////////////////////////////////
    /// \brief Construct with a certain amount of reserved space
    ///
    /// \param size Amount of objects to reserve space for
    ///
    ///////////////////////////////////////////////////////////
    HandleArray(uint32_t size);

    ///////////////////////////////////////////////////////////
    /// \brief Access the array by handle
    ///
    /// Access the array, similar to std::vector, but by handle.
    /// The will throw an exception if an invalid handle is used.
    /// If the element that is being referenced by the handle
    /// has been removed, the accessor will fail and throw an exception.
    ///
    /// \return Referenced to the element referenced by the handle
    ///
    ///////////////////////////////////////////////////////////
    T& operator[](Handle handle);

    ///////////////////////////////////////////////////////////
    /// \brief Add an element to the array and get its handle
    ///
    /// Elements that are added to the array will be kept in a
    /// internal contiguous array and are accessed by the returned
    /// handle. The handle returned will always be valid until the
    /// element being referenced by the handle is removed.
    ///
    /// \param element The element to add
    ///
    /// \return Handle used to access the added element
    ///
    /// \see remove
    ///
    ///////////////////////////////////////////////////////////
    Handle push(const T& element);

    ///////////////////////////////////////////////////////////
    /// \brief Add an element to the array and get its handle
    ///
    /// Elements that are added to the array will be kept in a
    /// internal contiguous array and are accessed by the returned
    /// handle. The handle returned will always be valid until the
    /// element being referenced by the handle is removed.
    ///
    /// \param element The element to add
    ///
    /// \return Handle used to access the added element
    ///
    /// \see remove
    ///
    ///////////////////////////////////////////////////////////
    Handle push(T&& element);

    ///////////////////////////////////////////////////////////
    /// \brief Remove the element being referenced by the handle
    ///
    /// When removing an element, the internal array used to store
    /// the element keeps all its data contiguous. The element is
    /// removed using swap-pop removal, so removal requires a single
    /// swap no matter how large the array may be. Even when the
    /// elements shift around in memory, the element's handle will
    /// always point to the correct element.
    ///
    /// When an element is removed, any following attempts to
    /// access the element using its handle will fail.
    ///
    /// \param handle Handle of the element to remove
    ///
    /// \see add
    ///
    ///////////////////////////////////////////////////////////
    void remove(Handle handle);

    ///////////////////////////////////////////////////////////
    /// \brief Completely reset the handle array
    ///
    /// Resets the array to its state after the default constructor.
    /// This will invoke the destructors of any element that was
    /// being stored in the array.
    ///
    ///////////////////////////////////////////////////////////
    void reset();

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of elements in the array
    ///
    /// \return Number of elements
    ///
    ///////////////////////////////////////////////////////////
    uint32_t size() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the amount of reserved memory in number of elements
    ///
    /// \return Amount of reserved space
    ///
    ///////////////////////////////////////////////////////////
    uint32_t capacity() const;

    ///////////////////////////////////////////////////////////
    /// \brief See if the array is empty
    ///
    /// \return True if the array is empty
    ///
    ///////////////////////////////////////////////////////////
    bool isEmpty() const;

    ///////////////////////////////////////////////////////////
    /// \brief Check if a handle is valid
    ///
    /// A handle is invalid when its index is out of bounds, or
    /// when the handle counter does not match its entry counter.
    ///
    /// \param handle The handle to check validity of
    ///
    /// \return True if the handle is valid
    ///
    ///////////////////////////////////////////////////////////
    bool isValid(Handle handle) const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the contiguous internal array
    ///
    /// Data in this array is always kept contiguous. However,
    /// its contents are not guarenteed to be in the same order
    /// they were added to the array.
    ///
    /// \return The internal array used to store the elements
    ///
    ///////////////////////////////////////////////////////////
    std::vector<T>& data();

    ///////////////////////////////////////////////////////////
    /// \brief Get the contiguous internal array
    ///
    /// Data in this array is always kept contiguous. However,
    /// its contents are not guarenteed to be in the same order
    /// they were added to the array.
    ///
    /// \return The internal array used to store the elements
    ///
    ///////////////////////////////////////////////////////////
    const std::vector<T>& data() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the internal index of a handle
    ///
    /// This function is not needed in most cases, but it can
    /// be useful in cases where many arrays depend on one handled
    /// array to maintain the order of the elements.
    ///
    /// It basically provides a mapping between a handle and
    /// its internal corresponding index.
    ///
    /// \note An index of 0xFFFF will be returned if the handle is invalid
    ///
    /// \param handle A handle to retrieve an index
    ///
    /// \return The internal index
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getIndex(Handle handle) const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the handle corresponding to an internal index
    ///
    /// This function is not needed in most cases, but it can
    /// be useful in cases where a handle of an object needs to
    /// found again.
    ///
    /// It basically provides a mapping between an internal index
    /// and its corresponding handle.
    ///
    /// \note An empty handle will be returned if the index is out of bounds
    ///
    /// \param index An index to retrieve a handle for
    ///
    /// \return The corresponding handle
    ///
    ///////////////////////////////////////////////////////////
    Handle getHandle(uint32_t index) const;

   private:
    std::vector<T> m_data;                 //!< Internal data array
    std::vector<Handle> m_handleToData;    //!< Maps handle index to actual index, also keeps a counter to detect invalid handles
    std::vector<uint32_t> m_dataToHandle;  //!< Maps actual index to handle index
    uint32_t m_nextFree;                   //!< Index of the next free handle
};

}  // namespace ply

#include <ply/core/HandleArray.inl>

///////////////////////////////////////////////////////////
/// \class ply::HandleArray
/// \ingroup Core
///
/// A data container that accesses elements by handles instead
/// of by index. The elements that are added to the array will
/// be kept in a internal contiguous array, and all elements
/// in this internal array will be kept contiguous, even if some
/// elements are removed. The main purpose of the handle array
/// is to have a way to consistently access elements in an
/// array that keeps its elements in contiguous storage. In
/// a normal array, if an element is removed, then all other
/// elements that came after the one that was removed has to be
/// shifted to fill in the empty slot. The problem with shifting
/// the elements is that the indices that were used to access them
/// before will become invalid.
///
/// For example:
/// \code
/// std::vector<int> v;
/// v.push_back(3);
/// v.push_back(1);
/// v.push_back(4);
/// v.push_back(1);
/// v.push_back(5);
///
/// // For whatever reason, we would like to keep a reference to
/// // the element "4", so we store the index it is at
/// int index = 2;
///
/// std::cout << v[index] << "\n"; // Prints "4"
///
/// // If we remove an element that comes before, the index that
/// // we stored will become invalid
/// v.erase(v.begin());
///
/// // The index that we stored is now invalid, it no longer points
/// // to the correct element
/// std::cout << v[index] << "\n"; // Prints "1"
///
/// \endcode
///
/// This is a very basic example, and we would have no use for storing
/// the index, but it serves as an example.
/// By using a handle array, elements could be removed and they could
/// kept contiguous in memory, but the handles used to access them
/// would still be valid.
///
/// \code
/// using namespace ply;
///
/// HandleArray<int> a;
/// Handle h1 = a.push(3);
/// Handle h2 = a.push(1);
/// Handle h3 = a.push(4);
/// Handle h4 = a.push(1);
/// Handle h5 = a.push(5);
///
/// std::cout << a[h3] << "\n"; // Prints "4"
///
/// // Remove the first element
/// a.remove(h1);
///
/// std::cout << a[h3] << "\n"; // Still prints "4"
///
/// int removedValue = a[h1]; // This would throw an exception because h1 was removed
///
/// \endcode
///
///////////////////////////////////////////////////////////