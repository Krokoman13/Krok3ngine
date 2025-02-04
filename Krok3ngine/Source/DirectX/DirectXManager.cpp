#include "Krok3ngine.h"
#include "DirectXManager.h"

namespace DX {
	DirectXManager::DirectXManager() : Win32::Window(L"Application", Win32::RESIZABLE, NULL) {
		m_screenViewport = { 0.0f, 0.0f, (float)GetSize().cx, (float)GetSize().cy };
		m_world = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), -DirectX::XM_PI / 2);;

		m_view = DirectX::XMMatrixTranslation(0.0f, -30.0f, 50.f);

		m_proj = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XM_PI / 2.0f,  // Field of view (45 degrees)
			(float)GetSize().cx / (float)GetSize().cy,     
			0.1f,                  // Near plane
			150.0f                 // Far plane
		);
	}

	void DirectXManager::Initialize() {
		createDeviceResources();
		for (IRenderObject* it : m_renderObjects) {
			it->CreateDeviceResources(m_d3dDevice.Get(), m_d3dContext.Get());
		}

		createWindowSizeDependentResources();

		for (auto it : m_renderObjects) {
			it->CreateWindowSizeDependentResources(m_d3dDevice.Get(), GetSize());
		}
	}

	void DirectXManager::createDeviceResources() {
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

		D3D_FEATURE_LEVEL featureLevel1_11 = D3D_FEATURE_LEVEL_11_1;
		D3D_FEATURE_LEVEL featureLevel9_1 = D3D_FEATURE_LEVEL_9_1;

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
			nullptr,
			creationFlags,
			&featureLevel1_11,
			1,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&featureLevel9_1,
			context.GetAddressOf()
		);
		DX::ThrowIfFailed(hr);
		DX::ThrowIfFailed(device.As(&m_d3dDevice));
		DX::ThrowIfFailed(context.As(&m_d3dContext));
		DX::ThrowIfFailed(context.As(&m_d3dAnnotation));
	}

	void DirectXManager::createWindowSizeDependentResources() {
		//Render based on screen size
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
		m_d3dRenderTargetView.Reset();
		m_d3dDepthStencilView.Reset();
		m_renderTarget.Reset();
		m_depthStencil.Reset();
		m_d3dContext->Flush();

		// Determine the render target size in pixels.
		UINT backBufferWidth = std::max<UINT>(static_cast<UINT>(GetSize().cx), 1u);
		UINT backBufferHeight = std::max<UINT>(static_cast<UINT>(GetSize().cy), 1u);
		DXGI_FORMAT backBufferFormat = m_backBufferFormat;

		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = m_backBufferCount;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags = 0u;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a SwapChain from a Win32 window.
		DX::ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice.Get(),
			GetHandle(),
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr, m_swapChain.ReleaseAndGetAddressOf()
		));

		// This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
		DX::ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(GetHandle(), DXGI_MWA_NO_ALT_ENTER));

		// Create a render target view of the swap chain back buffer.
		DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_renderTarget.ReleaseAndGetAddressOf())));

		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, m_backBufferFormat);
		DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(
			m_renderTarget.Get(),
			&renderTargetViewDesc,
			m_d3dRenderTargetView.ReleaseAndGetAddressOf()
		));

		if (m_depthBufferFormat != DXGI_FORMAT_UNKNOWN)
		{
			// Create a depth stencil view for use with 3D rendering if needed.
			CD3D11_TEXTURE2D_DESC depthStencilDesc(
				m_depthBufferFormat,
				backBufferWidth,
				backBufferHeight,
				1, // This depth stencil view has only one texture.
				1, // Use a single mipmap level.
				D3D11_BIND_DEPTH_STENCIL
			);

			DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(
				&depthStencilDesc,
				nullptr,
				m_depthStencil.ReleaseAndGetAddressOf()
			));

			CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
			DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(
				m_depthStencil.Get(),
				&depthStencilViewDesc,
				m_d3dDepthStencilView.ReleaseAndGetAddressOf()
			));
		}

		// Set the 3D rendering viewport to target the entire window.
		m_screenViewport = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(backBufferWidth),
			static_cast<float>(backBufferHeight)
		);
	}

	void DirectXManager::Render() {
		m_world *= DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), 0.01f);
		//m_view *= DirectX::XMMatrixTranslation(0.f, 0.f, 0.01f);

		m_d3dAnnotation->BeginEvent(L"Clear");

		// Clear the views.
		auto context = m_d3dContext.Get();
		auto renderTarget = m_d3dRenderTargetView.Get();
		auto depthStencil = m_d3dDepthStencilView.Get();

		// Use linear clear color for gamma-correct rendering.
		DirectX::XMVECTORF32 BackgroundColor = { { { 0.1f, 0.1f, 0.1f, 1.f } } };
		context->ClearRenderTargetView(renderTarget, BackgroundColor);
		context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		context->OMSetRenderTargets(1, &renderTarget, depthStencil);
		auto viewport = m_screenViewport;
		context->RSSetViewports(1, &viewport);

		m_d3dAnnotation->EndEvent();

		//Render
		m_d3dAnnotation->BeginEvent(L"Render");

		for (auto it : m_renderObjects) {
			it->Render(m_d3dDevice.Get(), m_d3dContext.Get(), m_world, m_view, m_proj);
		}

		//Present
		m_d3dAnnotation->EndEvent();
		HRESULT hr = m_swapChain->Present(1, 0);
		m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

		if (m_d3dDepthStencilView) {
			m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());
		}
	}
}
