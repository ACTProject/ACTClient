#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"

//cbuffer BarBuffer
//{
//    float stretchStart;
//    float stretchEnd;
//    float stretchFactor;
//};
const float scaleFactor = 2.0f;

MeshOutput VS_InstancingUIMesh(InstancingVertexMesh input)
{
    MeshOutput output;

    // position의 xyz 성분에만 스케일 적용
    float4 scaledPosition = float4(input.position.xyz * scaleFactor, input.position.w);

    // 월드 변환
    output.position = mul(scaledPosition, input.world);

    // 월드 좌표 저장
    output.worldPosition = output.position;

    // 뷰-프로젝션 변환
    output.position = mul(output.position, VP);

    // UV 및 노멀 전달
    output.uv = input.uv;
    output.normal = normalize(input.normal); // 노멀은 정규화

    return output;

    
    //MeshOutput output;

    //output.position = mul(input.position, input.world); // W
    //output.worldPosition = output.position;
    //output.position = mul(output.position, VP);
    //output.uv = input.uv;
    //output.normal = input.normal;

    //return output;
}
MeshOutput VS_UIMesh(VertexTextureNormal input)
{
    MeshOutput output;

    // position의 xyz 성분에만 스케일 적용
    float4 scaledPosition = float4(input.position.xyz * scaleFactor, input.position.w);

    // 월드 변환
    output.position = mul(scaledPosition, W);

    // 월드 좌표 저장
    output.worldPosition = output.position;

    // 뷰-프로젝션 변환
    output.position = mul(output.position, VP); 

    // UV 및 노멀 전달
    output.uv = input.uv;
    output.normal = normalize(input.normal); // 노멀은 정규화

    return output;
}

float4 PS_Collision() : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 PS(MeshOutput input) : SV_TARGET
{

	//float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);

    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

    return color;
}

technique11 T0 // 인스턴싱 렌더링
{
	PASS_VP(P0, VS_InstancingUIMesh, PS)
	PASS_VP(P1, VS_InstancingModel, PS)
	PASS_VP(P2, VS_InstancingAnimation, PS)
};

technique11 T1 // 싱글 렌더링
{
	PASS_VP(P0, VS_UIMesh, PS)
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
};