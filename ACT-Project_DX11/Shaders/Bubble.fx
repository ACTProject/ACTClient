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

   // 시간 비율 계산
    float lifetimeRatio = saturate(time / lifetime); // 0.0 ~ 1.0
    float fadeRatio = saturate((time - fadeStart) / (lifetime - fadeStart)); // 0.0 ~ 1.0, 작아지는 구간

    float oscillation;

    // 커지는 구간
    if (time < fadeStart)
    {
        float growRatio = saturate(time / fadeStart); // 0.0 ~ 1.0
        oscillation = sin(growRatio * 3.14159f / 1.3f); // 0.0 ~ 1.0
    }
    else // fadeStart 이후 작아지는 구간
    {
        oscillation = cos(fadeRatio * 3.14159f / 2.0f); // 1.0 ~ 0.0
    }

    // 스케일 변화
    float minScale = 0.1f; // 작아지는 구간의 최종 크기
    float scaleFactor = minScale + oscillation * (1.0f - minScale); // 1.0 ~ minScale로 감소
    float2 currentScale = input.scale * scaleFactor;

    // 시간에 따른 위로 이동
    float upwardSpeed = 1.2f; // 위로 이동하는 속도 (조절 가능)
    position.y += lifetimeRatio * upwardSpeed;
    
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
    //if (time > fadeStart)
    //{
    //    alpha = 1.0f - (time - fadeStart) / (lifetime - fadeStart);
    //}
    
    color.a *= alpha;
    
    if (color.a <= 0.8f)
    {
        discard;
    }
    
    return color;
}

technique11 T0
{
    pass P0
    {
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        //SetDepthStencilState(NoDepthWrite, 0);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};