#pragma once

namespace DX {
	class KROK3NGINE_API IRenderObject {
	public:
		virtual void CreateDeviceResources(ID3D11Device1* a_device) = 0;
		virtual void CreateWindowSizeDependentResources(ID3D11Device1* a_device, SIZE a_windowSize) {};
		virtual void Render(ID3D11DeviceContext1* a_context, DirectX::FXMMATRIX a_world, DirectX::CXMMATRIX a_view, DirectX::CXMMATRIX a_projection) = 0;
	};

	class KROK3NGINE_API DirectXManager : public Win32::Window {
	public:
		DirectXManager();
		void Initialize() override;
		void Render();

		void AddRenderObject(IRenderObject* a_renderObject) { m_renderObjects.push_back(a_renderObject); }

	protected:
		void createDeviceResources();
		void createWindowSizeDependentResources();

	private:
		std::vector<IRenderObject*> m_renderObjects;
		DirectX::XMMATRIX m_world;
		DirectX::XMMATRIX m_view;
		DirectX::XMMATRIX m_proj;

	private:
		DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
		DXGI_FORMAT m_depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
		UINT m_backBufferCount = 2;
		DXGI_COLOR_SPACE_TYPE m_colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
		unsigned int m_options = 0x1 | 0x1;

		Microsoft::WRL::ComPtr<IDXGIFactory2>				m_dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D11Device1>               m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1>        m_d3dContext;
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>   m_d3dAnnotation;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>             m_swapChain;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_renderTarget;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;
		D3D11_VIEWPORT                                  m_screenViewport;
	};
}