#pragma once

// 게임 프로그램의 최상위 관리자
class Engine
	: public singleton<Engine>
{
	SINGLE(Engine)
//
//private:
//	Engine()
//	{}
//
//	// private 생성자라서 부모에서 호출 불가능... new T 에서 오류
//	// 따라서 friend 선언
//	friend class singleton<Engine>;

private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Vec2		m_Resolution;

	FMOD::System* m_FMODSystem;	// FMOD 관리자
	bool		m_EditorMode; /// 에디터 모드 여부

public:
	// 인스턴스 핸들 반환
	HINSTANCE GetInstance() { return m_hInst; }

	// 메인 윈도우 핸들 반환
	HWND GetMainWndHwnd() { return m_hWnd; }

	Vec2 GetResolution() { return m_Resolution; }
	FMOD::System* GetFMODSystem() { return m_FMODSystem; }

public:
	int Init(HINSTANCE _hInst, UINT _Width, UINT _Height, bool _EditorMode);
	int Progress();
};

// FMOD 관리자 매크로
#define FMOD_SYSTEM Engine::GetInst()->GetFMODSystem()