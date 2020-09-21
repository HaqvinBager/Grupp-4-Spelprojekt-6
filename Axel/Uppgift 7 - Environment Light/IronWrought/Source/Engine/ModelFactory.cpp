#include "stdafx.h"
#include "ModelFactory.h"
#include "Model.h"
#include "Engine.h"
#include <d3d11.h>
#include <fstream>
#include "DDSTextureLoader.h"
#include "FBXLoaderCustom.h"

#ifdef _DEBUG
#pragma comment(lib, "ModelLoader_Debug.lib")
#endif
#ifdef NDEBUG
#pragma comment(lib, "ModelLoader_Release.lib")
#endif

CModelFactory* CModelFactory::ourInstance = nullptr;
CModelFactory* CModelFactory::GetInstance()
{
	return ourInstance;
}

CModelFactory::CModelFactory() : myEngine(nullptr)
{
	ourInstance = this;
	myEngine = nullptr;
}

CModelFactory::~CModelFactory()
{
	ourInstance = nullptr;
	myEngine = nullptr;
}

bool CModelFactory::Init(CEngine& engine)
{
	myEngine = &engine;
	return true;
}

CModel* CModelFactory::GetModel(std::string aFilePath)
{
	if (myModelMap.find(aFilePath) == myModelMap.end())
	{
		return LoadModel(aFilePath);
	}
	return myModelMap.at(aFilePath);
}

CModel* CModelFactory::LoadModel(std::string aFilePath)
{
	HRESULT result;

	const size_t last_slash_idx = aFilePath.find_last_of("\\/");
	std::string modelDirectory = aFilePath.substr(0, last_slash_idx + 1);

	CFBXLoaderCustom modelLoader;
	CLoaderModel* loaderModel = modelLoader.LoadModel(aFilePath.c_str());
	CLoaderMesh* mesh = loaderModel->myMeshes[0];

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = mesh->myVertexBufferSize * mesh->myVertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subVertexResourceData = { 0 };
	subVertexResourceData.pSysMem = mesh->myVerticies;

	ID3D11Buffer* vertexBuffer;
	result = myEngine->myFramework->GetDevice()->CreateBuffer(&vertexBufferDesc, &subVertexResourceData, &vertexBuffer);
	if (FAILED(result))
		return nullptr;

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(mesh->myIndexes.size());
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subIndexResourceData = { 0 };
	subIndexResourceData.pSysMem = mesh->myIndexes.data();

	ID3D11Buffer* indexBuffer;
	result = myEngine->myFramework->GetDevice()->CreateBuffer(&indexBufferDesc, &subIndexResourceData, &indexBuffer);
	if (FAILED(result))
		return nullptr;

	//VertexShader
	std::ifstream vsFile;
	vsFile.open("VertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	ID3D11VertexShader* vertexShader;
	result = myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);

	if (FAILED(result))
		return nullptr;

	vsFile.close();

	//PixelShader
	std::ifstream psFile;
	psFile.open("PixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	ID3D11PixelShader* pixelShader;
	result = myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);

	if (FAILED(result))
		return nullptr;

	psFile.close();

	//Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	result = myEngine->myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(result))
		return nullptr;

	//Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11InputLayout* inputLayout;
	result = myEngine->myFramework->GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result))
		return nullptr;

	std::string albedo_path = modelDirectory + loaderModel->myTextures[0];
	
		wchar_t* albedo_path_wide = new wchar_t[albedo_path.length() + 1];
		std::copy(albedo_path.begin(), albedo_path.end(), albedo_path_wide);
		albedo_path_wide[albedo_path.length()] = 0;
		ID3D11ShaderResourceView* albedoResourceView;
		result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), albedo_path_wide, nullptr, &albedoResourceView);
		if (FAILED(result))
		{
			return nullptr;
		}
		delete[] albedo_path_wide;
	



	std::string normal_path = modelDirectory + loaderModel->myTextures[5];
	
		wchar_t* normal_path_wide = new wchar_t[normal_path.length() + 1];
		std::copy(normal_path.begin(), normal_path.end(), normal_path_wide);
		normal_path_wide[normal_path.length()] = 0;

		ID3D11ShaderResourceView* normalResourceView;
		result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), normal_path_wide, nullptr, &normalResourceView);
		if (FAILED(result))
			return nullptr;

		delete[] normal_path_wide;
	
	//Model
	CModel* model = new CModel();
	if (!model)
		return nullptr;

	CModel::SModelData modelData;
	modelData.myNumberOfVerticies = mesh->myVertexCount;
	modelData.myNumberOfIndicies = static_cast<UINT>(mesh->myIndexes.size());
	modelData.myStride = mesh->myVertexBufferSize;
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.mySamplerState = sampler;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = albedoResourceView;
	modelData.myTexture[1] = normalResourceView;

	model->Init(modelData);

	myModelMap.emplace(aFilePath, model);
	return model;
}

