#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"


float4 PS_Collision() : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 PS_Octree() : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

float4 ComputeDiffuseLight(float3 normal, float4 color)
{
	// 디퓨즈 라이트
    float fDot = max(0.7, dot(normal, normalize(float3(0,-0.6,-1))));
    color = color * fDot;
    return float4(color.rgb, 1.0f);
}

float4 PS(MeshOutput input) : SV_TARGET
{
    float4 colors = DiffuseMap.Sample(LinearSampler, input.uv);
    float4 color = ComputeDiffuseLight(input.normal, colors);
    //NormalMap.Sample(input.uv);
    
    //Shadow
    float ShadowAmount = 0.0f;
    float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;
    const float delta = 1.0f / 4096;
    const int g_iNumKernel = 5;
    int iHalf = (g_iNumKernel - 1) / 2;
    
    for (int v = -iHalf; v <= iHalf; v++)
    {
        for (int u = -iHalf; u <= iHalf; u++)
        {
            float2 vOffset = float2(u * delta, v * delta);
            ShadowAmount += ShadowDepthTexture.SampleCmpLevelZero(SamComShadowMap,
									ShadowTexColor.xy + vOffset, ShadowTexColor.z);
        }
    }
    ShadowAmount /= g_iNumKernel * g_iNumKernel;
    
    float4 FinalColor = float4(color.rgb * max(0.7f, ShadowAmount), color.a);
    
    
    float distance = length(input.worldPosition - CameraPosition());
    float start = 60.f;
    float end = 160.f;
    float fogFactor = saturate((end - distance) / (end - start));
	
    float4 fogColor = float4(0.1, 0.4, 0.8, 1.0);
    float maxFog = 0.0;

    if (fogFactor <= maxFog)
    {
        FinalColor.rgb = lerp(fogColor.rgb, FinalColor.rgb, maxFog);
    }
    else
    {
        FinalColor.rgb = lerp(fogColor.rgb, FinalColor.rgb, fogFactor);
    }
    
    
    if (FinalColor.a < 0.3f)
        discard;
    
    return FinalColor;    	
}

technique11 T0 // 인스턴싱 렌더링
{
	PASS_VP(P0, VS_InstancingMesh, PS)
	PASS_VP(P1, VS_InstancingModel, PS)
};

technique11 T1 // 싱글 렌더링
{
	PASS_VP(P0, VS_Mesh, PS)
	PASS_VP(P1, VS_Model, PS)
};

technique11 T5 // Debug Collider Rendering
{
	PASS_VP(P0, VS_Collision, PS_Collision)
    PASS_VP(P1, VS_Octree, PS_Octree)
};