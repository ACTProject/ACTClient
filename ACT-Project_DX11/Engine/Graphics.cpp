#include "pch.h"
#include "Graphics.h"
#include "Camera.h"

void Graphics::Init(HWND hwnd)
{
	_hwnd = hwnd;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();
    CreateShadowDepthStencilView();
	SetViewport(GAME->GetGameDesc().width, GAME->GetGameDesc().height);
}

void Graphics::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), (float*)(&GAME->GetGameDesc().clearColor));
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    SetViewport(_oldVp.GetWidth(), _oldVp.GetHeight());
	_vp.RSSetViewport();
}

void Graphics::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}
void Graphics::RenderShadowBegin()
{
    viewPortNum = 1;

    _oldVp.Set(_vp.GetWidth(), _vp.GetHeight());
    SetViewport(2048, 2048);

    _deviceContext->OMSetRenderTargets(0, nullptr, _shadowDepthStencilView.Get());


    //렌타뷰, 뎁스뷰, 뷰포트 초기화
    Clear();
}
void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GAME->GetGameDesc().width;
		desc.BufferDesc.Height = GAME->GetGameDesc().height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);

}

void Graphics::CreateDepthStencilView()
{
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = static_cast<uint32>(GAME->GetGameDesc().width);
		desc.Height = static_cast<uint32>(GAME->GetGameDesc().height);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _depthStencilTexture.GetAddressOf());
		CHECK(hr);
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = DEVICE->CreateDepthStencilView(_depthStencilTexture.Get(), &desc, _depthStencilView.GetAddressOf());
		CHECK(hr);
	}
}

void Graphics::CreateShadowDepthStencilView()
{
    // DepthStencilView 생성
    // Texture2D
    ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
    {
        D3D11_TEXTURE2D_DESC DescDepth;
        ZeroMemory(&DescDepth, sizeof(D3D11_TEXTURE2D_DESC));
        DescDepth.Width = 2048;//static_cast<uint32>(GAME->GetGameDesc().width);
        DescDepth.Height = 2048;// static_cast<uint32>(GAME->GetGameDesc().height);
        DescDepth.MipLevels = 1;
        DescDepth.ArraySize = 1;
        DescDepth.SampleDesc.Count = 1;
        DescDepth.SampleDesc.Quality = 0;
        DescDepth.CPUAccessFlags = 0;
        DescDepth.MiscFlags = 0;
        DescDepth.Format = DXGI_FORMAT_R32_TYPELESS;
        DescDepth.Usage = D3D11_USAGE_DEFAULT;

        DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

        HRESULT hr = DEVICE->CreateTexture2D(&DescDepth, NULL, _shadowTexture.GetAddressOf());
        CHECK(hr);
    }
    //DSV
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
        dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

        HRESULT hr = DEVICE->CreateDepthStencilView(_shadowTexture.Get(), &dsvDesc, _shadowDepthStencilView.GetAddressOf());
        CHECK(hr);
    }
    //SRV
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        HRESULT hr = DEVICE->CreateShaderResourceView(_shadowTexture.Get(), &srvDesc, _DsvSRV.GetAddressOf());
        CHECK(hr);
    }
}


bool Graphics::Clear()
{
    _vp.RSSetViewport();
    _deviceContext->ClearDepthStencilView(_shadowDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    return true;
}

void Graphics::SetViewport(float width, float height, float x /*= 0*/, float y /*= 0*/, float minDepth /*= 0*/, float maxDepth /*= 1*/)
{
	_vp.Set(width, height, x, y, minDepth, maxDepth);
}

void Graphics::OnResize(float width, float height)
{
    UIMANAGER->ResizeButton(width,height);
    SAVE->ResizeUI(width,height);
    // 렌더 타겟과 깊이 스텐실 뷰 해제
    if (_renderTargetView) _renderTargetView.Reset();
    if (_depthStencilView) _depthStencilView.Reset();
    if (_depthStencilTexture) _depthStencilTexture.Reset();

    // 섀도우 렌타뷰 깊이 스텐실 해제
    //if (_shadowRenderTargetView) _shadowRenderTargetView.Reset();
    if (_shadowDepthStencilView) _shadowDepthStencilView.Reset();
    if (_shadowTexture) _shadowTexture.Reset();
    //if (_SRV) _SRV.Reset();

    GameDesc desc = GAME->GetGameDesc();
    desc.width = width;
    desc.height = height;
    GAME->SetGameDesc(desc);

    // 스왑 체인 크기 조정
    HRESULT hr = _swapChain->ResizeBuffers(0, static_cast<UINT>(width), static_cast<UINT>(height), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to resize swap chain buffers.", L"Error", MB_OK);
        return;
    }

    // 렌더 타겟 뷰 다시 생성
    CreateRenderTargetView();

    // 깊이 스텐실 뷰 다시 생성
    CreateDepthStencilView();

    // 섀도우 렌타뷰 깊이스탠실 다시 생성
    //CreateShadowDepthStencilView();

    // 뷰포트 업데이트
    SetViewport(width, height);
    _oldVp.Set(_vp.GetWidth(), _vp.GetHeight());

    if (CUR_SCENE->GetMainCamera())
    {
        auto camera = CUR_SCENE->GetMainCamera()->GetCamera();
        camera->SetWidth(static_cast<float>(width));
        camera->SetHeight(static_cast<float>(height));
        camera->UpdateMatrix(); // 투영 행렬 및 뷰 행렬 갱신
    }
}