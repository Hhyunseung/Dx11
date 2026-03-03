// GameClient.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"

#include "Engine.h"
#include "EditorMgr.h"

#include <crtdbg.h>


// 변수 템플릿
template<typename T>
bool g_IsTrue = true;

// 템플릿 특수화
// T1, T2 가 다르게 설정되면 초기값을 100으로
// T1, T2 타입이 같은 타입으로 설정되면 초기값을 200으로 설정한 전역변수를 만든다
template<typename T1, typename T2>
int NewData = 100;
template<typename T1> /// 특정상황일때에만 예외 조건 /// 똑같은 자료형이 들어오면 200으로 초기화 하겠다
int NewData<T1, T1> = 200;

// if constexpr () 
/// 컴파일 타임에 해당 조건이 참인지 알 수 있어야 함


// const 보다 더 엄격한 상수 키워드
// constexpr
void Test()
{
    int k = 200;

    /// 초기화 때 값이 결정되는 상수
    // i 는 상수화된 변수, 딱 한번만 초기화 가능
    const int i = k;

    // j 는 더 엄격한 상수, 컴파일 타임에 어떤 숫자가 들어갈 것인지 확신할 수 있어야 함
    // j 의 초기값으로 입력되는 k 는 변수기 때문에 
    // 컴파일 시점에는 어떤 숫자로 j 가 초기화 되는지 알 수 없음
	// constexpr int i = k; // 에러 /// 컴파일 과정에서 값이 결정되어야함


    // 자료형 : bool
    // 변수명 : g_IsTure<int>
    g_IsTrue<int> = 0;

    // 자료형 : bool
    // 변수명 : g_IsTure<float>
    g_IsTrue<float> = 0;

    // 자료형 : bool
    // 변수명 : NewData<int, float>
    int a = NewData<int, float>; // 100

	int b = NewData<int, int>; // 200

    // if 구문 옆에 constexpr 를 붙이면
    // if 의 조건안에 반드시 컴파일 타임에 확실할 수 있는 참 or 거짓이 들어와야만 한다
    // 컴파일 시간안에 확실할 수 있다면, 해당 조건구문만 남기고 다른 코드는 전부 삭제(컴파일 최적화) 가능
    if constexpr (0 == 0)
    {

    }
    else
    {

    }
}

HINSTANCE hInst;    

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // 자기 자신의 ID handle (프로세스)
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    //Test();


    // CRT new, delete, 디버깅 모드에서 메모리 누수 추적. 출력창에 알림
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    // 누수 발생지점 중단점 걸어주는 기능
    // _CrtSetBreakAlloc(220); 

    hInst = hInstance;



    // 복사생성자는 컴파일러가 기본적으로 만들어지는데 이는 public 필드에 선언된다
    // singleton 패턴에서는 복사생성자를 delete 해서 막아버리는게 좋다
    // 소멸자만 추가하더라도 복사생성자가 막혔던 이유는... 
	//  소멸자를 private 로 막아버리면 함수가 끝나서 지역변수가 사라지면서 소멸자가 호출될때 private 접근 불가로 에러가남
    // Engine e 라는 지역변수 자체가 만들어지지 않도록 막는것.. 
    // Engine e(*Engine::GetInst());

    // Engine 초기화
    // 최상위 관리자
    if (FAILED(Engine::GetInst()->Init(hInstance, 1600, 900, true)))
        return 0;

    // 프로세스는 여러개의 윈도우를 가질 수도, 하나도 가지지 않을 수도 있다
    // 게임 화면을 출력시킬 윈도우가 반드시 필요한 프로그램
    // 따라서 윈도우가 생성되지 않거나, 윈도우를 종료하면 곧 프로그램 종료로 싱크를 맞춰둔 상태


    CreateTestLevel();


    // 메시지 루프
    // 단축키 테이블 정보 핸들 받음
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMECLIENT));
    
    // 윈도우에 발생한 사건(이벤트, 메시지) 정보를 받을 구조체
    MSG msg = {}; // 어떤 윈도우에서 어떤 사건이 발생했는지에 대한 정보

    while (true)
    {
        // 메시지 큐에서 메시지를 꺼낸게 있다
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            // 단축키 관련된 내용이면 TranslateAccelerator 함수에서 처리
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                // 단축키 관련 이벤트가 아니면 TranslateMessage, DispatchMessage 함수로 처리
                TranslateMessage(&msg);
                DispatchMessage(&msg); // 클릭된 윈도우의 WndProc 함수 호출
            }
        }

        // 메시지 큐에서 메시지를 꺼낸게 없다
        else
        {
            // Game 실행, 1 프레임
            if (FAILED(Engine::GetInst()->Progress()))
                break;
        }
    }

    return (int) msg.wParam;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.

#include "KeyMgr.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 프로시저 함수
// 윈도우에 발생한 사건(이벤트, 메시지)들을 처리해주는 함수
// DispatchMessage 함수에 의해서 호출됨
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    int Data = 0;

    switch (message)
    {
    //case WM_LBUTTONDBLCLK: // 더블클릭시
    //    // 윈도우 종료함수
    //    PostQuitMessage(0);        // 메시지 큐에 WM_QUIT 가 들어감
    case WM_MOUSEWHEEL:
        // 마우스 휠 처리
        {
            // 휠 올리면 120 내리면 -120
            // Data = GET_WHEEL_DELTA_WPARAM(wParam);

        KeyMgr::GetInst()->SetMouseWheel(Data = GET_WHEEL_DELTA_WPARAM(wParam));
        }   
        break;
    //  break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
		return DefWindowProc(hWnd, message, wParam, lParam); 
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
