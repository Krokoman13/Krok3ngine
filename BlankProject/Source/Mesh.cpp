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
		std::stringstream lineStream(line);
	
		char cmd[3]{'\0', '\0', '\0' };
		lineStream >> cmd[0];
		if (line[1] != ' ') lineStream >> cmd[1];

		if (cmd[0] == '#') continue;

		float values[3] = {0, 0, 0};

		if (strcmp(cmd, "v") == 0) {
			DirectX::XMFLOAT3 position;
			if (lineStream >> position.x >> position.y >> position.z) {
				positions.push_back(position);
				continue;
			}
		}

		if (strcmp(cmd, "vn") == 0) {
			DirectX::XMFLOAT3 normal;
			if (lineStream >> normal.x >> normal.y >> normal.z) {
				normals.push_back(normal);
				continue;
			}
		}

		if (strcmp(cmd, "vt") == 0) {
			DirectX::XMFLOAT2 uv;
			if (lineStream >> uv.x >> uv.y) {
				uvs.push_back(uv);
				continue;
			}
		}

		if (strcmp(cmd, "f") == 0) {
			Vertex face[4];
			bool fourthVector = false;

			std::string vertex;
			for (unsigned int i = 0; i < 4; i++) {
				if (!(lineStream >> vertex)) break;
				std::stringstream verexStream(vertex);

				std::string value;
				if (!std::getline(verexStream, value, '/') || value.empty()) break;
				face[i].position = positions[stoi(value) - 1];

				if (std::getline(verexStream, value, '/') && !value.empty()) {
					face[i].uv = uvs[stoi(value) - 1];
				}

				unsigned int normalIndex;
				if (verexStream >> normalIndex) {
					face[i].normal = normals[normalIndex - 1];
				}

				if (i == 3) fourthVector = true;
			}

			for (unsigned int i = 0; i < 3; i++) m_vertices.push_back(face[i]);
			if (fourthVector) { // In case there is a fourth vector in the face, create a new triangle
				m_vertices.push_back(face[2]);
				m_vertices.push_back(face[3]);
				m_vertices.push_back(face[0]);
			}
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

	m_vertices.clear();
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