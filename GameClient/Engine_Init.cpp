#include "pch.h"
#include "Engine.h"

#include "Device.h"
#include "PathMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "AssetMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "FontMgr.h"
#include "EditorMgr.h"
#include "GameDataMgr.h"
#include "SceneFlowMgr.h"
#include "GamePlayMgr.h"

LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

int Engine::Init(HINSTANCE _hInst, UINT _Width, UINT _Height, bool _EditorMode)
{
    m_hInst = _hInst;
    m_Resolution = Vec2(_Width, _Height);
	m_EditorMode = _EditorMode;

    // 생성시킬 윈도우(창) 옵션 설정
    WNDCLASSEXW wcex = {};

    wcex.lpszClassName = L"MyGame"; // 키값
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; // 함수 포인터 등록
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_GAMECLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr; //MAKEINTRESOURCEW(IDC_GAMECLIENT);  // 윈도우 메뉴창 설정 /// 리소스 뷰 > Menu > IDC_GAMECLIENT 에서 디자인
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // 운영체제에 등록
    RegisterClassExW(&wcex);

    // 등록한 윈도우 설정 데이터를 기반으로 윈도우(창) 하나 생성
    // 커널 오브젝트  - OS 가 관리하는 객체 
    // 핸들          - 커널 오브젝트의 ID 개념 
    /// 커널 오브젝트 생성 후 핸들(아이디) 반환
    /// 부여받은 ID 가 우리가 만든 윈도우의 핸들값.. 핸들은 커널 오브젝트들을 구별하는 역할 같은 것

    // WS_POPUP : 타이틀 창이 없는 POPUP 모드로 변경
    // HWND hWnd = CreateWindowW(L"MyGame", L"Title Bar", WS_POPUP, 
    //     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    /// 윈도우 창 크기 고정
    UINT Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX;
    m_hWnd = CreateWindowW(L"MyGame", L"Title Bar", Style,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);


    // 윈도우 생성 실패시 
    if (!m_hWnd)
    {
        //return FALSE; // 프로그램 종료
        return E_FAIL;
    }

    /*
    window.h 파일 참고
    핸들을 주면 해당 커널 오브젝트를 조작해주는 window 함수들이 존재한다
    */

    // 생성한 윈도우를 화면에 보이도록 설정
    ShowWindow(m_hWnd, true);
    UpdateWindow(m_hWnd);    

    // 게임을 표시할 영역의 해상도가 원하는 크기가 되기 위해서
    // 실제 윈도우 크기는 몇이어야 하는지 계산
	RECT rt = { 0, 0, m_Resolution.x, m_Resolution.y }; /// left, top, right, bottom
	
    /// RECT 포인터 LPRECT
	/// LPRECT pRect = &rt;
	// 윈도우 스타일에 맞게 RECT 크기 조정
    ///      그래서 스타일과 메뉴 유무를 인자로 넣어줌
    // 
	// GetMenu(m_hWnd); // 메뉴가 있으면 메뉴 핸들 반환, 없으면 nullptr 반환
	// !! : 논리값 변환 연산자 (값이 있으면 true, 없으면 false)
	//    : true 와 false 개념이 아닌 값들을 true false 로 변환시켜줌
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!GetMenu(m_hWnd));


    // 윈도우 크기 및 위치 변경 (타이틀바, 테두리 포함된 전체 크기 설정)
    //SetWindowPos(m_hwnd, nullptr, 0, 0, m_resolution.x, m_resolution.y, 0);
    // 윈도우 크기 및 위치 변경 
    SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);


    // 화면에 렌더링
    // GPU 를 제어할 수 있는 Directx11 함수를 사용하기 위한 매니저
	// ID3D11Device, ID3D11DeviceContext
    if (FAILED(Device::GetInst()->Init(m_hWnd, m_Resolution)))
    {
        return E_FAIL;
    }

    // 실행파일과 같이 있는 Content 폴더의 경로를 찾아내기 위함
    PathMgr::GetInst()->Init();

    // 폰트 매니저 초기화
    FontMgr::GetInst()->Init();

    // 1프레임 동작하는데 걸리는 시간(DT) 계산
    TimeMgr::GetInst()->Init();

    // 각각의 키의 상태를 계산
    KeyMgr::GetInst()->Init();

    // 엔진 기본 에셋틀 생성
    AssetMgr::GetInst()->Init();

    // 레벨 매니저 초기화
    LevelMgr::GetInst()->Init();

    // 렌더링 매니저 초기화
    RenderMgr::GetInst()->Init();

    // FMOD 초기화	
    {
        FMOD::System_Create(&m_FMODSystem);
        assert(m_FMODSystem);

        // 32개 채널 생성
        FMOD_RESULT result = m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);
    }

    // 에디터 매니저 초기화
    if (m_EditorMode)
        EditorMgr::GetInst()->Init();

    GameDataMgr::GetInst()->Init();
    SceneFlowMgr::GetInst()->Init();
    GamePlayMgr::GetInst()->Init();


	return S_OK;
}
