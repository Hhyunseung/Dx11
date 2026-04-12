#include "pch.h"
#include "Device.h"

Device::Device()
	:m_hWnd(nullptr)
{
}

Device::~Device()
{
}


int Device::Init(HWND _hwnd, Vec2 _Resolution)
{
    m_hWnd = _hwnd;
    m_RenderResol = _Resolution;
    g_Global.Resolution = m_RenderResol;

    // Dx11 라이브러리는 동적 라이브러리 이고
    // Dx11 관련 객체 생성함수를 통해서 생성된 객체의 주소를 받은 경우,
	// 메모리 해제도 Dx11 라이브러리에서 제공하는 함수로 해제해야 한다.

    /// 만약 DLL 에서 new 로 할당한 메모리를 실행파일에서 delete 하려고하면 CRT 불일치 가능성이 있어서
    /// 힙 손상이나 프로그램 충돌이 날 수 있다


    // 스마트 포인터
    // 포인터를 흉내내는 클래스


#ifdef _DEBUG
	UINT iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL* pLevel = nullptr;

    // D3D11CreateDevice 는 Dx11 라이브러리에서 제공하는 함수로
    // ID3D11Device 객체와 ID3D11DeviceContext 객체를 생성해서 주소를 알려주는 함수
    // ID3D11Device, ID3D11DeviceContext 객체의 레퍼런스 카운트를 이미 1을 주고 시작함
    // 따라서 스마트 포인터가 해당 객체를 가리키게 되면, 레퍼런스 카운트가 2가 되는 문제가 생김
    // 스마트 포인터가 대상을 가리키는 멤버 포인트의 주소(이중 포인터)를 받아와서 
    // 가리킨 주소값을 강제로 바로 세팅해주려고 멤버의 주소(이중포인터)를 요구함
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, iFlag, 0, 0,
        D3D11_SDK_VERSION, m_Device.GetAddressOf(), pLevel, m_Context.GetAddressOf());

    if (FAILED(hr))
    {
        return E_FAIL;
    }

    // SwapChain
    // 윈도우에서 이미지를 표시하기 위해, 비트맵 데이터를 SystemMemory 영역에 가지고 있다
    // 이 비트맵에 그림을 그려야 윈도우 화면 영역에 표시가 된다
    if (FAILED(CreateSwapChain()))
    {
        return E_FAIL;
    }

    // GPU 는 이 SystemMemory 영역에 직접적으로 접근이 불가능하다
    // 따라서 GPU 로 렌더링 작업을 하기 위해서 VRAM 에 RenderTarget(픽셀 데이터 집합체 버퍼) 생성하고
    // 여기에 그림을 그린 다음 SwapChain 을 이용해서 SystemMemory 에 있는 비트맵으로 픽셀 데이터를 복사받아야 한다

    /*
    1600, 900 으로 채워진 픽셀들을 비트맵이라고함
    비트맵은 RAM 에 존재 (시스템 영역) -> CPU 가 접근 가능 .. GPU 가 접근 불가능
	따라서 GPU 메모리 영역에 똑같은 해상도의 비트맵을 하나 더 만들어야함 -> VRAM 영역. (Texture2D 라고함) RenderTarget
	이 GPU 가 그린 버퍼 이미지를 CPU 가 볼 수 있도록 다시 RAM 영역으로 복사해주는 역할을 하는 것이 SwapChain
    */

    // RenderTarget
    // 그림을 그릴 목적이 타겟에 해당하는 RenderTarget 은 SwapChain 생성 시 같이 만들어 진다

    // DepthStencilTarget
    // 렌더타겟과 동일한 해상도여야 함
    // RenderTarget이 물체의 그림이 그려진다면 DepthStencilTarget 은 카메라와 물체의 거리값이 픽셀에 기록됨
    // 더 가까운 깊이값을 가지는 물체가 픽셀단위로 렌더타겟에 덮어 씌임. 그리고 자신의 깊이를 깊이타겟에 덮어 씀
    // 만약 먼저 기록된 물체보다 현재 물체의 깊이값이 더 멀면, RenderTarget에 그려지지 않고, 깊이타겟도 갱신하지 않음


    /*
    GPU 메모리 영역 VRAM 안에는 RenderTarget 이 있지만 또다른 Texture2D 가 있는데 이것을 DepthStencilTarget 이라고 한다
	반드시 RenderTarget 과 똑같은 해상도로 만들어져야 한다
    픽셀단위로 카메라로부터의 거리 정보(값)를 저장 .. 깊이값
    */

	// View - 리소스(RenderTarget, DepthStencilTarget...)의 전달자, 매니징 역할 
	//        연결된 리소스의 무결성을 보증, 실제 리소스가 필요한 곳에다가 연결된 View 를 전달해서 리소스를 연결해줌
    /// 직접적인 리소스를 주는게 아니라 뷰를 통해서 리소스를 전달
	/// 무결성 보증 (리소스가 잘못 되면 View 가 생성되지 않음) 
    // RenderTargetView 
    // DepthStencilView
    // ShaderResourceView 
    // UnordredAccessView
    if (FAILED(CreateBuffer()))
    {
        return E_FAIL;
    }

    D3D11_VIEWPORT ViewPort = {};
    ViewPort.TopLeftX   = 0;
    ViewPort.TopLeftY   = 0;
    ViewPort.Width      = m_RenderResol.x;
    ViewPort.Height     = m_RenderResol.y;
    ViewPort.MinDepth   = 0.f;
    ViewPort.MaxDepth   = 1.f;
    
    // RenderTarget 을 목적지에 출력시킬 영역 설정
    m_Context->RSSetViewports(1, &ViewPort);

    // 렌더타겟, 깊이 타겟 출력 설정
    // 렌더링 파이프라인 과정에서 마지막에 그림을 출력시킬 목적지 설정
    m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());

    //// 렌더타겟의 모든 픽셀을 특정 색상으로 칠한다
	//// Dx 에서는 색상 데이터를 0 ~ 1 범위로 정규화 (Normalize) 해서 사용한다 ( 0 ~ 255 -> 0.f ~ 1.f )
	//float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
    //m_Context->ClearRenderTargetView(m_RTV.Get(), clearColor);



    if (FAILED(CreateSampler()))
        return E_FAIL;


    if (FAILED(CreateRasterizerState()))
        return E_FAIL;

    if (FAILED(CreateDepthStencilState()))
        return E_FAIL;

    // 앞으로 사용할 BlendState 제작
    if (FAILED(CreateBlendState()))
        return E_FAIL;

    // 상수 버퍼 생성
    CreateConstBuffer();

    return S_OK;
}

