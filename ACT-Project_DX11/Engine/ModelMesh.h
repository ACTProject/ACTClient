#pragma once

struct ModelBone : public std::enable_shared_from_this<ModelBone>
{
	bool isDummy = false;

	shared_ptr<ModelBone> AddDummyBone(const wstring& dummyName, Matrix dummyTransform, int32 dummyIndex);
    
	wstring name;
	int32 index;
	int32 parentIndex;
	shared_ptr<ModelBone> parent; // Cache

	Matrix transform;
	vector<shared_ptr<ModelBone>> children; // Cache
};

struct ModelMesh
{
	// 더미 본 연결 함수
	void AttachToDummyBone(shared_ptr<ModelBone> dummyBone);

	void CreateBuffers();

	wstring name;

	// Mesh
	shared_ptr<Geometry<ModelVertexType>> geometry = make_shared<Geometry<ModelVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;

	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache

	// Bones
	int32 boneIndex;
	shared_ptr<ModelBone> bone; // Cache;
};

