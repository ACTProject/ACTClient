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

// 시간에 따른 위로 이동
    float upwardSpeed = 20.f; // 위로 이동하는 속도 (조절 가능)
    position.y += time * upwardSpeed;

// 흔들림 효과 추가
    float oscillationIntensity = 2.f; // 흔들림 강도
    float oscillationSpeed = 2.0f; // 흔들림 속도 (조절 가능)
    float oscillationX = sin(time * oscillationSpeed) * oscillationIntensity;
    float oscillationY = cos(time * oscillationSpeed * 1.5f) * oscillationIntensity;

// 위치 변환 + 흔들림 효과 적용
    position.xyz += (input.uv.x - 0.5f) * right + oscillationX * right; // X축 변환 및 흔들림
    position.xyz += (1.0f - input.uv.y - 0.5f) * up + oscillationY * up; // Y축 변환 및 흔들림
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