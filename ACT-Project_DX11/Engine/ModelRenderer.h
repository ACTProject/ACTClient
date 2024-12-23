#pragma once
#include "Component.h"

class Model;
class Shader;
class Material;

class ModelRenderer : public Component
{
	using Super = Component;

public:
	ModelRenderer(shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	shared_ptr<Model> GetModel() { return _model; };
	shared_ptr<Shader> GetShader() { return _shader; };
    uint8 GetPass() { return _pass; };
    uint8 GetTechnique() { return _technique; };

	void SetModel(shared_ptr<Model> model);
	void SetTechnique(uint8 technique) { _technique = technique; }
	void SetPass(uint8 pass) { _pass = pass; }
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }

	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	void RenderSingle();
    // 섀도우맵 그리는 함수.
    void RenderShadowMap(Matrix view, Matrix proj);
	InstanceID GetInstanceID();

private:
	shared_ptr<Shader>	_shader;
	uint8				_technique = 0;
	uint8				_pass = 0;
	shared_ptr<Model>	_model;
};

