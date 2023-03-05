#pragma once
#include "objects/object.hpp"

#include "ray.hpp"

namespace gfx {
    class Camera : public Object {
    public:
		Camera();
	
		void SetLookAt(const math::vec4f& look_at) noexcept;
		void SetUp(const math::vec4f& up) noexcept;
		void SetLength(float length) noexcept;
		void SetHorzSize(float size) noexcept;
		void SetAspect(float aspect_ration) noexcept;
	
		math::vec4f GetLookAt() const noexcept;
		math::vec4f	GetUp() const noexcept;
		math::vec4f	GetU() const noexcept;
		math::vec4f	GetV() const noexcept;
		math::vec4f	GetScreenCentre() const noexcept;
		float GetLength() const noexcept;
		float GetHorzSize() const noexcept;
		float GetAspect() const noexcept;
	
		Ray GenerateRay(float screen_x, float screen_y) const noexcept;
	
		void UpdateCameraGeometry() noexcept;
	
	private:
		math::vec4f m_look_at;
		math::vec4f m_up;
		float m_length;
		float m_horz_size;
		float m_aspect_ratio;
		
		math::vec4f m_alignment_vector;
		math::vec4f m_projection_screen_U;
		math::vec4f m_projection_screen_V;
		math::vec4f m_projection_screen_centre;
    };
}