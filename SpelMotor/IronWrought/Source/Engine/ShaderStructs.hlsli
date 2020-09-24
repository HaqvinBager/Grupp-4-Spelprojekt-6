TextureCube environmentTexture : register(t0);
Texture2D diffuseTexture : register(t1); //RGB-A = Albedo
Texture2D materialTexture : register(t2); //R = Metalness, G = Roughness, B = Emissive
Texture2D normalTexture : register(t3); //RGB = Normal, A = AmbientOcclusion

SamplerState defaultSampler : register(s0);

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCamera;	//64
    float4x4 toProjection; //64
    float4 cameraPosition; //16
    float4 toDirectionalLight; //16
    float4 directionalLightColor; //16
};

cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
    struct PointLight
    {
        float3 myPosition;
        float myRange;
        float4 myColor;
    } myPointLights[8];
    
    float myNumberOfUsedPointLights;
    float3 padding;
};

struct VertexInput
{
	float4 myPosition : POSITION;
	float4 myNormal : NORMAL;
	float4 myTangent : TANGENT;
	float4 myBiNormal : BITANGENT;
	float2 myUV : UV;
	float4 myBoneID : BONEID;
	float4 myBoneWeight : BONEWEIGHT;
};

struct VertexToPixel
{
	float4 myPosition : SV_POSITION;
	float4 myWorldPosition : WORLD_POSITION;
	float4 myNormal : NORMAL;
	float4 myTangent : TANGENT;
	float4 myBiNormal : BITANGENT;
	float2 myUV : UV;
};

struct PixelOutPut
{
	float4 myColor : SV_TARGET;
};

