#include "Krok3ngine.h"

namespace Engine {
	Camera* Camera::s_mainCamera = nullptr;

	Camera::Camera(SIZE a_windowSize, float a_fovRadians, float a_near, float a_far) {
		m_projection = DirectX::XMMatrixPerspectiveFovLH(
			a_fovRadians,
			(float)a_windowSize.cx / (float)a_windowSize.cy,
			a_near, a_far
		);

		if (s_mainCamera) return;
		s_mainCamera = this;
	}
}