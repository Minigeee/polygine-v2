#pragma once

#include <ply/math/Frustum.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief A camera provides the a perspective to view the world from
///
///////////////////////////////////////////////////////////
class Camera {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// The camera starts at position (0, 0, 0) facing in the
    /// negative Z direction.
    ///
    ///////////////////////////////////////////////////////////
    Camera();

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~Camera();

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera position
    ///
    /// \param pos Camera position along the x, y, and z direction
    ///
    ///////////////////////////////////////////////////////////
    void setPosition(const Vector3f& pos);

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera position
    ///
    /// \param x Position along the x axis
    /// \param y Position along the y axis
    /// \param z Position along the z axis
    ///
    ///////////////////////////////////////////////////////////
    void setPosition(float x, float y, float z);

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera direction
    ///
    /// The direction vector does not have to be normalized
    ///
    /// \param dir The direction vector
    ///
    ///////////////////////////////////////////////////////////
    void setDirection(const Vector3f& dir);

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera direction
    ///
    /// The direction vector does not have to be normalized
    ///
    /// \param x The x component of the direction vector
    /// \param y The y component of the direction vector
    /// \param z The z component of the direction vector
    ///
    ///////////////////////////////////////////////////////////
    void setDirection(float x, float y, float z);

    ///////////////////////////////////////////////////////////
    /// \brief Set camera rotation along the x and y axis
    ///
    /// Rotation along the z axis (roll) is not supported yet
    ///
    /// \param rotation The rotation along x and y axis
    ///
    ///////////////////////////////////////////////////////////
    void setRotation(const Vector2f& rotation);

    ///////////////////////////////////////////////////////////
    /// \brief Set camera rotation along the x and y axis
    ///
    /// Rotation along the z axis (roll) is not supported yet
    ///
    /// \param x Rotation on the x axis
    /// \param y Rotation on the y axis
    ///
    ///////////////////////////////////////////////////////////
    void setRotation(float x, float y);

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera zoom factor
    ///
    /// The zoom factor scales the FOV to make objects appear zoomed
    ///
    /// \param zoom The zoom factor
    ///
    ///////////////////////////////////////////////////////////
    void setZoom(float zoom);

    ///////////////////////////////////////////////////////////
    /// \brief Move the camera in world space relative to the world origin
    ///
    /// \brief offset The offset to move the camera in world space
    ///
    ///////////////////////////////////////////////////////////
    void move(const Vector3f& offset);

    ///////////////////////////////////////////////////////////
    /// \brief Move the camera in world space relative to the world origin
    ///
    /// \brief x The x offset to move the camera in world space
    /// \brief y The y offset to move the camera in world space
    /// \brief z The z offset to move the camera in world space
    ///
    ///////////////////////////////////////////////////////////
    void move(float x, float y, float z);

    ///////////////////////////////////////////////////////////
    /// \brief Scale the zoom factor
    ///
    /// \brief The amount to scale the zoom factor by
    ///
    ///////////////////////////////////////////////////////////
    void zoom(float zoom);

    ///////////////////////////////////////////////////////////
    /// \brief Get the camera position in world space relative to the world
    /// origin
    ///
    /// \return The camera position in world space
    ///
    ///////////////////////////////////////////////////////////
    const Vector3f& getPosition() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the camera direction
    ///
    /// The camera direction is a normalized vector that points
    /// in the direction the camera is facing.
    ///
    /// \return The camera direction
    ///
    ///////////////////////////////////////////////////////////
    const Vector3f& getDirection() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the camera right direction vector
    ///
    /// The right vector points at a 90 degree angle to the right
    /// of the direction the camera is facing, and it has 0 y component.
    ///
    /// \return The camera right direction vector
    ///
    ///////////////////////////////////////////////////////////
    const Vector3f& getRightDir() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the camera zoom factor
    ///
    /// The zoom factor scales the camera fov to make objects appear
    /// bigger or smaller.
    ///
    /// \return The camera zoom factor
    ///
    ///////////////////////////////////////////////////////////
    float getZoom() const;

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera perspective parameters
    ///
    /// This specifies the parameters needed to create a perspective
    /// projection matrix, including field of view, aspect ratio,
    /// distance to the near plane, and distance to the far plane.
    ///
    /// \param fov The field of view along the x axis in degrees
    /// \param ar The aspect ratio
    /// \param near The distance to the near plane
    /// \param far The distance to the far plane
    ///
    ///////////////////////////////////////////////////////////
    void setPerspective(float fov, float ar, float near, float far);

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera field of view
    ///
    /// \param The field view along the x axis in degrees
    ///
    ///////////////////////////////////////////////////////////
    void setFov(float fov);

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera aspect ratio
    ///
    /// \param ar The camera aspect ratio
    ///
    ///////////////////////////////////////////////////////////
    void setAspectRatio(float ar);

    ///////////////////////////////////////////////////////////
    /// \brief Set the distance to the near plane
    ///
    /// \param near The distance to the near plane
    ///
    ///////////////////////////////////////////////////////////
    void setNear(float near);

    ///////////////////////////////////////////////////////////
    /// \brief Set the distance to the far plane
    ///
    /// \param far The distance to the far plane
    ///
    ///////////////////////////////////////////////////////////
    void setFar(float far);

    ///////////////////////////////////////////////////////////
    /// \brief Set the camera orthographic parameters
    ///
    /// This specifies the parameters needed to create an orthographic
    /// projection matrix, including the left, right, bottom, top, near,
    /// and far planes.
    ///
    /// \param left The distance from the camera to the left orthographic plane
    /// \param right The distance from the camera to the right orthographic
    /// plane
    /// \param bottom The distance from the camera to the bottom orthographic
    /// plane
    /// \param top The distance from the camera to the top orthographic plane
    /// \param near The distance from the camera to the near orthographic plane
    /// \param far The distance from the camera to the far orthographic plane
    ///
    ///////////////////////////////////////////////////////////
    void setOrthographic(
        float left,
        float right,
        float bottom,
        float top,
        float near,
        float far
    );

