#pragma once
#include <array>
#include <d3d11.h>

class CModel {
public:
	struct SModelData {
		UINT myNumberOfVerticies = 0;
		UINT myNumberOfIndicies = 0;
		UINT myStride = 0;
		UINT myOffset = 0;
		ID3D11Buffer* myVertexBuffer = nullptr;
		ID3D11Buffer* myIndexBuffer = nullptr;
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11SamplerState* mySamplerState = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
		ID3D11InputLayout* myInputLayout = nullptr;
		std::array<ID3D11ShaderResourceView*, 2> myTexture { };
	};

public:
	CModel();
	~CModel();

	void Init(SModelData data);
	SModelData& GetModelData();
private:
	SModelData myModelData;
};