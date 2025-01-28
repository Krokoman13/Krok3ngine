#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

#include "BlankProject.h"
#include "Mesh.h"

Mesh::Mesh(): m_vertices() 
{
}

void Mesh::Load(std::wstring a_filename) {
	std::ifstream file(a_filename, std::ios::in);

	if (!file.is_open()) {
		return;
	}

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> uvs;

	std::string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		const std::vector<std::string> elemnts = split(line, ' ', false, 9);
		const std::string cmd = elemnts[0];

		if (cmd == "#") continue;

		if (cmd == "v" && elemnts.size() >= 4) {
			DirectX::XMFLOAT3 position;
			position.x = std::stof(elemnts[1]);
			position.y = std::stof(elemnts[2]);
			position.z = std::stof(elemnts[3]);
			positions.push_back(position);
			continue;
		}

		if (cmd == "vn" && elemnts.size() >= 4) {
			DirectX::XMFLOAT3 normal;
			normal.x = std::stof(elemnts[1]);
			normal.y = std::stof(elemnts[2]);
			normal.z = std::stof(elemnts[3]);
			normals.push_back(normal);
			continue;
		}

		if (cmd == "vt" && elemnts.size() >= 3) {
			DirectX::XMFLOAT2 uv;
			uv.x = std::stof(elemnts[1]);
			uv.y = std::stof(elemnts[2]);
			uvs.push_back(uv);
			continue;
		}

		if (cmd != "f" || elemnts.size() < 4) {
			continue;
		}

		for (int i = 1; i < 4; ++i) {
			Vertex vertex;
			const std::vector<std::string> indices = split(elemnts[i], '/', true, 3);

			if (indices.size() < 3) continue;

			try {
				int positionIndx = std::stoi(indices[0]);
				if (positionIndx > 0) vertex.position = positions[positionIndx - 1];
			} catch (const std::invalid_argument& e) {}

			try {
				int uvIndx = std::stoi(indices[1]);
				if (uvIndx > 0) vertex.uv = uvs[uvIndx - 1];
			}
			catch (const std::invalid_argument& e) {}

			try {
				int normalIndx = std::stoi(indices[2]);
				if (normalIndx > 0) vertex.normal = normals[normalIndx - 1];
			} catch (const std::invalid_argument& e) {}

			m_vertices.push_back(vertex);
		}
	}

	file.close();
}

void Mesh::Initialize(ID3D11Device1* a_device) {
	D3D11_SUBRESOURCE_DATA initialData = {};
	const Vertex* vertexData = m_vertices.data();
	initialData.pSysMem = vertexData;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(vertexData);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof(Vertex);

	DX::ThrowIfFailed(
		a_device->CreateBuffer(&bufferDesc, &initialData, m_spVertexBuffer.ReleaseAndGetAddressOf())
	);
}

std::vector<std::string> Mesh::split(const std::string& a_string, char a_delimiter, bool a_returnEmpty, unsigned int a_maxCount) {
	std::vector<std::string> tokens;
	std::stringstream ss(a_string);
	std::string token;

	while (std::getline(ss, token, a_delimiter)) {
		if (token.empty() && !a_returnEmpty) continue;
		tokens.push_back(token);
		if (tokens.size() == a_maxCount) break;
	} 

	return tokens;
}



