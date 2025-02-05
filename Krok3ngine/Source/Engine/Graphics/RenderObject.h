#pragma once

namespace Engine {
	struct Vertex {
		DirectX::XMFLOAT3 position;

		static unsigned int InputElementCount() { return 1; }
		static D3D11_INPUT_ELEMENT_DESC* InputElementDesc() {
			return new D3D11_INPUT_ELEMENT_DESC[]{
				{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
			};
		}
	};

	template<typename VertexData = Vertex>
	class KROK3NGINE_API Material {
	public:
		Material(const wchar_t* a_vertexShaderPath, const wchar_t* a_pixelShaderPath) 
			: m_vertexShaderPath(a_vertexShaderPath), m_pixelShaderPath(a_pixelShaderPath){
			static_assert(std::derived_from<VertexData, Vertex>, "VertexData must inherit from IVertex");
		};

	public:
		void Load(ID3D11Device1* a_device) {
			auto vertexShaderBlob = File::ReadData(m_vertexShaderPath);
			DX::ThrowIfFailed(
				a_device->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(), nullptr, m_spVertexShader.ReleaseAndGetAddressOf())
			);

			auto pixelShaderBlob = File::ReadData(m_pixelShaderPath);
			DX::ThrowIfFailed(
				a_device->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(), nullptr, m_spPixelShader.ReleaseAndGetAddressOf())
			);

			const D3D11_INPUT_ELEMENT_DESC* inputElementDesc = VertexData::InputElementDesc();

			DX::ThrowIfFailed(
				a_device->CreateInputLayout(inputElementDesc, VertexData::InputElementCount(), vertexShaderBlob.data(), vertexShaderBlob.size(), m_spInputLayout.ReleaseAndGetAddressOf())
			);
		};

		void SetShaders(ID3D11DeviceContext1* a_context) {
			a_context->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
			a_context->GSSetShader(nullptr, nullptr, 0);
			a_context->PSSetShader(m_spPixelShader.Get(), nullptr, 0);
		}

	public:
		ID3D11VertexShader* GetVertexShader() { return m_spVertexShader.Get(); }
		ID3D11PixelShader* GetPixelShader() { return m_spPixelShader.Get(); }
		ID3D11InputLayout* GetInputLayout() { return m_spInputLayout.Get(); }

	private:
		const wchar_t* m_vertexShaderPath;
		const wchar_t* m_pixelShaderPath;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_spVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_spPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_spInputLayout;
	};

	template<typename VertexData = Vertex>
	class KROK3NGINE_API Mesh : public DX::IRenderObject {
	public:
		Mesh() {
			static_assert(std::derived_from<VertexData, Vertex>, "VertexData must inherit from VertexBase");
		};

		Mesh(const std::vector<VertexData>& a_vertices) : Mesh(){
			m_vertices = a_vertices;
		};

	public:
		virtual void Load() override {
			m_material->Load(m_pRenderer->GetDevice());
			load();
			afterLoad();
		}

		virtual void Render() override {
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			ID3D11DeviceContext1* context = m_pRenderer->GetContext();
			context->IASetInputLayout(m_material->GetInputLayout());

			UINT strides = sizeof(VertexData);
			UINT offsets = 0;
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetVertexBuffers(0, 1, m_spVertexBuffer.GetAddressOf(), &strides, &offsets);

			m_material->SetShaders(context);

			context->Draw(m_vertexCount, 0);
		}

	public:
		Material<VertexData>* GetMaterial() { return m_material; };
		void SetMaterial(Material<VertexData>* a_material) { m_material = a_material; }

	protected:
		virtual void load() {};

		void afterLoad() {
			if (m_vertices.empty()) throw Exception::Warning("Emtpy Mesh, does not contain vertices", 5);

			{
				D3D11_SUBRESOURCE_DATA initialData = {};
				const VertexData* vertexData = m_vertices.data();
				initialData.pSysMem = vertexData;

				D3D11_BUFFER_DESC bufferDesc = {};
				bufferDesc.ByteWidth = sizeof(vertexData);
				bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bufferDesc.StructureByteStride = sizeof(VertexData);

				DX::ThrowIfFailed(
					m_pRenderer->GetDevice()->CreateBuffer(&bufferDesc, &initialData, m_spVertexBuffer.ReleaseAndGetAddressOf())
				);
			}

			m_vertexCount = m_vertices.size();
			m_vertices.clear();

			{
				D3D11_SUBRESOURCE_DATA initialData = {};
				initialData.pSysMem = m_vertices.data();

				D3D11_BUFFER_DESC bufferDesc = {};
				bufferDesc.ByteWidth = sizeof(VertexData) * m_vertexCount;
				bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bufferDesc.StructureByteStride = sizeof(VertexData);

				DX::ThrowIfFailed(
					m_pRenderer->GetDevice()->CreateBuffer(&bufferDesc, &initialData, m_spVertexBuffer.GetAddressOf())
				);
			}
		}

	protected:
		Material<VertexData>* m_material;
		std::vector<VertexData> m_vertices;

	private:
		unsigned int m_vertexCount;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_spVertexBuffer;
	};
}