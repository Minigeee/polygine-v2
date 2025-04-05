#pragma once

#include <mutex>
#include <vector>

namespace ply {

template <typename T>
class TypePool;

///////////////////////////////////////////////////////////
/// \brief A pool allocator
///
///////////////////////////////////////////////////////////
class ObjectPool {
    template <typename T>
    friend class TypePool;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty object pool that has an initial object
    /// size of 0
    ///
    /// \see setObjectSize
    ///
    ///////////////////////////////////////////////////////////
    ObjectPool();

    ///////////////////////////////////////////////////////////
    /// \brief Construct and set the object size and page size
    ///
    /// \param objectSize Size of each object slot in bytes
    /// \param pageSize Size of each page in number of objects
    ///
    /// \see setObjectSize
    /// \see setPageSize
    ///
    ///////////////////////////////////////////////////////////
    ObjectPool(uint32_t objectSize, uint32_t pageSize = 512);

#ifndef DOXYGEN_SKIP
    ObjectPool(const ObjectPool &) = delete;
    ObjectPool &operator=(const ObjectPool &) = delete;
    ObjectPool(ObjectPool &&other) noexcept;
    ObjectPool &operator=(ObjectPool &&other) noexcept;
#endif

    ///////////////////////////////////////////////////////////
    /// \brief Cleans up memory used by the object pool
    ///
    ///////////////////////////////////////////////////////////
    ~ObjectPool();

    ///////////////////////////////////////////////////////////
    /// \brief Set the size of each object slot in the object pool
    ///
    /// Each time an allocation is requested, the object pool will
    /// allocate a space of the specified size. Object size has
    /// to be a number greater than or equal to 4 (default 0) to
    /// be valid. All allocations will fail otherwise.
    ///
    /// \param size Size of the object slot in bytes
    ///
    ///////////////////////////////////////////////////////////
    void setObjectSize(uint32_t size);

    ///////////////////////////////////////////////////////////
    /// \brief Set the size of each page in number of objects
    ///
    /// A page is just a static collection of object slots. A page
    /// acts similar to an array, where there is a fixed number of
    /// elements after creation and each element is the same size.
    /// The difference is that there are allowed to be gaps, where
    /// the slots that are actively being used don't have to be
    /// next to each other. If the current page of the object
    /// pool is filled up, the object pool will allocate a new
    /// page of the same size next time an allocation is requested.
    ///
    /// For example, if the page size is set to 100 (default 512),
    /// and there have been 99 allocations, the next allocation
    /// will take the last available slot in the current page.
    /// The next allocation after that won't find any space in
    /// the current page, so the object pool will create a new
    /// page and take space for the allocation from the new page
    /// instead.
    ///
    /// Page size has to be greater than or equal to 1 for
    /// any allocation requests to be valid.
    ///
    /// \param size The size of each page in number of objects
    ///
    ///////////////////////////////////////////////////////////
    void setPageSize(uint32_t size);

    ///////////////////////////////////////////////////////////
    /// \brief Get the object size
    ///
    /// \return The size of each object slot in bytes
    ///
    /// \see setObjectSize
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getObjectSize() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the page size
    ///
    /// \return The size of each page in number of objects
    ///
    /// \see setPageSize
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getPageSize() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of object allocations that have been made
    ///
    /// Every time an allocation is made, the number of objects
    /// will increase by one. This number will decrease every
    /// time an allocated object is freed.
    ///
    /// \return The number of objects in the pool
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getNumObjects() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of pages in the object pool
    ///
    /// Every time a page is filled up, a new page is created
    /// to store allocations in. Pages will continue to exist
    /// after they are created, unless the object pool is reset.
    ///
    /// \return The number of pages that exist
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getNumPages() const;

    ///////////////////////////////////////////////////////////
    /// \brief Allocate a new slot in the object pool
    ///
    /// The amount of space allocated will be equal to the
    /// object size that was set. This function will sometimes
    /// faile and return NULL if invalid parameters are set.
    /// If the object size is less than 4 or if the page size is
    /// less than 1, allocation will fail.
    ///
    /// \return A pointer to the allocated space
    ///
    ///////////////////////////////////////////////////////////
    void *alloc();

    ///////////////////////////////////////////////////////////
    /// \brief Free memory that was previously allocated by the pool
    ///
    /// If the pointer is NULL or if the pointer does not belong
    /// to the pool, nothing will happen to the pool and no exceptions
    /// will be thrown.
    ///
    /// \param ptr The pointer to free from the pool
    ///
    ///////////////////////////////////////////////////////////
    void free(void *ptr);

