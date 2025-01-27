#include "BlankProject.h"

#include "Engine/Simulation.h"
#include "Platform/Win32/WinEntry.h"

#include "ReadData.h"

class BlankProject : public Krok3ngine::Simulation {
public:
	BlankProject() {};
	~BlankProject() {};

public:
	VOID SetupPerGameSettings();
	VOID Initialize();
	VOID Update();

private:
	DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT m_depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
	UINT m_backBufferCount = 2;
	D3D_FEATURE_LEVEL m_d3dMinFeatureLevel = D3D_FEATURE_LEVEL_10_0;
	D3D_FEATURE_LEVEL m_d3dFeatureLevel = D3D_FEATURE_LEVEL_9_1;
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

	//Triangle
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_spVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_spVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_spPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_spInputLayout;
};

ENTRYAPP(BlankProject)

struct Vertex
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
};

VOID BlankProject::SetupPerGameSettings() {
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
	//PerGameSettings::SetSplashURL(IDS_SPLASHURL);
}

VOID BlankProject::Initialize() {
	//Create device resources
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

	// Determine DirectX hardware feature levels this app will support.
	static const D3D_FEATURE_LEVEL s_featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	UINT featLevelCount = 0;
	for (; featLevelCount < _countof(s_featureLevels); ++featLevelCount)
	{
		if (s_featureLevels[featLevelCount] < m_d3dMinFeatureLevel)
			break;
	}

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
			nullptr,
			creationFlags,
			s_featureLevels,
			featLevelCount,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&m_d3dFeatureLevel,
			context.GetAddressOf()
		);
	DX::ThrowIfFailed(hr);
	DX::ThrowIfFailed(device.As(&m_d3dDevice));
	DX::ThrowIfFailed(context.As(&m_d3dContext));
	DX::ThrowIfFailed(context.As(&m_d3dAnnotation));

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

	//Initialize triangle
	// Load and create shaders.
	auto vertexShaderBlob = DX::ReadData(L"VertexShader.cso");

	DX::ThrowIfFailed(
		device->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(), nullptr, m_spVertexShader.ReleaseAndGetAddressOf())
	);

	auto pixelShaderBlob = DX::ReadData(L"PixelShader.cso");

	DX::ThrowIfFailed(
		device->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(),	nullptr, m_spPixelShader.ReleaseAndGetAddressOf())
	);

	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[2] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	DX::ThrowIfFailed(
		device->CreateInputLayout(s_inputElementDesc, _countof(s_inputElementDesc), vertexShaderBlob.data(), vertexShaderBlob.size(), m_spInputLayout.ReleaseAndGetAddressOf())
	);

	// Create vertex buffer.
	static const Vertex s_vertexData[3] = {
		{ { 0.0f,   0.5f,  0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top / Red
		{ { 0.5f,  -0.5f,  0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },  // Right / Green
		{ { -0.5f, -0.5f,  0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }   // Left / Blue
	};

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = s_vertexData;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(s_vertexData);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof(Vertex);

	DX::ThrowIfFailed(
		device->CreateBuffer(&bufferDesc, &initialData, m_spVertexBuffer.ReleaseAndGetAddressOf())
	);
}

VOID BlankProject::Update() {
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

	//Drawing
	context->IASetInputLayout(m_spInputLayout.Get());

	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, m_spVertexBuffer.GetAddressOf(), &strides, &offsets);

	context->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(m_spPixelShader.Get(), nullptr, 0);
	context->Draw(3, 0);

	//Present
	m_d3dAnnotation->EndEvent();
	HRESULT hr = m_swapChain->Present(1, 0);
	m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

	if (m_d3dDepthStencilView) {
		m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());
	}
}