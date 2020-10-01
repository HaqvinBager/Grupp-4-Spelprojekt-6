#pragma once

struct ID3D11ShaderResourceView;
class CDirectXFramework;
namespace DirectX {
	namespace SimpleMath {
		struct Vector4;
		struct Vector3;
	}
}

class CEnvironmentLight
{
public:
	CEnvironmentLight();
	~CEnvironmentLight();

	bool Init(CDirectXFramework* aFramework, std::string aFilePath);
	void SetDirection(DirectX::SimpleMath::Vector3 aDirection);
	void SetColor(DirectX::SimpleMath::Vector3 aColor);

	DirectX::SimpleMath::Vector4 GetDirection() { return myDirection; }
	DirectX::SimpleMath::Vector4 GetColor() { return myColor; }

	ID3D11ShaderResourceView* GetCubeMap();
private:
	ID3D11ShaderResourceView* myCubeMap = nullptr;
	DirectX::SimpleMath::Vector4 myDirection; //Points towards the Light
	DirectX::SimpleMath::Vector4 myColor; //Color (XYZ) and Intensity (W)
};