    ///////////////////////////////////////////////////////////
    /// \brief Reset the object pool
    ///
    /// Does a full reset to its state right after being constructed.
    /// This will free all the memory it is using and will reset
    /// all of its data. This will not invoke any of the destructors
    /// of the objects previously being held in the pool.
    ///
    ///////////////////////////////////////////////////////////
    void reset();

private:
    ///////////////////////////////////////////////////////////
    /// \brief Header structure that holds metadata for each page
    ///
    ///////////////////////////////////////////////////////////
    struct PageHeader {
        void *m_nextPage;      //!< A pointer to the next page
        void *m_nextFree;      //!< A pointer to the next free object slot
        uint32_t m_numObjects; //!< The number of objects currently in the page

#ifndef NDEBUG
        std::vector<bool> m_used; //!< This keeps track of which slots are being used
#endif
    };

    ///////////////////////////////////////////////////////////
    /// \brief Allocate a new page of memory
    ///
    ///////////////////////////////////////////////////////////
    void *allocPage();

private:
    void *m_firstPage;     //!< Pointer to the first page of objects
    uint32_t m_objectSize; //!< Size of each object in bytes
    uint32_t m_pageSize;   //!< Size of each page in number of objects
};

///////////////////////////////////////////////////////////
/// \brief An object pool that holds a single class type
///
///////////////////////////////////////////////////////////
template <typename T>
class TypePool {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    TypePool();

    ///////////////////////////////////////////////////////////
    /// \brief Construct and set the object size and page size
    ///
    /// \param pageSize Size of each page in number of objects
    ///
    /// \see setPageSize
    ///
    ///////////////////////////////////////////////////////////
    TypePool(uint32_t pageSize);

    ///////////////////////////////////////////////////////////
    /// \brief Cleans up memory used by the object pool
    ///
    /// This invokes the destructors of every object still existing
    /// inside the type pool, then frees the pool memory.
    ///
    ///////////////////////////////////////////////////////////
    ~TypePool();

    ///////////////////////////////////////////////////////////
    /// \brief Set the size of each page in number of objects
    ///
    /// A page is just a static collection of object slots. A page
    /// acts similar to an array, where there is a fixed number of
    /// elements after creation and each element is the same size.
    /// The difference is that there are allowed to be gaps, where
    /// the slots that are actively being used don't have to be
    /// next to each other. If the current page of the object
    /// pool is filled up, the object pool will allocate a new
    /// page of the same size next time an allocation is requested.
    ///
    /// For example, if the page size is set to 100 (default 512),
    /// and there have been 99 allocations, the next allocation
    /// will take the last available slot in the current page.
    /// The next allocation after that won't find any space in
    /// the current page, so the object pool will create a new
    /// page and take space for the allocation from the new page
    /// instead.
    ///
    /// Page size has to be greater than or equal to 1 for
    /// any allocation requests to be valid.
    ///
    /// \param size The size of each page in number of objects
    ///
    ///////////////////////////////////////////////////////////
    void setPageSize(uint32_t size);

    ///////////////////////////////////////////////////////////
    /// \brief Get the page size
    ///
    /// \return The size of each page in number of objects
    ///
    /// \see setPageSize
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getPageSize() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of object allocations that have been made
    ///
    /// Every time an allocation is made, the number of objects
    /// will increase by one. This number will decrease every
    /// time an allocated object is freed.
    ///
    /// \return The number of objects in the pool
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getNumObjects() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of pages in the object pool
    ///
    /// Every time a page is filled up, a new page is created
    /// to store allocations in. Pages will continue to exist
    /// after they are created, unless the object pool is reset.
    ///
    /// \return The number of pages that exist
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getNumPages() const;

    ///////////////////////////////////////////////////////////
    /// \brief Allocate a new slot in the object pool
    ///
    /// The amount of space allocated will be equal to the
    /// object size that was set. This function will sometimes
    /// faile and return NULL if invalid parameters are set.
    /// If the object size is less than 4 or if the page size is
    /// less than 1, allocation will fail.
    ///
    /// \return A pointer to the allocated space
    ///
    ///////////////////////////////////////////////////////////
    T *alloc();

