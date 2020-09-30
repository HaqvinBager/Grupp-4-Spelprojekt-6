TextureCube environmentTexture : register(t0);
texture2D albedoTexture : register(t1);
texture2D normalTexture : register(t2);
SamplerState defaultSampler : register(s0);

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCamera;
    float4x4 toProjection;
    float4 toDirectionalLight;
    float4 directionalLightColor;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
}

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
    float4 myNormal : NORMAL;
    float4 myTangent : TANGENT;
    float4 myBiNormal : BITANGENT;
    float2 myUV : UV;
};

struct PixelOutPut
{
    float4 myColor : SV_TARGET;
};

