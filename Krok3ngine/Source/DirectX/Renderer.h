#pragma once

namespace DX {
	class KROK3NGINE_API Renderer;

	class KROK3NGINE_API IRenderObject : public Engine::Transform {
	public:
		void Initialize(Renderer* a_pRenderer) { m_pRenderer = a_pRenderer; };
		virtual void Load() = 0;
		virtual void Render() = 0;

	protected:
		Renderer* m_pRenderer;
	};

	//Todo: Move Device managment to another class
	//Todo: Update Renderer accordingly when the window gets resized
	class KROK3NGINE_API Renderer {
	public:
		Renderer();

	public:
		void Initialize(Win32::Window* a_pWindow);
		void Render();

		void AddRenderObject(IRenderObject* a_renderObject) { 
			a_renderObject->Initialize(this);
			m_toLoad.push_back(a_renderObject); 
		}

	public:
		ID3D11Device1* GetDevice() { return m_d3dDevice.Get(); }
		ID3D11DeviceContext1* GetContext() { return m_d3dContext.Get(); }

	protected:
		void createDeviceResources();
		void createWindowSizeDependentResources();

	private:
		Win32::Window* m_pWindow = nullptr;
		std::vector<IRenderObject*> m_toLoad;
		std::vector<IRenderObject*> m_toRender;

		//Todo: Move constant buffer to a more appropriate class
		Microsoft::WRL::ComPtr<ID3D11Buffer>			m_pConstantBuffer;

	private:
		DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
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
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthState;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;
		D3D11_VIEWPORT                                  m_screenViewport;
	};
}