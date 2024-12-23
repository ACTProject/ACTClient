#pragma once
#include "Component.h"

class Mesh;
class Shader;
class Material;

#define MAX_MESH_INSTANCE 500

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer();
	virtual ~MeshRenderer();
    
    shared_ptr<Shader> GetShader() { return _shader; }
    const shared_ptr<Material> GetMaterial() { return _material; }
    uint8 GetTech() { return _technique; }
    uint8 GetPass() { return _pass; }

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetTechnique(uint8 technique) { _technique = technique; }
	void SetPass(uint8 pass) { _pass = pass; }
	void SetAlphaBlend(bool isAlphaBlend) { _isAlphaBlend = isAlphaBlend; }
    void SetShader(shared_ptr<Shader> shader) { _shader = shader; }

	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	void RenderSingle();
    void RenderShadowMap(Matrix view, Matrix proj);

	InstanceID GetInstanceID();

private:
	shared_ptr<Shader>	_shader;
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
	uint8 _technique = 0;
	uint8 _pass = 0;
	bool _isAlphaBlend = false;
};
