#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CEngine;

class CPointLight
{
public:
	CPointLight();
	~CPointLight();

	bool Init();

	void SetPosition(DirectX::SimpleMath::Vector3 aPosition)	{ myPosition = aPosition; }
	void SetColor(DirectX::SimpleMath::Vector3 aColor)			{ myColor = aColor; }
	void SetRange(float aRange)									{ myRange = aRange; }
	void SetIntensity(float anIntensity)						{ myIntensity = anIntensity; }

	DirectX::SimpleMath::Vector3 GetPosition() 	{ return myPosition; };
	DirectX::SimpleMath::Vector3 GetColor() 	{ return myColor; }
	float GetRange() 							{ return myRange; }
	float GetIntensity() 						{ return myIntensity; }
private:

	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myColor;
	float myRange;
	float myIntensity;
};