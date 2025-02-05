#pragma once

namespace Engine {
	//Todo: Make camera's components rather than transforms
	class KROK3NGINE_API Camera : public Transform {
	public:
		Camera(SIZE a_windowSize, float a_fovRadians = DirectX::XM_PI / 2.0f, float a_near = 0.1f, float a_far = 150.0f);

	public:
		static Camera* GetMainCamera() { return s_mainCamera; }

	public:
		const DirectX::XMMATRIX& GetView() { return GetWorldMatrix(); };
		const DirectX::XMMATRIX& GetProjection() { return m_projection; };

	private:
		static Camera* s_mainCamera;

	private:
		DirectX::XMMATRIX m_projection;
	};
}