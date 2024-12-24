#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float2 scale : SCALE;
};

struct V_OUT
{
    float4 position : SV_POSITION;
    float4 world : POSITION1;
    float2 uv : TEXCOORD;
};

V_OUT VS(VertexInput input)
{
    V_OUT output;

    output.world = input.position;
    
    float3 up = float3(0, 1, 0);
    //float3 forward = float3(0, 0, 1); // TODO
    float3 forward = input.position.xyz - CameraPosition(); // BillBoard
    float3 right = normalize(cross(up, forward));
    
    float3 offsetX = (input.uv.x - 0.5f) * right * input.scale.x;
    float3 offsetY = (1.0f - input.uv.y - 0.5f) * up * input.scale.y;
    
    
    float4 wavePos = float4(input.position.x,0, input.position.z, input.position.w);
    wavePos.y += abs(offsetY.y);
    
    wavePos.xyz += offsetX;
    wavePos.xyz += offsetY;
    wavePos.w = 1.0f;
    
    wavePos = mul(wavePos, Wave);

    wavePos.y -= abs(offsetY.y);
    wavePos.y += input.position.y;
    
    
    input.position.xyz = wavePos.xyz;
    input.position.w = 1.0f;
    
    
    output.position = mul(mul(input.position, V), P);

    output.uv = input.uv;

    return output;
}

float4 PS(V_OUT input) : SV_Target
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
   
    float distance = length(input.world.xyz - CameraPosition());
	
    float start = 50.f;
    float end = 90.f;
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

technique11 T0
{
    pass P0
    {
        SetBlendState(AlphaBlend, float4(0,0,0,0), 0xFF);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};