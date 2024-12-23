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
    
    //Shadow
    float ShadowAmount = 0.0f;
    //float4 shadowDepth = float4(input.TexShadow.xy, input.TexShadow.z + Bias.x,1.0f);
    //float3 ShadowTexColor = shadowDepth.xyz / shadowDepth.w;
    float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;
    //float4 tex = ShadowDepthTexture.Sample(LinearSampler, ShadowTexColor.xy);
    //tex == 
    const float delta = 1.0f / 4096;
    const int g_iNumKernel = 3;
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
    float4 fColor = float4(ShadowAmount, ShadowAmount, ShadowAmount, 1.0f);
    
    
    
    
    //ShadowAmount = ShadowDepthTexture.SampleCmpLevelZero(SamComShadowMap,
	//								ShadowTexColor.xy, ShadowTexColor.z).r;
    float4 FinalColor = color * max(0.5f, ShadowAmount);
    FinalColor.a = 1.0f;
    
    return FinalColor;
    
    
    //float3 shadowTexColor = input.TexShadow.xyz /= input.TexShadow.w;
    //float depth = shadowTexColor.z;
    //const float dx = 1.0f / 2048.0f;
    //return ShadowDepthTexture.SampleCmpLevelZero(SamComShadowMap, shadowTexColor.xy, depth).r;
    

    
    //float distance = length(input.worldPosition - CameraPosition());
	
    //float start = 60.f;
    //float end = 140.f;
    //float fogFactor = saturate((end - distance) / (end - start));
	
    //float4 fogColor = float4(0.1, 0.6, 0.9, 1.0);
    //float maxFog = 0.0;
    //if (fogFactor <= maxFog)
    //{
    //    color.rgb = lerp(fogColor.rgb, color.rgb, maxFog);
    //}
    //else
    //{
    //    color.rgb = lerp(fogColor.rgb, color.rgb, fogFactor);
    //}
    
    //if (color.a < 0.3f)
    //    discard;
    
    //return color;
    
}


technique11 T0 // 인스턴싱 렌더링
{
	PASS_VP(P0, VS_InstancingMesh, PS)
	PASS_VP(P1, VS_InstancingModel, PS)
	PASS_VP(P2, VS_InstancingAnimation, PS)
};

technique11 T1 // 싱글 렌더링
{
	PASS_VP(P0, VS_Mesh, PS)
	PASS_VP(P1, VS_Model, PS)
	PASS_VP(P2, VS_Animation, PS)
	//PASS_VP(P3, VS_MeshColor, PS)
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