void Device::ClearTarget()
{
    Vec4 vColor = Vec4(0.f, 0.f, 0.f, 0.f);
    CONTEXT->ClearRenderTargetView(m_RTV.Get(), vColor);

    /// 깊이의 최대 깊이는 1.f
	/// 초기화를 1.f 로 해야 모든 픽셀이 가장 멀리 있는 상태로 초기화
    CONTEXT->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void Device::ClearDepth()
{
    CONTEXT->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void Device::OMSetTarget()
{
    // 렌더타겟, 깊이 타겟 출력 설정
    // 렌더링 파이프라인 과정에서 마지막에 그림을 출력시킬 목적지 설정
    m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());
}

int Device::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC m_Desc = {};

    // RenderTarget 의 개수
    m_Desc.BufferCount = 1;

    // 버퍼(렌더타겟) 해상도 == 윈도우 비트맵 해상도
    m_Desc.BufferDesc.Width = (UINT)m_RenderResol.x; 
    m_Desc.BufferDesc.Height = (UINT)m_RenderResol.y;

    // 버퍼의 사용 용도 == 렌더타겟
    m_Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // 픽셀의 종류(타입)
    m_Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    
    // 스왑체인이 렌더타겟을 출력시킬 목적지 윈도우(비트맵) /// 출력 시킬 윈도우
    m_Desc.OutputWindow = m_hWnd;

    // 창모드 설정 (true : 창 모드 , false : 전체화면 모드)
    m_Desc.Windowed = true;

    // 렌더타겟의 이미지를 출력후 버려도 된다
    m_Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
   
    m_Desc.BufferDesc.RefreshRate.Numerator = 60;
    m_Desc.BufferDesc.RefreshRate.Denominator = 1;
    m_Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    m_Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    m_Desc.SampleDesc.Count = 1;
    m_Desc.SampleDesc.Quality = 0;
    m_Desc.Flags = 0;


    // SwapChian 생성하기
    // IDXGIFactory 가 필요
    ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
    ComPtr<IDXGIAdapter> pAdapter = nullptr;
    ComPtr<IDXGIFactory> pFactory = nullptr;

    m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
    pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &m_Desc, m_SwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int Device::CreateBuffer()
{
    // RenderTarget
    // SwapChain 생성할 때 이미 같이 만들어져있다
		/// SwapChain 내부의 RenderTarget 을 가져와서 m_RenderTarget 에 세팅
        /// GetBuffer 함수에서 m_RenderTarget 을 가리키기 때문에 m_RenderTarget 의 RefCount 증가
        /// 또 RefCount 가 증가하는 것을 막기 위해 GetAddressOf() 로 직접적으로 주소 전달
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RenderTarget.GetAddressOf());

    // DepthStencilTarget 제작하기
    D3D11_TEXTURE2D_DESC Desc = {};

    // 텍스쳐 1장
    Desc.ArraySize          = 1; 
   
    // 해상도 - 깊이 타겟도 렌더 타겟과 해상도가 동일해야한다
    Desc.Width              = m_RenderResol.x;
    Desc.Height             = m_RenderResol.y;
	Desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT; // 3바이트는 깊이를 저장, 1바이트는 스탠실 값을 저장
    
	// CPU 메모리 접근 옵션 - CPU 접근 불가 설정
    Desc.CPUAccessFlags     = 0;
    Desc.Usage              = D3D11_USAGE_DEFAULT;
    
    // 용도
    Desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;

    Desc.MipLevels          = 1; // 밉맵. 저화질 버전 텍스쳐 추가 생성 // 1 == 원본 /// 원본만 생성
    Desc.MiscFlags          = 0;
    Desc.SampleDesc.Count   = 1;
    Desc.SampleDesc.Quality = 0;


    if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr/*초기 전달할 데이터*/
                                         , m_DepthStencilTarget.GetAddressOf())))
    {
        return E_FAIL;
    }


    // RenderTargetView
	/// 무언가를 만들때는 m_Device 객체의 CreateXXX() 함수를 호출해서 생성
	/// m_RenderTarget 은 ID3D11Texture2D 를 가리키는 스마트 포인터니까 해당 포인터가 가리키는 ID3D11Texture2D
    if (FAILED(m_Device->CreateRenderTargetView(m_RenderTarget.Get(), nullptr, m_RTV.GetAddressOf())))
    {
        return E_FAIL;
    }

    // DepthStencilView
    if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilTarget.Get(), nullptr, m_DSV.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int Device::CreateSampler()
{
	//===============================
    // 이방성 필터링 + 랩 모드
	//===============================
    /// 이방성 필터링 : 3차원 공간에서 이미지 왜곡을 줄여줌
    D3D11_SAMPLER_DESC Desc = {};

    Desc.Filter     = D3D11_FILTER_ANISOTROPIC;

    // Address Mode 
    //Desc.AddressU   = D3D11_TEXTURE_ADDRESS_CLAMP; // UV 초과하면 막아버림
    //Desc.AddressU   = D3D11_TEXTURE_ADDRESS_MIRROR; // 
    Desc.AddressU   = D3D11_TEXTURE_ADDRESS_WRAP; /// 보통은 이 모드
    Desc.AddressV   = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW   = D3D11_TEXTURE_ADDRESS_WRAP;

    DEVICE->CreateSamplerState(&Desc, m_arrSampler[0].GetAddressOf());

    // s0 레지스터에 바인딩 (전부 세팅하여 모든 파이프라인 단계에서 사용 가능)
    CONTEXT->VSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
    
    //===============================
    // POINT 필터링 + 랩 모드
	//===============================
	/// 포인트 필터링 : 가장 가까운 픽셀의 색상을 그대로 가져옴
    Desc = {};

	// 2D 텍스쳐 샘플링 필터링 방식
    /// 리소스 자체는 작게 만들고 화면에 확장시켜서 그린다 
	/// 이방성은 보간을 해서 뿌옇게 보임
    /// 
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

    // Address Mode 
    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    DEVICE->CreateSamplerState(&Desc, m_arrSampler[1].GetAddressOf());

    // s1 레지스터에 바인딩 (전부 세팅하여 모든 파이프라인 단계에서 사용 가능)
    CONTEXT->VSSetSamplers(1, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_arrSampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_arrSampler[0].GetAddressOf());

    return 0;
}