    ///////////////////////////////////////////////////////////
    /// \brief Free memory that was previously allocated by the pool
    ///
    /// If the pointer is NULL or if the pointer does not belong
    /// to the pool, nothing will happen to the pool and no exceptions
    /// will be thrown.
    ///
    /// This function will automatically call the object's destructor.
    ///
    /// \param ptr The pointer to free from the pool
    ///
    ///////////////////////////////////////////////////////////
    void free(T *ptr);

    ///////////////////////////////////////////////////////////
    /// \brief Reset the object pool
    ///
    /// Does a full reset to its state right after being constructed.
    /// This will free all the memory it is using and will reset
    /// all of its data. This will invoke the destructors of all
    /// remaining objects being held in the pool, as the type of
    /// object is known.
    ///
    ///////////////////////////////////////////////////////////
    void reset();

private:
    ObjectPool m_pool;
};

///////////////////////////////////////////////////////////
/// \brief A global object pool
///
///////////////////////////////////////////////////////////
template <typename T>
class Pool {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Allocate space for an object and call its default constructor
    ///
    /// All objects allocated with this function should be released
    /// with free() after done using so the object deconstructor can
    /// be called.
    ///
    /// \return A pointer to the allocated object
    ///
    ///////////////////////////////////////////////////////////
    static T *alloc();

    ///////////////////////////////////////////////////////////
    /// \brief Free the object from memory and call its destructor
    ///
    /// Failing to call this function may lead to memory leaks if
    /// the object type has a destructor that manages its own memory
    /// allocations.
    ///
    /// \param ptr A pointer to the object
    ///
    ///////////////////////////////////////////////////////////
    static void free(T *ptr);

    ///////////////////////////////////////////////////////////
    /// \brief Check if the object pool has been initialized
    ///
    /// This will return false before the object pool has been
    /// constructed, and after the object pool has been destroyed.
    ///
    /// \return True if the object pool is initialized and valid to use
    ///
    ///////////////////////////////////////////////////////////
    static bool isInitialized();

private:
    struct PoolWrapper {
        PoolWrapper();

        ~PoolWrapper();

        TypePool<T> m_pool;
        std::mutex m_mutex;
    };

private:
    static bool s_isInitialized;
    static PoolWrapper s_wrapper;
};

} // namespace ply

#include <ply/core/PoolAllocator.inl>

///////////////////////////////////////////////////////////
/// \class poly::ObjectPool
/// \ingroup Core
///
/// The object pool is a pool allocator that can hold a collection
/// objects of the same size. Its purpose is to increase allocation
/// speed if many object allocations are required, and to minimize
/// memory fragmentation. The object pool should be used in
/// place of the new operator because of the reasons stated.
///
/// The object pool internally creates a space where many objects
/// of the same size can reserve, and because every slot is the
/// the same size, no fragmentation can occur. If a large number
/// of objects is required, the object pool will allocate more
/// "pages" to store these objects. A page is just a static
/// collection of object slots. They will not change size or
/// move in memory once they are created. With the use of pages,
/// object pools can grow to however big is needed.
///
/// Usage example:
/// \code
/// using namespace poly;
///
/// // Create a pool for objects that are 8 bytes, and make each page 100 objects
/// ObjectPool pool(8, 100);
///
/// // Allocate space for a double
/// double* pi = (double*)pool.alloc();
/// *pi = 3.1415;
///
/// std::cout << pool.getNumObjects() << "\n";  // 1 object
/// std::cout << pool.getNumPages() << "\n";    // 1 page
///
/// for (int i = 0; i < 100; ++i)
///     pool.alloc();
///
/// std::cout << pool.getNumObjects() << "\n";  // 101 objects
/// std::cout << pool.getNumPages() << "\n";    // 2 pages
///
/// // Free the first object
/// pool.free(pi);
///
/// // Reset the object pool
/// pool.reset();
///
/// \endcode
///
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \class poly::TypePool
/// \ingroup Core
///
/// The typed object pool is a pool allocator that can hold a collection
/// objects of the same size. Its purpose is to increase allocation
/// speed if many object allocations are required, and to minimize
/// memory fragmentation. The typed object pool is the same as a
/// normal object pool, except that it can handle object creation
/// and destruction. This means that free() and reset() will invoke
/// the object destructors.
///
/// All functionality of the typed object pool is the same as
/// the regular object pool, so please see ObjectPool for
/// a more in depth description and usage example.
///
///////////////////////////////////////////////////////////