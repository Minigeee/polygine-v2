#define POLY_ALLOC_IMPLEMENTATION
#include <ply/core/Allocate.h>

#include <iostream>
#include <loguru.hpp>
#include <string>

namespace ply {

namespace priv {

///////////////////////////////////////////////////////////
Allocate* g_allocate = 0;

///////////////////////////////////////////////////////////
uint32_t AllocateInit::s_refCount = 0;

///////////////////////////////////////////////////////////
Allocate::Allocate() : m_test(1) {
}

///////////////////////////////////////////////////////////
Allocate::~Allocate() {
    if (m_data.size()) {
        fprintf(stderr, "\nFound %d memory leaks\n", m_data.size());
        fprintf(stderr, "Location                             Address      Size\n");
        fprintf(stderr, "===========================================================\n");
    }

    // Log the rest of the remaining allocated segments
    std::vector<AllocData> leaks;
    for (auto it = m_data.begin(); it != m_data.end(); ++it)
        leaks.push_back(it->second);

    // Sort by location
    std::sort(leaks.begin(), leaks.end(),
              [](const AllocData& a, const AllocData& b) -> bool {
                  return a.m_info < b.m_info;
              });

    // Print each leak
    for (uint32_t i = 0; i < leaks.size(); ++i) {
        AllocData& data = leaks[i];

        if (data.m_info.size() > 32) {
            uint32_t lineLen = data.m_info.size() - data.m_info.find(':');
            data.m_info = data.m_info.substr(0, 32 - 3 - lineLen) + "..." + data.m_info.substr(data.m_info.size() - lineLen);
        }

        fprintf(stderr, "%-36s 0x%08X   %d\n", data.m_info.c_str(), data.m_address, data.m_size);
    }
}

///////////////////////////////////////////////////////////
void* Allocate::alloc(uint32_t size, const std::string& file, uint32_t line) {
    // Allocate
    void* ptr = ::malloc(size);
    if (!ptr) return 0;

    std::string loc = file + ':' + std::to_string(line);
    uint32_t pos = loc.find_last_of("/\\");
    if (pos != std::string::npos)
        loc = loc.substr(pos + 1, loc.size() - pos - 1);

    m_data[ptr] = AllocData{loc, ptr, size};

    LOG_F(1, "allocated block: %d bytes (%s)", size, loc.c_str());

    return ptr;
}

///////////////////////////////////////////////////////////
void Allocate::free(void* ptr) {
    // Free
    ::free(ptr);

    auto it = m_data.find(ptr);
    if (it != m_data.end())
        m_data.erase(it);
}

///////////////////////////////////////////////////////////
void* Allocate::alignedAlloc(uint32_t size, uint32_t align, const std::string& file, uint32_t line) {
    // Allocate
#ifdef _WIN32
    void* ptr = ::_aligned_malloc(size, align);
#else
    void* ptr = ::aligned_malloc(size, align);
#endif
    if (!ptr) return 0;

    std::string loc = file + ':' + std::to_string(line);
    uint32_t pos = loc.find_last_of("/\\");
    if (pos != std::string::npos)
        loc = loc.substr(pos + 1, loc.size() - pos - 1);

    m_data[ptr] = AllocData{loc, ptr, size};

    LOG_F(1, "allocated aligned block: %d bytes (%s)", size, loc.c_str());

    return ptr;
}

///////////////////////////////////////////////////////////
void Allocate::alignedFree(void* ptr) {
    // Free
#ifdef _WIN32
    ::_aligned_free(ptr);
#else
    ::aligned_free(ptr);
#endif

    auto it = m_data.find(ptr);
    if (it != m_data.end())
        m_data.erase(it);
}

///////////////////////////////////////////////////////////
AllocateInit::AllocateInit() {
    if (++s_refCount == 1)
        g_allocate = new Allocate();
}

///////////////////////////////////////////////////////////
AllocateInit::~AllocateInit() {
    if (--s_refCount == 0)
        delete g_allocate;
}

}  // namespace priv

}  // namespace ply
