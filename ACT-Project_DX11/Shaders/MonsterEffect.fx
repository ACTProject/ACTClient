#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"
cbuffer ParticleBuffer
{
    float time;
    float lifetime;
    float fadeStart;
    float delaytime;
    bool isLeft;
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
    float3 targetColor = float3(1.0f, 1.0f, 0.5f); // 연한 노랑색
    float blendFactor = 1.0f; // 색상 보간 비율

    if (time < delaytime)
    {
        discard;
    }

    if (time > fadeStart)
    {
    // 보간 비율을 계산 (0.0에서 1.0으로 증가)
        blendFactor = (time - fadeStart) / (lifetime - fadeStart);
    }

// RGB 값을 연한 노랑으로 점진적으로 보간
    color.rgb = lerp(color.rgb, targetColor, blendFactor);

    color.a *= alpha;

    if (blendFactor >= 1.0f)
    {
        discard; // 완전히 연한 노랑이 되면 픽셀 제거 (선택 사항)
    }
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
        //SetRasterizerState(FillModeWireFrame);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        //SetDepthStencilState(NoDepthWrite, 0);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};