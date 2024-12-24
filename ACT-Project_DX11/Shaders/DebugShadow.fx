#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

MeshOutput VS_Debug(VertexTextureNormal input)
{
    MeshOutput output = (MeshOutput) 0;
    output.position = mul(input.position, W);
    output.uv = input.uv;
    

    return output;
}

float4 PS_Debug(MeshOutput input) : SV_Target
{
    float4 texColor = ShadowDepthTexture.Sample(samLinear, input.uv);

    return float4(texColor.rrr, 1.0f) * 100;
}

technique11 T0 
{
	PASS_VP(P0, VS_Debug, PS_Debug)
	PASS_VP(P1, VS_Debug, PS_Debug)
};

technique11 T1 
{
	PASS_VP(P0, VS_Debug, PS_Debug)
	PASS_VP(P1, VS_Debug, PS_Debug)
};

technique11 T2 
{
	PASS_VP(P0, VS_Debug, PS_Debug)
	PASS_VP(P1, VS_Debug, PS_Debug)
};

technique11 T3 
{
	PASS_VP(P0, VS_Debug, PS_Debug)
	PASS_VP(P1, VS_Debug, PS_Debug)
};

technique11 T4 
{
	PASS_VP(P0, VS_Debug, PS_Debug)
	PASS_VP(P1, VS_Debug, PS_Debug)
};