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

	void SetModel(shared_ptr<Model> model);
	void SetTechnique(uint8 technique) { _technique = technique; }
	void SetPass(uint8 pass) { _pass = pass; }

	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	void RenderSingle();
	InstanceID GetInstanceID();

private:
	shared_ptr<Shader>	_shader;
	uint8				_technique = 0;
	uint8				_pass = 0;
	shared_ptr<Model>	_model;
};

