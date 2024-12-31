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

    // 월드 변환
    float4 position = mul(input.position, W);

    // 카메라 방향 계산
    float3 up = float3(0, 1, 0);
    float3 forward = normalize(position.xyz - CameraPosition()); // 카메라를 향한 빌보드 방향
    float3 right = normalize(cross(up, forward));

    // 옆에서 보이는 문제 보정 (lerp 사용)
    float3 adjustedForward = lerp(forward, float3(0, 0, 1), 0.5f); // 0.5f는 보정 강도 (0~1)
    float3 adjustedRight = normalize(cross(up, adjustedForward));

    // 빌보드 평면 조정
    position.xyz += (input.uv.x - 0.5f) * adjustedRight * input.scale.x;
    position.xyz += (1.0f - input.uv.y - 0.5f) * up * input.scale.y;
    position.w = 1.0f;

    // 카메라 변환
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
    
    if (color.a <= 0.3f)
    {
        discard;
    }
    
    return color;
}

technique11 T0
{
    pass P0
    {
        //SetRasterizerState(FillModeWireFrame);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        //SetDepthStencilState(NoDepthWrite, 0);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};