#pragma once
#include <vector>

struct Vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

class Mesh {
public:
	Mesh();

public:
	void Load(std::wstring a_filename);
	void Initialize(ID3D11Device1* a_device);

public:
	Vertex* GetVerticesData() { return m_vertices.data();  };
	unsigned int GetVertexCount() { return m_vertices.size(); };
	ID3D11Buffer** GetVertexBufferAddress() { return m_spVertexBuffer.GetAddressOf(); };

private:
	std::vector<std::string> split(const std::string& str, char delimiter, unsigned int maxCount = 0); //Todo: Move this to a file management class

private:
	std::vector<Vertex> m_vertices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_spVertexBuffer;
};