    ///////////////////////////////////////////////////////////
    /// \brief Set the distance to the left orthographic plane
    ///
    /// \param left The distance from the camera to the left orthographic plane
    ///
    ///////////////////////////////////////////////////////////
    void setLeft(float left);

    ///////////////////////////////////////////////////////////
    /// \brief Set the distance to the right orthographic plane
    ///
    /// \param right The distance from the camera to the right orthographic
    /// plane
    ///
    ///////////////////////////////////////////////////////////
    void setRight(float right);

    ///////////////////////////////////////////////////////////
    /// \brief Set the distance to the bottom orthographic plane
    ///
    /// \param bottom The distance from the camera to the bottom orthographic
    /// plane
    ///
    ///////////////////////////////////////////////////////////
    void setBottom(float bottom);

    ///////////////////////////////////////////////////////////
    /// \brief Set the distance to the top orthographic plane
    ///
    /// \param top The distance from the camera to the top orthographic plane
    ///
    ///////////////////////////////////////////////////////////
    void setTop(float top);

    ///////////////////////////////////////////////////////////
    /// \brief Get the projection matrix
    ///
    /// Every time one of the projection parameters is changed,
    /// the projection matrix will be recalculated and cached.
    ///
    /// \return The projection matrix
    ///
    ///////////////////////////////////////////////////////////
    const Matrix4f& getProjMatrix();

    ///////////////////////////////////////////////////////////
    /// \brief Get the view matrix
    ///
    /// Every time one of the view parameters is changed (position,
    /// direction, or zoom), the view matrix is recalculated and cached.
    ///
    /// \return The view matrix
    ///
    ///////////////////////////////////////////////////////////
    const Matrix4f& getViewMatrix();

    ///////////////////////////////////////////////////////////
    /// \brief Get the camera frustum
    ///
    /// Every time one of the view parameters is changed (position,
    /// direction, or zoom), or one of the projection parameters is
    /// changed, the frustum will be recalculated and cached in this
    /// function call.
    ///
    /// \return The camera frustum
    ///
    ///////////////////////////////////////////////////////////
    const Frustum& getFrustum();

    ///////////////////////////////////////////////////////////
    /// \brief Get the camera field of view
    ///
    /// \return The field of view along the x axis in degrees
    ///
    ///////////////////////////////////////////////////////////
    float getFov() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the camera aspect ratio
    ///
    /// \return The camera aspect ratio
    ///
    ///////////////////////////////////////////////////////////
    float getAspectRatio() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the distance to the near plane
    ///
    /// \return The distance to the near plane
    ///
    ///////////////////////////////////////////////////////////
    float getNear() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the distance to the far plane
    ///
    /// \return The distance to the far plane
    ///
    ///////////////////////////////////////////////////////////
    float getFar() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the distance to the left orthographic plane
    ///
    /// \return The distance to the left orthographic plane
    ///
    ///////////////////////////////////////////////////////////
    float getLeft() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the distance to the right orthographic plane
    ///
    /// \return The distance to the right orthographic plane
    ///
    ///////////////////////////////////////////////////////////
    float getRight() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the distance to the bottom orthographic plane
    ///
    /// \return The distance to the bottom orthographic plane
    ///
    ///////////////////////////////////////////////////////////
    float getBottom() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the distance to the top orthographic plane
    ///
    /// \return The distance to the top orthographic plane
    ///
    ///////////////////////////////////////////////////////////
    float getTop() const;

private:
    Matrix4f m_projMatrix; //!< The projection matrix
    Matrix4f m_viewMatrix; //!< The view matrix
    Frustum m_frustum;     //!< The camera frustum

    Vector3f m_position;  //!< The position in world space
    Vector3f m_direction; //!< The direction vector
    Vector3f m_rightDir;  //!< The right direction vector
    float m_zoom;         //!< The zoom factor

    float m_fov;         //!< The field of view in the x axis
    float m_aspectRatio; //!< The aspect ratio
    float m_near;        //!< The distance to the near plane
    float m_far;         //!< The distance to the far plane

    float m_left;   //!< The distance to the left plane in orthographic mode
    float m_right;  //!< The distance to the right plane in orthographic mode
    float m_bottom; //!< The distance to the bottom plane in orthographic mode
    float m_top;    //!< The distance to the top plane in orthographic mode

    bool m_isPerspective; //!< This is true when the camera is in perspective
                          //!< projection mode
    bool m_isProjDirty; //!< This is true when one of the projection parameters
                        //!< has changed
    bool m_isViewDirty; //!< This is true when on of the view parameters has
                        //!< changed
};

} // namespace ply

///////////////////////////////////////////////////////////
/// \class poly::Camera
/// \ingroup Graphics
///
/// The camera provids a perspective to render the world from.
/// Internally, the camera calculates projection and view matrices
/// based on its parameters, which can then be used to transform
/// mesh vertices into the right location on screen.
///
/// A single camera can be used to render to multiple render
/// targets.
///
/// The default perspective values are:
/// \li 90-degree field of view
/// \li 16:9 aspect ratio
/// \li 0.1 near plane
/// \li 500 far plane
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create camera and set its initial orientation
/// Camera camera;
/// camera.setPosition(0.0f, 1.0f, 5.0f);
/// camera.setDirection(0.0f, -0.2f, -1.0f);
///
/// // Get the projection-view matrix
/// Matrix projView = camera.getProjMatrix() * camera.getViewMatrix();
///
/// \endcode
///
///////////////////////////////////////////////////////////