CModel* CModelFactory::GetCube()
{

	HRESULT result;

	//Vertex Setup
	struct Vertex
	{
		float x, y, z, w;
		float r, g, b, a;
		float u, v;
	} verticies[24] = {
		//X Y Z		      W		   RGBA			     UV	
		{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 },
		{  1, -1, -1,     1,       1, 1, 0, 1,       1, 0 },
		{ -1,  1, -1,     1,       1, 0, 1, 1,       0, 1 },
		{  1,  1, -1,     1,       0, 1, 1, 1,       1, 1 },
		{ -1, -1,  1,     1,       1, 0, 0, 1,       0, 0 },
		{  1, -1,  1,     1,       0, 1, 0, 1,       1, 0 },
		{ -1,  1,  1,     1,       0, 0, 1, 1,       0, 1 },
		{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 },
		//X Y Z		      W		   RGBA				 UV	
		{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 },
		{ -1,  1, -1,     1,       1, 1, 0, 1,       1, 0 },
		{ -1, -1,  1,     1,       1, 0, 1, 1,       0, 1 },
		{ -1,  1,  1,     1,       0, 1, 1, 1,       1, 1 },
		{  1, -1, -1,     1,       1, 0, 0, 1,       0, 0 },
		{  1,  1, -1,     1,       0, 1, 0, 1,       1, 0 },
		{  1, -1,  1,     1,       0, 0, 1, 1,       0, 1 },
		{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 },
		//X Y Z		      W		   RGBA			     UV		
		{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 },
		{  1, -1, -1,     1,       1, 1, 0, 1,       1, 0 },
		{ -1, -1,  1,     1,       1, 0, 1, 1,       0, 1 },
		{  1, -1,  1,     1,       0, 1, 1, 1,       1, 1 },
		{ -1,  1, -1,     1,       1, 0, 0, 1,       0, 0 },
		{  1,  1, -1,     1,       0, 1, 0, 1,       1, 0 },
		{ -1,  1,  1,     1,       0, 0, 1, 1,       0, 1 },
		{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 }
	};
	//Index Setup
	unsigned int indicies[36] = {
		0,2,1,
		2,3,1,
		4,5,7,
		4,7,6,
		8,10,9,
		10,11,9,
		12,13,15,
		12,15,14,
		16,17,18,
		18,17,19,
		20,23,21,
		20,22,23
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(verticies);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResourceData = { 0 };
	subResourceData.pSysMem = verticies;

	ID3D11Buffer* vertexBuffer;
	result = myEngine->myFramework->GetDevice()->CreateBuffer(&vertexBufferDesc, &subResourceData, &vertexBuffer);
	if (FAILED(result))
		return nullptr;

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(indicies);
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA indexSubresourceData = { 0 };
	indexSubresourceData.pSysMem = indicies;

	ID3D11Buffer* indexBuffer;
	result = myEngine->myFramework->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}

	//VertexShader
	std::ifstream vsFile;
	vsFile.open("CubeVertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result))
		return nullptr;
	vsFile.close();

	//PixelShader
	std::ifstream psFile;
	psFile.open("CubePixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result))
		return nullptr;
	psFile.close();

	//Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	result = myEngine->myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(result))
		return nullptr;

	//Layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D11InputLayout* inputLayout;
	result = myEngine->myFramework->GetDevice()->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result))
		return nullptr;

	std::wstring filename = L"Texture.dds";
	ID3D11ShaderResourceView* shaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), filename.c_str(), nullptr, &shaderResourceView);
	if (FAILED(result))
	{
		return nullptr;
	}

	//Model
	CModel* model = new CModel();
	if (!model)
		return nullptr;

	CModel::SModelData modelData;
	modelData.myNumberOfVerticies = sizeof(verticies) / sizeof(Vertex);
	modelData.myNumberOfIndicies = sizeof(indicies) / sizeof(unsigned int);
	modelData.myStride = sizeof(Vertex);
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.mySamplerState = sampler;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = shaderResourceView;

	model->Init(modelData);
	myModelMap.emplace("Cube", model);
	return model;
}