int Device::CreateRasterizerState()
{
    // CULL_BACK
    m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    // CULL_FRONT
    D3D11_RASTERIZER_DESC Desc = {};
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

    // CULL_NONE
    Desc = {};
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    // WIRE_FRAME
    Desc = {};
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());


    return 0;
}

int Device::CreateDepthStencilState()
{
    // Less
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

    // LessEqual
    D3D11_DEPTH_STENCIL_DESC Desc = {};

    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;       // 작거나 같으면 통과
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 자신의 깊이로 덮어씀
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUL].GetAddressOf())))
        return E_FAIL;

    // LESS_NO_WRITE: 깊이 테스트 O (LESS), 깊이 쓰기 X → 투명/이펙트 오브젝트용
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_NO_WRITE].GetAddressOf())))
        return E_FAIL;

    // NO_TEST
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // 깊이판정은 항상 통과
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 자신의 깊이로 덮어씀
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST].GetAddressOf())))
        return E_FAIL;

    // NO_TEST_NO_WRITE
    Desc.DepthEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
        return E_FAIL;

    return S_OK;
}

int Device::CreateBlendState()
{
    // 1. Default (ScrRGB * 1) + (DestRGB * 0)
    // Context 에 nullptr 를 전달하면, 기본 블렌드 스테이트를 사용한다는 뜻
    /// 이전 옵션이 끝나면 nullptr 로 초기화하여 기본상태로 돌린다
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr; 
    
    
    // 2. AlphaBlend - (ScrRGB * SrcAlpha) + (DestRGB * (1 - SrcAlpha))
    D3D11_BLEND_DESC Desc = { };

    Desc.AlphaToCoverageEnable = false; /// 투명한데 깊이가 남아있어서 불투명해지는현상막아주는거
    Desc.IndependentBlendEnable = false; /// 렌더타겟이 여러개일때 ..~ 

    // 배열 인덱스... n번 타겟을 블렌딩할때
    Desc.RenderTarget[0].BlendEnable = true; 
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 블랜딩 결과를 타겟에 출력
    
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; /// 더하기 할거고
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;   // 본인의 알파가 자기 자신에 곱해지는 계수가 된다
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;     // 목적지는 출력할려는 Alpha 의 인버스한값을 곱할것


    /// 우리는 렌더타겟에 들어갈 RGB 값을 구하고 있는중
    /// 렌더타겟의 A 는 무엇이 들어가게 할것이냐?
    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; 
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());

    // 3. One_One - (ScrRGB * 1) + (DestRGB * 1)
    Desc = { };

    Desc.AlphaToCoverageEnable = false; /// 투명한데 깊이가 남아있어서 불투명해지는현상막아주는거
    Desc.IndependentBlendEnable = false; /// 렌더타겟이 여러개일때 ..~ 

    // 배열 인덱스... n번 타겟을 블렌딩할때
    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 블랜딩 결과를 타겟에 출력

    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; /// 더하기 할거고
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;   // 본인의 알파가 자기 자신에 곱해지는 계수가 된다
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;     // 목적지는 출력할려는 Alpha 의 인버스한값을 곱할것

    /// 우리는 렌더타겟에 들어갈 RGB 값을 구하고 있는중
    /// 렌더타겟의 A 는 무엇이 들어가게 할것이냐?
    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());


    return S_OK;
}

void Device::CreateConstBuffer()
{
    m_CB[(UINT)CB_TYPE::TRANSFORM] = new ConstBuffer;
    m_CB[(UINT)CB_TYPE::TRANSFORM]->Create(CB_TYPE::TRANSFORM, sizeof(TransformMatrix));

    m_CB[(UINT)CB_TYPE::MATERIAL] = new ConstBuffer;
    m_CB[(UINT)CB_TYPE::MATERIAL]->Create(CB_TYPE::MATERIAL, sizeof(MtrlConst));

    m_CB[(UINT)CB_TYPE::GLOBAL] = new ConstBuffer;
    m_CB[(UINT)CB_TYPE::GLOBAL]->Create(CB_TYPE::GLOBAL, sizeof(GlobalData));
}
