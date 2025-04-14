#include <ply/graphics/Image.h>

#include <loguru.hpp>
#include <ply/core/Allocate.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ply {

///////////////////////////////////////////////////////////
Image::Image() :
    m_data(0),
    m_width(0),
    m_height(0),
    m_dataType(Type::Undefined),
    m_numChannels(0),
    m_ownsData(false) {}

///////////////////////////////////////////////////////////
Image::~Image() {
    // Free data if image owns the data
    free();
}

///////////////////////////////////////////////////////////
Image::Image(const std::string& fname, Type dtype) :
    m_data(0),
    m_width(0),
    m_height(0),
    m_dataType(Type::Undefined),
    m_numChannels(0),
    m_ownsData(false) {
    load(fname, dtype);
}

///////////////////////////////////////////////////////////
bool Image::load(const std::string& fname, Type dtype) {
    int x = 0, y = 0, c = 0;
    void* data = 0;

    // Load image based on type
    if (dtype == Type::Uint8)
        data = stbi_load(fname.c_str(), &x, &y, &c, 0);
    else if (dtype == Type::Uint16)
        data = stbi_load_16(fname.c_str(), &x, &y, &c, 0);
    else if (dtype == Type::Float32)
        data = stbi_loadf(fname.c_str(), &x, &y, &c, 0);
    else {
        LOG_F(
            ERROR,
            "Unsupported data type while loading image: %s",
            fname.c_str()
        );
        return false;
    }

    if (!data) {
        LOG_F(ERROR, "Failed to load image: %s", fname.c_str());
        return false;
    }

    m_data = data;
    m_width = x;
    m_height = y;
    m_numChannels = c;
    m_dataType = dtype;
    m_ownsData = true;

    LOG_F(INFO, "Loaded image: %s", fname.c_str());
    return true;
}

///////////////////////////////////////////////////////////
void Image::free() {
    if (m_ownsData && m_data)
        FREE_DBG(m_data);

    m_data = 0;
}

///////////////////////////////////////////////////////////
void Image::create(
    void* data,
    uint32_t w,
    uint32_t h,
    uint32_t c,
    Type dtype,
    bool manage
) {
    // 32-bit colors are not supported
    CHECK_F(
        dtype != Type::Int32 && dtype != Type::Uint32,
        "32-bit image colors are not supported"
    );

    // Free previous data
    free();

    m_data = data;
    m_width = w;
    m_height = h;
    m_numChannels = c;
    m_dataType = dtype;
    m_ownsData = manage;

    // Make sure the data type is an unsigned variant
    if (m_dataType == Type::Int8)
        m_dataType = Type::Uint8;
    else if (m_dataType == Type::Int16)
        m_dataType = Type::Uint16;

    // Create an empty image if data is a null pointer
    if (!m_data) {
        uint32_t typeSize = 1;
        if (m_dataType == Type::Uint16)
            typeSize = 2;
        else if (m_dataType == Type::Float32)
            typeSize = 4;

        // Allocate data
        uint32_t size = m_width * m_height * m_numChannels * typeSize;
        m_data = MALLOC_DBG(size);
        memset(m_data, 0, size);

        // Image owns data
        m_ownsData = true;
    }
}

///////////////////////////////////////////////////////////
void Image::setSize(const Vector2u& size) {
    m_width = size.x;
    m_height = size.y;
}

///////////////////////////////////////////////////////////
void Image::setSize(uint32_t w, uint32_t h) {
    m_width = w;
    m_height = h;
}

///////////////////////////////////////////////////////////
void Image::setDataType(Type dtype) {
    m_dataType = dtype;
}

///////////////////////////////////////////////////////////
void Image::setNumChannels(uint32_t c) {
    m_numChannels = c;
}

///////////////////////////////////////////////////////////
void* Image::getData() const {
    return m_data;
}

///////////////////////////////////////////////////////////
uint32_t Image::getWidth() const {
    return m_width;
}

///////////////////////////////////////////////////////////
uint32_t Image::getHeight() const {
    return m_height;
}

///////////////////////////////////////////////////////////
Type Image::getDataType() const {
    return m_dataType;
}

///////////////////////////////////////////////////////////
uint32_t Image::getNumChannels() const {
    return m_numChannels;
}

///////////////////////////////////////////////////////////
void* Image::getPixel(uint32_t r, uint32_t c) const {
    CHECK_F(
        r < m_height && c < m_width,
        "Requested image pixel row or column is out of bounds: %d, %d",
        r,
        c
    );

    uint32_t typeSize = 1;
    if (m_dataType == Type::Uint16)
        typeSize = 2;
    else if (m_dataType == Type::Float32)
        typeSize = 4;

    return (uint8_t*)m_data + (r * m_width + c) * m_numChannels * typeSize;
}

} // namespace ply