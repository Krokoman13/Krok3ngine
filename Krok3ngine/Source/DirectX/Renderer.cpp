#include "Krok3ngine.h"

namespace DX {
	struct ConstMatrixBuffer {
		DirectX::XMFLOAT4X4  world;
		DirectX::XMFLOAT4X4  invWorld;
		DirectX::XMFLOAT4X4  view;
		DirectX::XMFLOAT4X4  projection;
	};

	Renderer::Renderer() {
	}

	void Renderer::Initialize(Win32::Window* a_pWindow) {
		m_pWindow = a_pWindow;
		createDeviceResources();
		createWindowSizeDependentResources();
	}

	void Renderer::Render() {
		m_screenViewport = { 0.0f, 0.0f, (float)m_pWindow->GetSize().cx, (float)m_pWindow->GetSize().cy };
		m_d3dAnnotation->BeginEvent(L"Clear");

		// Clear the views.
		auto context = m_d3dContext.Get();
		auto renderTarget = m_d3dRenderTargetView.Get();
		auto depthStencil = m_d3dDepthStencilView.Get();

		// Use linear clear color for gamma-correct rendering.
		DirectX::XMVECTORF32 BackgroundColor = { { { 0.052860655f, 0.052860655f, 0.052860655f, 1.f } } };
		context->ClearRenderTargetView(renderTarget, BackgroundColor);
		context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		context->OMSetRenderTargets(1, &renderTarget, depthStencil);
		auto viewport = m_screenViewport;
		context->RSSetViewports(1, &viewport);

		m_d3dAnnotation->EndEvent();

		//Render
		m_d3dAnnotation->BeginEvent(L"Render");

		while (!m_toLoad.empty()) {
			IRenderObject* toLoad = m_toLoad.back();
			m_toLoad.pop_back();

			if (!toLoad) return;
			toLoad->Load();

			m_toRender.push_back(toLoad);
		}

		for (auto it : m_toRender) {
			ConstMatrixBuffer updatedMatrixBuffer;
			DirectX::XMStoreFloat4x4(&updatedMatrixBuffer.world, it->GetWorldMatrix());
			DirectX::XMStoreFloat4x4(&updatedMatrixBuffer.invWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, it->GetWorldMatrix())));
			DirectX::XMStoreFloat4x4(&updatedMatrixBuffer.view, Engine::Camera::GetMainCamera()->GetView());
			DirectX::XMStoreFloat4x4(&updatedMatrixBuffer.projection, Engine::Camera::GetMainCamera()->GetProjection());

			D3D11_MAPPED_SUBRESOURCE mappedResource;

			// Lock the constant buffer so it can be written to.
			context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (SUCCEEDED(context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
				*static_cast<ConstMatrixBuffer*>(mappedResource.pData) = updatedMatrixBuffer;
				context->Unmap(m_pConstantBuffer.Get(), 0);
			}

			it->Render();
		}

		//Present
		m_d3dAnnotation->EndEvent();
		HRESULT hr = m_swapChain->Present(1, 0);
		m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

		if (m_d3dDepthStencilView) {
			m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());
		}
	}

	void Renderer::createDeviceResources() {
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

		//Todo: setup multiple feature levels for more compatibility

		D3D_FEATURE_LEVEL featureLevel1_11 = D3D_FEATURE_LEVEL_11_1;
		D3D_FEATURE_LEVEL featureLevel9_1 = D3D_FEATURE_LEVEL_9_1;

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

		//Todo: if possible, create a hardware device, not a WARP one

		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP,
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

	void Renderer::createWindowSizeDependentResources() {
		//Render based on screen size
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
		m_d3dRenderTargetView.Reset();
		m_d3dDepthStencilView.Reset();
		m_renderTarget.Reset();
		m_depthStencil.Reset();
		m_d3dContext->Flush();

		// Determine the render target size in pixels.
		UINT backBufferWidth = std::max<UINT>(static_cast<UINT>(m_pWindow->GetSize().cx), 1u);
		UINT backBufferHeight = std::max<UINT>(static_cast<UINT>(m_pWindow->GetSize().cy), 1u);
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
			m_pWindow->GetHandle(),
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr, m_swapChain.ReleaseAndGetAddressOf()
		));

		// This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
		DX::ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(m_pWindow->GetHandle(), DXGI_MWA_NO_ALT_ENTER));

		// Create a render target view of the swap chain back buffer.
		DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_renderTarget.ReleaseAndGetAddressOf())));

		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, m_backBufferFormat);
		DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(
			m_renderTarget.Get(),
			&renderTargetViewDesc,
			m_d3dRenderTargetView.ReleaseAndGetAddressOf()
		));

		if (m_depthBufferFormat != DXGI_FORMAT_UNKNOWN) {
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

		m_screenViewport = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(backBufferWidth),
			static_cast<float>(backBufferHeight)
		);

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = sizeof(ConstMatrixBuffer);
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		DX::ThrowIfFailed(m_d3dDevice->CreateBuffer(&bufferDesc, nullptr, m_pConstantBuffer.ReleaseAndGetAddressOf()));

		ID3D11Buffer* buffer[1] = { m_pConstantBuffer.Get() };
		m_d3dContext->VSSetConstantBuffers(0, 1, buffer);

		if (!Engine::Camera::GetMainCamera()) {
			new Engine::Camera(m_pWindow->GetSize());
		}
	}
}
