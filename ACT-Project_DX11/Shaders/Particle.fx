#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"

cbuffer ParticleBuffer
{
    float time;
    float lifetime;
    float fadeStart;
};

float4 PS_Collision() : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
MeshOutput VS_InstancingMesh_Particle(InstancingVertexMesh input)
{
    MeshOutput output;

    float4 position = mul(input.position, input.world);
    
    float3 up = float3(0, 1, 0);
    float3 forward = position.xyz - CameraPosition();
    float3 right = normalize(cross(up, forward));
    
    position.xyz += (input.uv.x - 0.5f) * right;
    position.xyz += (1.0f - input.uv.y - 0.5f) * up;
    position.w = 1.0f;
    
    output.worldPosition = position;
    output.position = mul(position, VP);
    output.uv = input.uv;
    output.normal = input.normal;

    return output;
}

// ************** SingleMeshRender ****************

MeshOutput VS_Mesh_Particle(VertexTextureNormal input)
{
        MeshOutput output;

    // 월드 공간 위치로 변환
        float4 position = mul(input.position, W);

    // 카메라 위치와 방향 계산
        float3 cameraPosition = CameraPosition();
        float3 forward = normalize(cameraPosition - position.xyz); // 카메라 -> 입자 방향
        float3 right = normalize(cross(float3(0, 1, 0), forward)); // 카메라 오른쪽 벡터
        float3 up = cross(forward, right); // 카메라 위쪽 벡터

    // UV 좌표를 기준으로 빌보드 정점 위치 계산
        float2 uvOffset = input.uv - float2(0.5, 0.5); // UV 중심 기준으로 [-0.5, 0.5]
        position.xyz += uvOffset.x * right; // 오른쪽 방향 오프셋
        position.xyz += uvOffset.y * up; // 위쪽 방향 오프셋

        position.w = 1.0f; // w 컴포넌트 설정

    // 출력 설정
        output.worldPosition = position; // 월드 좌표 저장
        output.position = mul(position, VP); // 클립 공간 변환
        output.uv = input.uv; // UV 전달
        output.normal = forward; // 입자의 정면 방향 설정

        return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
	//float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);

    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
   
    float alpha = 1.0f;
    if (time > fadeStart)
    {
        alpha = 1.0f - (time - fadeStart) / (lifetime - fadeStart);
    }
    
    color.a *= alpha;
    
    return color;
}


technique11 T0 // 인스턴싱 렌더링
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