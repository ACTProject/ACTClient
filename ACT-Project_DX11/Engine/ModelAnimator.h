﻿#pragma once
#include "Component.h"

class Model;

struct AnimTransform
{
	// [ ][ ][ ][ ][ ][ ][ ] ... 250개
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;
	// [ ][ ][ ][ ][ ][ ][ ] ... 500 개
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};

class ModelAnimator : public Component
{
	using Super = Component;

public:
	ModelAnimator(shared_ptr<Shader> shader);
	~ModelAnimator();

	void SetModel(shared_ptr<Model> model);
	void SetPass(uint8 pass) { _pass = pass; }
	void SetTechnique(uint8 technique) { _technique = technique; }
	void ChangeAnimation(AnimationState newState);
	shared_ptr<Shader> GetShader() { return _shader; }

	virtual void Update() override;

	void UpdateTweenData();
	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	void RenderSingle();

	InstanceID GetInstanceID();
	TweenDesc& GetTweenDesc() { return _tweenDesc; }

private:
	void CreateTexture();
	void CreateAnimationTransform(uint32 index);

private:
	vector<AnimTransform> _animTransforms;
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11ShaderResourceView> _srv;

private:
	KeyframeDesc _keyframeDesc;
	TweenDesc _tweenDesc;

private:
	shared_ptr<Shader>	_shader;
	uint8				_technique = 0;
	uint8				_pass = 0;
	shared_ptr<Model>	_model;
};
