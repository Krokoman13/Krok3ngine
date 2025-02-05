#pragma once

namespace Engine {
	class KROK3NGINE_API Transform {
	public:
		Transform() : m_world(DirectX::XMMatrixIdentity()) {};

	public:
		const DirectX::XMMATRIX& GetWorldMatrix() const { return m_world; }

		void Move(float a_offsetX, float a_offsetY, float a_offsetZ) {
			m_world *= DirectX::XMMatrixTranslation(a_offsetX, a_offsetY, a_offsetZ);
		}

		void Rotate(float a_axisX, float a_axisY, float a_axisZ, float a_amount) {
			m_world *= DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(a_axisX, a_axisY, a_axisZ, 1.0f), a_amount);
		}

		void Scale(float a_x, float a_y, float a_z) {
			m_world *= DirectX::XMMatrixScaling(a_x, a_y, a_z);
		}


	private:
		DirectX::XMMATRIX m_world;
	};
}