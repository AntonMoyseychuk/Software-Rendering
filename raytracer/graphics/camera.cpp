#include "camera.hpp"
#include "math_3d/lin_math.hpp"

namespace gfx {
    Camera::Camera()
        : Object(), m_look_at(math::VECTOR_BACKWARD), m_up(math::VECTOR_UP), m_length(), m_horz_size(1.0f), m_aspect_ratio(1.0f)
    {
    }
    
    void Camera::SetLookAt(const math::vec4f &look_at) noexcept {
        m_look_at = look_at;
    }
    
    void Camera::SetUp(const math::vec4f &up) noexcept {
        m_up = up;
    }
    
    void Camera::SetLength(float lenght) noexcept {
        m_length = lenght;
    }

    void Camera::SetHorzSize(float size) noexcept {
        m_horz_size = size;
    }

    void Camera::SetAspect(float aspect_ratio) noexcept {
        m_aspect_ratio = aspect_ratio;
    }
    
    math::vec4f Camera::GetLookAt() const noexcept {
        return m_look_at;
    }

    math::vec4f Camera::GetUp() const noexcept {
        return m_up;
    }

    math::vec4f Camera::GetU() const noexcept {
        return m_projection_screen_U;
    }

    math::vec4f Camera::GetV() const noexcept {
        return m_projection_screen_V;
    }

    math::vec4f Camera::GetScreenCentre() const noexcept {
        return m_projection_screen_centre;
    }

    float Camera::GetLength() const noexcept {
        return m_length;
    }

    float Camera::GetHorzSize() const noexcept {
        return m_horz_size;
    }

    float Camera::GetAspect() const noexcept {
        return m_aspect_ratio;
    }

    Ray Camera::GenerateRay(float screen_x, float screen_y) const noexcept {
        // Compute the location of the screen point in world coordinates.
        math::vec4f screen_world_part1 = m_projection_screen_centre + (m_projection_screen_U * screen_x);
        math::vec4f screen_world_coordinate = screen_world_part1 + (m_projection_screen_V * screen_y);
        
        // Use this point along with the camera position to compute the ray.
        return Ray(m_position, screen_world_coordinate);
    }

    void Camera::UpdateCameraGeometry() noexcept {
        // First, compute the vector from the camera position to the LookAt position.
        m_alignment_vector = (m_look_at - m_position).Normalize();
        
        // Second, compute the alpha and beta unit vectors.
        m_projection_screen_U = math::LinMath::Cross(m_alignment_vector, m_up).Normalize();
        m_projection_screen_V = math::LinMath::Cross(m_projection_screen_U, m_alignment_vector).Normalize();
        
        // Thirdly, compute the position of the center point of the screen.
        m_projection_screen_centre = m_position + m_alignment_vector * m_length;
        
        // Modify the U and V vectors to match the size and aspect ratio.
        m_projection_screen_U *= m_horz_size;
        m_projection_screen_V *= m_horz_size / m_aspect_ratio;
    }
}