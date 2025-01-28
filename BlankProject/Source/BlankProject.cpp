#include "BlankProject.h"

#include "Engine/Simulation.h"
#include "Platform/Win32/WinEntry.h"

#include "ReadData.h"

struct Vertex
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
};

class Triangle : public DX::IRenderObject {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_spVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_spVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_spPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_spInputLayout;

public:
	virtual void CreateDeviceResources(ID3D11Device1* a_device) override {
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
			a_device->CreateBuffer(&bufferDesc, &initialData, m_spVertexBuffer.ReleaseAndGetAddressOf())
		);
	};

	virtual void Render(ID3D11DeviceContext1* a_context, DirectX::FXMMATRIX a_world, DirectX::CXMMATRIX a_view, DirectX::CXMMATRIX a_projection) override {
		a_context->IASetInputLayout(m_spInputLayout.Get());

		UINT strides = sizeof(Vertex);
		UINT offsets = 0;
		a_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		a_context->IASetVertexBuffers(0, 1, m_spVertexBuffer.GetAddressOf(), &strides, &offsets);

		a_context->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
		a_context->GSSetShader(nullptr, nullptr, 0);
		a_context->PSSetShader(m_spPixelShader.Get(), nullptr, 0);
		a_context->Draw(3, 0);
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