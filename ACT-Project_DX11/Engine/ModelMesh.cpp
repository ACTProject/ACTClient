#include "pch.h"
#include "ModelMesh.h"


shared_ptr<ModelBone> ModelBone::AddDummyBone(const wstring& dummyName, Matrix dummyTransform, int32 dummyIndex)
{
	auto dummyBone = make_shared<ModelBone>();
	dummyBone->isDummy = true;
	dummyBone->name = dummyName;
	dummyBone->index = dummyIndex;
	dummyBone->parentIndex = index; // 현재 본을 부모로 설정
	dummyBone->parent = shared_from_this();
	dummyBone->transform = shared_from_this()->transform;

	children.push_back(dummyBone);

	return dummyBone;
}

void ModelMesh::AttachToDummyBone(shared_ptr<ModelBone> dummyBone)
{
	bone = dummyBone; // 본을 더미 본에 연결
	boneIndex = dummyBone->index;

	int parentBoneIndex = bone->parent ? bone->parentIndex : bone->index;

	// 해당 본 인덱스와 가중치를 모든 정점에 적용
	for (auto& vertex : geometry->GetVertices())
	{
		// 더미 본을 부모 본에 고정
		vertex.blendIndices = Vec4(static_cast<float>(parentBoneIndex), 0, 0, 0);
		vertex.blendWeights = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	}
}

void ModelMesh::CreateBuffers()
{
	vertexBuffer = make_shared<VertexBuffer>();
	vertexBuffer->Create(geometry->GetVertices());
	indexBuffer = make_shared<IndexBuffer>();
	indexBuffer->Create(geometry->GetIndices());
}