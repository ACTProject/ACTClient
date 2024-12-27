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

float4 PS(MeshOutput input) : SV_TARGET
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
    
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
    
    
    //float distance = length(input.worldPosition - CameraPosition());
    //float start = 60.f;
    //float end = 140.f;
    //float fogFactor = saturate((end - distance) / (end - start));
	
    //float4 fogColor = float4(0.1, 0.6, 0.9, 1.0);
    //float maxFog = 0.0;
    //if (fogFactor <= maxFog)
    //{
    //    FinalColor.rgb = lerp(fogColor.rgb, FinalColor.rgb, maxFog);
    //}
    //else
    //{
    //    FinalColor.rgb = lerp(fogColor.rgb, FinalColor.rgb, fogFactor);
    //}
    
    
    if (FinalColor.a < 0.3f)
        discard;
    
    return FinalColor;
    
	

}

float4 PS_NoShadow(MeshOutput input) : SV_TARGET
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
    
    float distance = length(input.worldPosition - CameraPosition());
	
    float start = 60.f;
    float end = 140.f;
    float fogFactor = saturate((end - distance) / (end - start));
	
    float4 fogColor = float4(0.1, 0.6, 0.9, 1.0);
    float maxFog = 0.0;
    if (fogFactor <= maxFog)
    {
        color.rgb = lerp(fogColor.rgb, color.rgb, maxFog);
    }
    else
    {
        color.rgb = lerp(fogColor.rgb, color.rgb, fogFactor);
    }
    
    if (color.a < 0.3f)
        discard;
    
    return color;
}


technique11 T0 // 인스턴싱 렌더링
{
	PASS_VP(P0, VS_InstancingMesh, PS)
	PASS_VP(P1, VS_InstancingModel, PS)
	PASS_VP(P2, VS_InstancingAnimation, PS)
	PASS_VP(P3, VS_InstancingMesh, PS_NoShadow)
	PASS_VP(P4, VS_InstancingModel, PS_NoShadow)
	PASS_VP(P5, VS_InstancingAnimation, PS_NoShadow)

};

technique11 T1 // 싱글 렌더링
{
	PASS_VP(P0, VS_Mesh, PS)
	PASS_VP(P1, VS_Model, PS)
	PASS_VP(P2, VS_Animation, PS)
	PASS_VP(P3, VS_Mesh, PS_NoShadow)
	PASS_VP(P4, VS_Model, PS_NoShadow)
    PASS_VP(P5, VS_Animation, PS_NoShadow)

};

technique11 T2 // 와이어프레임 인스턴싱
{
	PASS_RS_VP(P0, FillModeWireFrame, VS_InstancingMesh, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS_InstancingModel, PS)
	PASS_RS_VP(P2, FillModeWireFrame, VS_InstancingAnimation, PS)
};

technique11 T3 // 와이어프레임 싱글
{
	PASS_RS_VP(P0, FillModeWireFrame, VS_Mesh, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS_Model, PS)
	PASS_RS_VP(P2, FillModeWireFrame, VS_Animation, PS)
	//PASS_RS_VP(P3, FillModeWireFrame, VS_MeshColor, PS)
};

technique11 T4 // 매쉬 알파블렌딩
{
	PASS_BS_VP(P0, AlphaBlend, VS_Mesh, PS) // 기본적인 알파 블렌딩
	PASS_BS_VP(P1, AlphaBlendAlphaToCoverageEnable, VS_Mesh, PS) // 멀티샘플링 환경 알파블렌딩
	PASS_BS_VP(P2, AdditiveBlend, VS_Mesh, PS) // 애드블렌딩
	PASS_BS_VP(P3, AdditiveBlendAlphaToCoverageEnable, VS_Mesh, PS) // 멀티샘플링 환경 애드블렌딩
};

technique11 T5 // Debug Collider Rendering
{
	PASS_VP(P0, VS_Collision, PS_Collision)
    PASS_VP(P1, VS_Octree, PS_Octree)
};