#pragma once

#include "DirectX/Renderer.h"

namespace Krok3ngine {
	class KROK3NGINE_API Simulation : public Win32::IApplication {
	public:
		Simulation();
		~Simulation();

	protected:
		void baseInitialize() override;
		void baseLoad() override;
		void baseUpdate() override;

		void AddRenderObject(DX::IRenderObject* a_renderObject) {
			m_renderer.AddRenderObject(a_renderObject);
		}

	protected:
		Win32::Window m_window;

	private:
		DX::Renderer m_renderer;
	};
}