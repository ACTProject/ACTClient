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
    float lifetimeRatio = saturate(time / lifetime)/2; // 0.0 ~ 1.0
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

    // UV 좌표 방향 결정
    float uv_x = isLeft == 1.0f ? input.uv.x : (1.0f - input.uv.x);

    if (time < delaytime)
    {
        discard;
    }
    else
    {
    // UV 좌표의 x 값에 따라 알파를 점차 증가
        float appearThreshold = (time - delaytime) / (fadeStart - delaytime); // 0에서 1까지 변화
        if (uv_x > appearThreshold)
        {
            discard;
        }
    }
    if (time > fadeStart)
    {
    // UV 좌표의 x 값에 따라 알파를 점차 감소
        float fadeThreshold = (time - fadeStart) / (lifetime - fadeStart); // 0에서 1까지 변화
        if (uv_x < fadeThreshold)
        {
            alpha = 1.0f - fadeThreshold; // 점차 감소
        }
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