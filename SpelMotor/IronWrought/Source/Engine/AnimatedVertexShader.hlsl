#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
    VertexToPixel returnValue;
     
    returnValue.myPosition = input.myPosition;

  
    float4 vWeights = input.myBoneWeight;
    uint4 vBones = uint4((uint) input.myBoneID.x, (uint) input.myBoneID.y, (uint) input.myBoneID.z, (uint) input.myBoneID.w);
    returnValue.myPosition = mul(returnValue.myPosition, toWorld);
    
    float4 skinnedPos = 0;
    uint iBone = 0;
    float fWeight = 0;
    
    iBone = 0;
    fWeight = vWeights.x;
    
    /// Bone 0
    iBone = vBones.x;
    fWeight = vWeights.x;
    float4 pos = input.myPosition;
    skinnedPos += fWeight * mul(pos, myBones[iBone]);
    
    /// Bone 1
    iBone = vBones.y;
    fWeight = vWeights.y;
    skinnedPos += fWeight * mul(pos, myBones[iBone]);
    
    /// Bone 2
    iBone = vBones.z;
    fWeight = vWeights.z;
    skinnedPos += fWeight * mul(pos, myBones[iBone]);
    
    /// Bone 3
    iBone = vBones.w;
    fWeight = vWeights.w;
    skinnedPos += fWeight * mul(pos, myBones[iBone]);
    
    input.myPosition.x = skinnedPos.x;
    input.myPosition.y = skinnedPos.y;
    input.myPosition.z = skinnedPos.z;
    input.myPosition.w = skinnedPos.w;
    
    float4 vertexObjectPos = input.myPosition.xyzw;
    float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
    float4 vertexViewPos = mul(toCamera, vertexWorldPos);
    float4 vertexProjectionPos = mul(toProjection, vertexViewPos);
    
    returnValue.myPosition = vertexProjectionPos;
    
    float3x3 toWorldRotation = (float3x3) toWorld;
    float3 vertexWorldNormal = mul(toWorldRotation, input.myNormal.xyz);
    float3 vertexWorldTangent = mul(toWorldRotation, input.myTangent.xyz);
    float3 vertexWorldBinormal = mul(toWorldRotation, input.myBiNormal.xyz);
    
    returnValue.myWorldPosition = vertexWorldPos;
    returnValue.myNormal = float4(vertexWorldNormal, 0);
    returnValue.myTangent = float4(vertexWorldTangent, 0);
    returnValue.myBiNormal = float4(vertexWorldBinormal, 0);
    returnValue.myUV = input.myUV;
      
    return returnValue;
}