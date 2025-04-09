#pragma once

namespace ply {

class RenderDevice;

namespace priv {
    struct DeviceImpl;
}

class GpuResource {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    GpuResource();

protected:
    GpuResource(RenderDevice* device);
    GpuResource(priv::DeviceImpl* device);

    priv::DeviceImpl* m_device; //!< Device state
};

} // namespace ply

#define GPU_RESOURCE(X, impl)          \
    X(const X&) = delete;              \
    X& operator=(const X&) = delete;   \
    X(X&& other) noexcept :            \
        impl(other.impl) {             \
        other.impl = nullptr;          \
    }                                  \
    X& operator=(X&& other) noexcept { \
        impl = other.impl;             \
        other.impl = nullptr;          \
        return *this;                  \
    }