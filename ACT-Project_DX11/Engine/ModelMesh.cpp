#include "pch.h"
#include "ModelMesh.h"


shared_ptr<ModelBone> ModelBone::AddDummyBone(const wstring& dummyName, Matrix dummyTransform, int32 dummyIndex)
{
	auto dummyBone = make_shared<ModelBone>();
	dummyBone->isDummy = true;
	dummyBone->name = dummyName;
	dummyBone->index = dummyIndex;
	dummyBone->parentIndex = index; // ���� ���� �θ�� ����
	dummyBone->parent = shared_from_this();
	dummyBone->transform = Matrix::Identity;

	children.push_back(dummyBone);

	return dummyBone;
}

void ModelMesh::AttachToDummyBone(shared_ptr<ModelBone> dummyBone)
{
	bone = dummyBone; // ���� ���� ���� ����
	boneIndex = dummyBone->index;

	int parentBoneIndex = bone->parent ? bone->parentIndex : bone->index;

	// �ش� �� �ε����� ����ġ�� ��� ������ ����
	for (auto& vertex : geometry->GetVertices())
	{
		// ���� ���� �θ� ���� ����
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

