#include <ply/core/Allocate.h>

#include <loguru.hpp>

namespace ply {

///////////////////////////////////////////////////////////
template <typename T>
bool Pool<T>::s_isInitialized = false;

///////////////////////////////////////////////////////////
template <typename T>
typename Pool<T>::PoolWrapper Pool<T>::s_wrapper;

///////////////////////////////////////////////////////////
template <typename T>
inline TypePool<T>::TypePool() : m_pool(sizeof(T)) {
}

///////////////////////////////////////////////////////////
template <typename T>
inline TypePool<T>::TypePool(uint32_t pageSize) : m_pool(sizeof(T), pageSize) {
}

///////////////////////////////////////////////////////////
template <typename T>
inline TypePool<T>::~TypePool() {
    // Invoke all destructors then free memory
    reset();
}

///////////////////////////////////////////////////////////
template <typename T>
inline void TypePool<T>::setPageSize(uint32_t size) {
    m_pool.setPageSize(size);
}

///////////////////////////////////////////////////////////
template <typename T>
inline uint32_t TypePool<T>::getPageSize() const {
    return m_pool.getPageSize();
}

///////////////////////////////////////////////////////////
template <typename T>
inline uint32_t TypePool<T>::getNumObjects() const {
    return m_pool.getNumObjects();
}

///////////////////////////////////////////////////////////
template <typename T>
inline uint32_t TypePool<T>::getNumPages() const {
    return m_pool.getNumPages();
}

///////////////////////////////////////////////////////////
template <typename T>
inline T *TypePool<T>::alloc() {
    // Allocate spce
    T *obj = (T *)m_pool.alloc();
    if (!obj)
        return 0;

    // Default constructor
    new (obj) T();

    return obj;
}

///////////////////////////////////////////////////////////
template <typename T>
inline void TypePool<T>::free(T *ptr) {
    // Make sure there are existing pages first
    if (!m_pool.m_firstPage || !ptr)
        return;

    ObjectPool::PageHeader *header = (ObjectPool::PageHeader *)m_pool.m_firstPage;
    T *page = (T *)(header + 1);

    // Find the page that contains the pointer
    while (header && (ptr < page || ptr > page + m_pool.m_pageSize)) {
        header = (ObjectPool::PageHeader *)header->m_nextPage;
        page = (T *)(header + 1);
    }

    // Error if the object pool doesn't contain the pointer (this shouldn't be allowed to happen)
    CHECK_F(header != NULL, "Tried to free memory that doesn't belong to the object pool");

#ifndef NDEBUG
    // In debug mode, keep track of which slots are being used to prevent double freeing
    uint32_t index = (T *)ptr - (T *)(header + 1);
    CHECK_F(header->m_used[index], "The pointer 0x%08X is being freed from the object pool more than once, this will cause undefined behavior in release builds", ptr);

    header->m_used[index] = false;
#endif

    // Invoke destructor
    ptr->~T();

    // Update free list and number of objects
    *(void **)ptr = header->m_nextFree;
    header->m_nextFree = ptr;
    --header->m_numObjects;
}

///////////////////////////////////////////////////////////
template <typename T>
inline void TypePool<T>::reset() {
    ObjectPool::PageHeader *page = (ObjectPool::PageHeader *)m_pool.m_firstPage;

    // Just free every page
    while (page) {
        ObjectPool::PageHeader *nextPage = (ObjectPool::PageHeader *)page->m_nextPage;

        // Keep track of which objects are free
        std::vector<bool> isFree(m_pool.m_pageSize, false);
        T *pageStart = (T *)(page + 1);

        // Follow the free list until every free slot is visited
        T *current = (T *)page->m_nextFree;
        while (current) {
            // Mark as free
            isFree[current - pageStart] = true;

            // Go to next free
            current = *(T **)current;
        }

        // Loop through the free list and invoke destructor for any that are filled
        for (uint32_t i = 0; i < isFree.size(); ++i) {
            if (!isFree[i])
                pageStart[i].~T();
        }

        // Now free the page
#ifndef NDEBUG
        page->~PageHeader();
#endif
        ALIGNED_FREE_DBG(page);

        page = nextPage;
    }

    // Reset first page pointer
    m_pool.m_firstPage = 0;
}

///////////////////////////////////////////////////////////
template <typename T>
inline Pool<T>::PoolWrapper::PoolWrapper() : m_pool(32) {
    s_isInitialized = true;
}

///////////////////////////////////////////////////////////
template <typename T>
inline Pool<T>::PoolWrapper::~PoolWrapper() {
    s_isInitialized = false;
}

///////////////////////////////////////////////////////////
template <typename T>
inline T *Pool<T>::alloc() {
    // Allocate the object
    if (s_isInitialized) {
        std::unique_lock<std::mutex> lock(s_wrapper.m_mutex);
        return s_wrapper.m_pool.alloc();
    }

    return 0;
}

///////////////////////////////////////////////////////////
template <typename T>
inline void Pool<T>::free(T *ptr) {
    // Free from pool
    if (s_isInitialized) {
        std::unique_lock<std::mutex> lock(s_wrapper.m_mutex);
        s_wrapper.m_pool.free(ptr);
    }
}

///////////////////////////////////////////////////////////
template <typename T>
inline bool Pool<T>::isInitialized() {
    return s_isInitialized;
}

} // namespace ply