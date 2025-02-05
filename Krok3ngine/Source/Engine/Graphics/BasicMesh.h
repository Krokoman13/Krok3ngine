#pragma once

namespace Engine {
	struct BasicVertex : public Vertex {
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;

		static unsigned int InputElementCount() { return 3; }
		static D3D11_INPUT_ELEMENT_DESC* InputElementDesc() {
			return new D3D11_INPUT_ELEMENT_DESC[]{
				{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 }
			};
		}
	};

	class KROK3NGINE_API BasicMaterial : public Material<BasicVertex> {
	public:
		BasicMaterial() : Material(L"BasicVertexShader.cso", L"BasicPixelShader.cso") {}
	};

	class KROK3NGINE_API BasicMesh : public Mesh<BasicVertex> {
	public:
		BasicMesh() {
			m_pBasicMaterial = std::unique_ptr<BasicMaterial>();
			m_material = m_pBasicMaterial.get();
		}

		BasicMesh(const char* a_meshPath) : m_meshFilePath(a_meshPath) {
			m_pBasicMaterial = std::make_unique<BasicMaterial>();
			m_material = m_pBasicMaterial.get();
		}

	public:
		void Initialize(char* a_meshPath) { m_meshFilePath = a_meshPath; }
	
	protected:
		virtual void load() override;
		virtual void loadFromObj();

	private:
		std::unique_ptr<BasicMaterial> m_pBasicMaterial;
		const char* m_meshFilePath;
	};
}