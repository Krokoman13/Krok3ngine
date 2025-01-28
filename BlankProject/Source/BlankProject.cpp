#include "BlankProject.h"

#include "Engine/Simulation.h"
#include "Platform/Win32/WinEntry.h"

#include "ReadData.h"

struct Vertex {
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
};

struct MatrixBuffer {
	DirectX::XMFLOAT4X4  world;
	DirectX::XMFLOAT4X4  view;
	DirectX::XMFLOAT4X4  projection;
};

class Triangle : public DX::IRenderObject {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_spVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_spVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_spPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_spInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_pConstantBuffer;

	Vertex m_topLeftFront =		{ { -0.5f,  0.5f,  0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }; // Red
	Vertex m_topRightFront =	{ {  0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }; // Blue
	Vertex m_bottemRightFront = { {  0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }; // Green
	Vertex m_bottemLeftFront =	{ { -0.5f, -0.5f,  0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }; // White

	Vertex m_bottemRightBack =	{ {  0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }; // Cyan
	Vertex m_topRightBack =		{ {  0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } }; // Magenta
	Vertex m_topLeftBack =		{ { -0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }; // Yellow
	Vertex m_bottemLeftBack =	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }; // Black

	// Create vertex buffer.
	const Vertex m_vertexData[36] = {
		//Front
		m_bottemRightFront,
		m_topRightFront,
		m_topLeftFront,
		//Front
		m_topLeftFront,
		m_bottemLeftFront,
		m_bottemRightFront,
		

		m_bottemRightBack,
		m_topRightBack,
		m_topRightFront,
		//Right
		m_topRightFront,
	    m_bottemRightFront,
		m_bottemRightBack,


		m_topLeftBack,
		m_topRightBack,  
		m_bottemRightBack,
		//Back
		m_bottemRightBack,
		m_bottemLeftBack,  
		m_topLeftBack,

		m_topLeftBack,
		m_bottemLeftBack,
		m_bottemLeftFront,
		//Left
		m_bottemLeftFront,
		m_topLeftFront,
		m_topLeftBack,

		m_topRightFront,
		m_topRightBack,
		m_topLeftBack,
		//Top
		m_topLeftBack,
		m_topLeftFront,
		m_topRightFront,

		m_bottemLeftFront,
		m_bottemLeftBack,
		m_bottemRightBack,
		//Bottem
		m_bottemRightBack,
		m_bottemRightFront,
		m_bottemLeftFront,
	};

public:
	virtual void CreateDeviceResources(ID3D11Device1* a_device, ID3D11DeviceContext1* a_context) override {
		// Load and create shaders.
		auto vertexShaderBlob = DX::ReadData(L"VertexShader.cso");

		DX::ThrowIfFailed(
			a_device->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(), nullptr, m_spVertexShader.ReleaseAndGetAddressOf())
		);

		auto pixelShaderBlob = DX::ReadData(L"PixelShader.cso");

		DX::ThrowIfFailed(
			a_device->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(), nullptr, m_spPixelShader.ReleaseAndGetAddressOf())
		);

		// Create input layout.
		static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[2] =
		{
			{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
			{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		};

		DX::ThrowIfFailed(
			a_device->CreateInputLayout(s_inputElementDesc, _countof(s_inputElementDesc), vertexShaderBlob.data(), vertexShaderBlob.size(), m_spInputLayout.ReleaseAndGetAddressOf())
		);

		{
			D3D11_SUBRESOURCE_DATA initialData = {};
			initialData.pSysMem = m_vertexData;

			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.ByteWidth = sizeof(m_vertexData);
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.StructureByteStride = sizeof(Vertex);

			DX::ThrowIfFailed(
				a_device->CreateBuffer(&bufferDesc, &initialData, m_spVertexBuffer.ReleaseAndGetAddressOf())
			);
		}

		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.ByteWidth = sizeof(MatrixBuffer);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			DX::ThrowIfFailed(a_device->CreateBuffer(&bufferDesc, nullptr, m_pConstantBuffer.ReleaseAndGetAddressOf()));

			ID3D11Buffer* buffer[1] = { m_pConstantBuffer.Get() };
			a_context->VSSetConstantBuffers(0, 1, buffer);
		}
	};

	virtual void CreateWindowSizeDependentResources(ID3D11Device1* a_device, SIZE a_windowSize) override {

	}

	virtual void Render(ID3D11Device1* a_device, ID3D11DeviceContext1* a_context, DirectX::FXMMATRIX a_world, DirectX::CXMMATRIX a_view, DirectX::CXMMATRIX a_projection) override {
		MatrixBuffer updatedMatrixBuffer;
		DirectX::XMStoreFloat4x4(&updatedMatrixBuffer.world, a_world);
		DirectX::XMStoreFloat4x4(&updatedMatrixBuffer.view, a_view);
		DirectX::XMStoreFloat4x4(&updatedMatrixBuffer.projection, a_projection);
		
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Lock the constant buffer so it can be written to.
		a_context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (SUCCEEDED(a_context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
			*static_cast<MatrixBuffer*>(mappedResource.pData) = updatedMatrixBuffer;
			a_context->Unmap(m_pConstantBuffer.Get(), 0);
		}
		
		
		a_context->IASetInputLayout(m_spInputLayout.Get());

		UINT strides = sizeof(Vertex);
		UINT offsets = 0;
		a_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		a_context->IASetVertexBuffers(0, 1, m_spVertexBuffer.GetAddressOf(), &strides, &offsets);

		a_context->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
		a_context->GSSetShader(nullptr, nullptr, 0);
		a_context->PSSetShader(m_spPixelShader.Get(), nullptr, 0);
		a_context->Draw(sizeof(m_vertexData), 0);
	}
};

class BlankProject : public Krok3ngine::Simulation {
public:
	BlankProject() { DX::DirectXManager::AddRenderObject(new Triangle()); };
	~BlankProject() {};

public:
	VOID SetupPerGameSettings();

private:
};

ENTRYAPP(BlankProject)

VOID BlankProject::SetupPerGameSettings() {
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
	//PerGameSettings::SetSplashURL(IDS_SPLASHURL);
}