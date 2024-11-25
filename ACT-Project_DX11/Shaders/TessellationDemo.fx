//#include "00. Global.fx"
//#include "00. Light.fx"
//#include "00. Render.fx"
//
//// Hull Shader : �������� �ܰ�, �������� ���, �󸶳� ���� ���� ����
//// �潦�̴����� ���� �ſ� ���� Vertex���� ���� ����
//// �� Vertex�� ���� Domain Shader���� ���� ����
//// ���� = ��Ʈ�� ����Ʈ, ��ġ = ��Ʈ�� ����Ʈ�� ���� �ٰ��� ��ü
//
//struct VertexIn
//{
//    float3 position : POSITIONT;
//    float2 uv : TESCOORD0;
//};
//
//struct VertexOut
//{
//    float3 position : POSITIONT;
//    float2 uv : TESCOORD0;
//};
//
//Texture2D heightMap;
//
//float heightScale = 10.0f;
//
//VertexOut VS(VertexIn input)
//{
//    VertexOut output;
//
//    output.position = mul(float4(input.position, 1.0f), W).xyz;
//    output.uv = input.uv;
//
//    return output;
//}
//
//struct PatchTess
//{
//    float edge[4] : SV_TessFactor; //���� 4���� ����ȭ �� ex) ------O------O------O-------
//    float inside[2] : SV_InsideTessFactor; // ���� ����
//};
//
//PatchTess ConstantHS(InputPatch<VertexOut, 4> p, uint patchId : SV_PrimitiveID)
//{
//    PatchTess tess;
//
//    // �� ���� �ؼ����̼� ����
//    tess.edge[0] = 8.0f;//Ŭ���� ���� ���� 64����
//    tess.edge[1] = 8.0f;
//    tess.edge[2] = 8.0f;
//    tess.edge[3] = 8.0f;
//
//    // ����
//    tess.inside[0] = 8.0f;
//    tess.inside[1] = 8.0f;
//
//    return tess;
//}
//
//struct HullOut
//{
//    float3 position : POSITIONT; // ��Ʈ�� ����Ʈ ��ġ
//};
//
//
//[domain("quad")]//��ġ�� ����
//[partitioning("integer")]// ���� ���� (integer : 1..64 )
//[outputtopology("triangle_cw")]// �׼��������� ��� �⺻ ���� ���� : �ﰢ�� �ð�������� (clock_wise)
//[outputcontrolpoints(4)] // �Էµ� ��Ʈ�� ����Ʈ ��
//[patchconstantfunc("ConstantHS")] // �Լ��� �̸�
//[maxtessfactor(64.0f)] // ��� �׼����̼� ����� ���� �� ���̴��� ��ȯ�� �ִ�
//HullOut HS(InputPatch<VertexOut, 4> p,
//    uint i : SV_OutputControlPointID,
//    uint patchId : SV_PrimitiveID)
//{
//    HullOut output;
//    output.position = p[i].position; // �Է� ��Ʈ�� ����Ʈ�� �״�� ���
//
//    return output;
//}
//
//struct DomainOUt
//{
//    float4 position : SV_Position; // ���� ��ǥ
//    float2 uv : TEXCOORD0;
//};
//
////[domain("quad")]
////DomainOUt DS(PatchTess patchTess,
////    float2 uv : SV_DomainLocation,
////    const OutputPatch<HullOut, 4> quad)
////{
////    DomainOUt output;
////
////    // ���� ���� ������ �̿��� �簢�� ��ġ ���
////    float3 position = lerp(lerp(quad[0].position, quad[1].position, uv.x), // Linear Interpolation : �� ���� ���������Ѵ�.
////        lerp(quad[3].position, quad[2].position, uv.x), uv.y);
////
////    float height = heightMap.Sample(LinearSampler, uv).r * heightScale;
////    position.y += height;
////
////    output.position = mul(float4(position, 1.0f), VP);
////    output.uv = uv;
////
////    return output;
////}
//
//[domain("quad")]
//DomainOUt DS(PatchTess patchTess,
//    float2 uv : SV_DomainLocation,
//    const OutputPatch<HullOut, 4> quad)
//{
//    DomainOUt output;
//
//    // ���� ���� ������ �̿��� �簢�� ��ġ ���
//    float3 position = lerp(lerp(quad[0].position, quad[1].position, uv.x),
//        lerp(quad[3].position, quad[2].position, uv.x), uv.y);
//
//    // UV ���� Ȯ�� �� Ŭ����
//    float2 clampedUV = clamp(uv, 0.0f, 1.0f);
//
//    // ���̸� ���ø�
//    float height = clamp(heightMap.Sample(LinearSampler, clampedUV).r * heightScale, 0.0f, 10.0f);
//    position.y += height;
//
//    // ���� ���
//    output.position = mul(float4(position, 1.0f), VP);
//    output.uv = clampedUV;
//
//    return output;
//}
//float4 PS(MeshOutput input) : SV_TARGET
//{
//    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
//    return color;
//}
//
//technique T0
//{
//    pass P0
//    {
//        SetVertexShader(CompileShader(vs_5_0, VS()));
//        SetHullShader(CompileShader(hs_5_0, HS()));
//        SetDomainShader(CompileShader(ds_5_0, DS()));
//        SetPixelShader(CompileShader(ps_5_0, PS()));
//    }
//}

