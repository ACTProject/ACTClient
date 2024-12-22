#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"



struct ShadowDebugPS
{
    float4 p : SV_POSITION;
    float4 TexShadow : TEXCOORD1;
};

ShadowDebugPS VS_ShadowMesh(VertexTextureNormalTangent input)
{
    ShadowDebugPS output = (ShadowDebugPS) 0;
    output.p = mul(float4(input.position.xyz, 1.0f), W);
    output.TexShadow = mul(float4(output.p.xyz, 1.0f), S);
    output.p = mul(output.p, V);
    output.p = mul(output.p, P);
    
    return output;
}

ShadowDebugPS VS_ShadowModel(VertexTextureNormalTangentBlend input)
{
    ShadowDebugPS output = (ShadowDebugPS)0;
    output.p = mul(float4(input.position.xyz, 1.0f), W);
    output.TexShadow = mul(float4(output.p.xyz, 1.0f), S);
    output.p = mul(output.p, V);
    output.p = mul(output.p, P);
    
    return output;
}


technique11 T0
{
    PASS_SHADOW(P0, AlphaBlend, Depth, VS_ShadowModel)
    PASS_SHADOW(P1, AlphaBlend, Depth, VS_ShadowMesh)
};


technique11 T1
{
    PASS_SHADOW(P0, AlphaBlend, Depth, VS_ShadowModel)
    PASS_SHADOW(P1, AlphaBlend, Depth, VS_ShadowMesh)
};