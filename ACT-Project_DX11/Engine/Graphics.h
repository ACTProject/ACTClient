#pragma once
#include "Viewport.h"

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void Init(HWND hwnd);

	void RenderBegin();
	void RenderEnd();

    // 그림자맵 텍스처를 그리기 위한 렌더타겟변경함수
    void RenderShadowBegin();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }
    ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }

    ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return _renderTargetView; }
    ComPtr<ID3D11Texture2D> GetDepthStencilTexture() { return _depthStencilTexture; }
    ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return _depthStencilView; }


    // 섀도우 함수.

    ComPtr<ID3D11DepthStencilView> GetShadowDSV() { return _shadowDepthStencilView; }
    ComPtr<ID3D11ShaderResourceView> GetShadowSRV() { return _DsvSRV; }


private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
    void CreateShadowDepthStencilView();

    // 그림자맵텍스처를 위한 렌타뷰 클리어 함수.
    bool Clear();

public:
	void SetViewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	Viewport& GetViewport() { return _vp; }

public:
    void OnResize(float width, float height);

private:
	HWND _hwnd = {};

	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
    

	// DSV
	ComPtr<ID3D11Texture2D> _depthStencilTexture;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;

    // Misc
    //D3D11_VIEWPORT _viewport = { 0 };
    Viewport _vp;



    //// 그림자맵텍스처 그리기 위한 변수들
    //ComPtr<ID3D11RenderTargetView> _shadowRenderTargetView;
    //ComPtr<ID3D11ShaderResourceView> _SRV; //
    //ComPtr<ID3D11Texture2D> _shadowColorTexture;

    ComPtr<ID3D11DepthStencilView> _shadowDepthStencilView;
    ComPtr<ID3D11ShaderResourceView> _DsvSRV; 
    ComPtr<ID3D11Texture2D>         _shadowTexture;


    // 렌타뷰 저장 변수들
	ComPtr<ID3D11RenderTargetView> _oldRenderTargetView;
	ComPtr<ID3D11DepthStencilView> _oldDepthStencilView;

    UINT viewPortNum;
    Viewport _oldVp;
};