// TessellationDemo.fx

#ifndef _TESSELLATION_DEMO_FX_
#define _TESSELLATION_DEMO_FX_

#include "00. Global.fx"
#include "00. Light.fx"

///////////////
// Constant Buffers
///////////////
cbuffer TessellationBuffer
{
    float tessellationFactor; // �⺻ �׼����̼� ����
    float minDistance;        // �ּ� �Ÿ�
    float maxDistance;        // �ִ� �Ÿ�
    float padding;
};

///////////////
// Vertex Shader
///////////////
struct TessVertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct TessControlPoint
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

TessControlPoint VS_Tess(TessVertexInput input)
{
    TessControlPoint output;
    output.position = mul(input.position, W); // ���� ��ǥ ��ȯ
    output.uv = input.uv;
    return output;
}

///////////////
// Hull Shader
///////////////
struct HullOutput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};
struct PatchConstants
{
    float TessellationFactor[3] : SV_TessFactor;        // �ﰢ���� �� ���� ���� �׼����̼� ����
    float InsideTessellationFactor : SV_InsideTessFactor; // �ﰢ�� ���� �׼����̼� ����
};

// ī�޶� ��ġ ���� �Լ�
float3 GetCameraPosition()
{
    return float3(VInv._41, VInv._42, VInv._43); // VInv ��Ʈ������ ��ġ ���� ���
}

PatchConstants HS_CalculatePatchConstants(InputPatch<TessControlPoint, 3> patch)
{
    float3 cameraPosition = GetCameraPosition();

    // ��ġ�� �߽� ��ǥ ���
    float3 worldPosition = (patch[0].position.xyz + patch[1].position.xyz + patch[2].position.xyz) / 3.0f;
    float distance = length(worldPosition - cameraPosition);

    // �Ÿ� ��� �׼����̼� ���� ���
    float tessFactor = saturate(1.0f - (distance - minDistance) / (maxDistance - minDistance)) * tessellationFactor;

    PatchConstants output;
    output.TessellationFactor[0] = tessFactor; // �ﰢ���� ù ��° ��
    output.TessellationFactor[1] = tessFactor; // �ﰢ���� �� ��° ��
    output.TessellationFactor[2] = tessFactor; // �ﰢ���� �� ��° ��
    output.InsideTessellationFactor = tessFactor; // �ﰢ�� ���� ����
    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_CalculatePatchConstants")]
HullOutput HS_Tess(InputPatch<TessControlPoint, 3> patch, uint i : SV_OutputControlPointID)
{
    HullOutput output;
    output.position = patch[i].position;
    output.uv = patch[i].uv;
    return output;
}

///////////////
// Domain Shader
///////////////
struct DomainOutput
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

[domain("tri")]
DomainOutput DS_Tess(PatchConstants patch, float3 bary : SV_DomainLocation, const OutputPatch<HullOutput, 3> controlPoints)
{
    float4 pos = bary.x * controlPoints[0].position + bary.y * controlPoints[1].position + bary.z * controlPoints[2].position;

    DomainOutput output;
    output.position = mul(pos, VP); // VP ��ȯ
    output.worldPosition = pos.xyz; // ���� ��ǥ
    output.uv = bary.x * controlPoints[0].uv + bary.y * controlPoints[1].uv + bary.z * controlPoints[2].uv;
    output.normal = float3(0, 1, 0); // �⺻ ����
    return output;
}

///////////////
// Pixel Shader
///////////////
float4 PS_Tess(DomainOutput input) : SV_TARGET
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
    return color;
}

technique11 T_Tessellation
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Tess()));
        SetHullShader(CompileShader(hs_5_0, HS_Tess()));
        SetDomainShader(CompileShader(ds_5_0, DS_Tess()));
        SetPixelShader(CompileShader(ps_5_0, PS_Tess()));
    }
};

#endif
