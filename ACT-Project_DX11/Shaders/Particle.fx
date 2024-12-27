#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"
cbuffer ParticleBuffer
{
    float time;
    float lifetime;
    float fadeStart;
};
struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float2 scale : SCALE;
};

struct V_OUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

V_OUT VS(VertexInput input)
{
    V_OUT output;

    float4 position = mul(input.position, W);

    float3 up = float3(0, 1, 0);
    float3 forward = position.xyz - CameraPosition();
    float3 right = normalize(cross(up, forward));

    // 시간에 따른 스케일 증가 계산
    float lifetimeRatio = saturate(time / lifetime); // 0.0 ~ 1.0
    float scaleFactor = 1.0f + lifetimeRatio; // 1.0에서 시작해 2.0까지 커짐

    float2 currentScale = input.scale * scaleFactor;

    // 위치 변환에 스케일 적용
    position.xyz += (input.uv.x - 0.5f) * right * currentScale.x;
    position.xyz += (1.0f - input.uv.y - 0.5f) * up * currentScale.y;
    position.w = 1.0f;

    output.position = mul(mul(position, V), P);

    output.uv = input.uv;

    return output;
}

float4 PS(V_OUT input) : SV_Target
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
   
    float alpha = 1.0f;
    if (time > fadeStart)
    {
        alpha = 1.0f - (time - fadeStart) / (lifetime - fadeStart);
    }
    
    color.a *= alpha;
    
    if (color.a <= 0.8f)
    {
        discard;
    }
    
    return color;
}

technique11 T0
{
	PASS_VP(P0, VS_InstancingMesh_Particle, PS)
	PASS_VP(P1, VS_InstancingModel, PS)
	PASS_VP(P2, VS_InstancingAnimation, PS)
};

technique11 T1 // 싱글 렌더링
{
	PASS_VP(P0, VS_Mesh_Particle, PS)
	PASS_VP(P1, VS_Model, PS)
	PASS_VP(P2, VS_Animation, PS)
	//PASS_VP(P3, VS_MeshColor, PS)
};

technique11 T2 // 와이어프레임 인스턴싱
{
	PASS_RS_VP(P0, FillModeWireFrame, VS_InstancingMesh_Particle, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS_InstancingModel, PS)
	PASS_RS_VP(P2, FillModeWireFrame, VS_InstancingAnimation, PS)
};

technique11 T3 // 와이어프레임 싱글
{
	PASS_RS_VP(P0, FillModeWireFrame, VS_Mesh_Particle, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS_Model, PS)
	PASS_RS_VP(P2, FillModeWireFrame, VS_Animation, PS)
	//PASS_RS_VP(P3, FillModeWireFrame, VS_MeshColor, PS)
};

technique11 T4 // 매쉬 알파블렌딩
{
	PASS_BS_VP(P0, AlphaBlend, VS_Mesh_Particle, PS) // 기본적인 알파 블렌딩
	PASS_BS_VP(P1, AlphaBlendAlphaToCoverageEnable, VS_Mesh_Particle, PS) // 멀티샘플링 환경 알파블렌딩
	PASS_BS_VP(P2, AdditiveBlend, VS_Mesh_Particle, PS) // 애드블렌딩
	PASS_BS_VP(P3, AdditiveBlendAlphaToCoverageEnable, VS_Mesh_Particle, PS) // 멀티샘플링 환경 애드블렌딩
};

technique11 T5 // Debug Collider Rendering
{
	PASS_VP(P0, VS_Collision, PS_Collision)
};
