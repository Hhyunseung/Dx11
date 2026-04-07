#include "pch.h"
#include "Engine.h"

#include "Device.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "EditorMgr.h"
#include "TaskMgr.h"
#include "FontMgr.h"

Engine::Engine()
	: m_hInst(nullptr)
	, m_hWnd(nullptr)
	, m_Resolution{}
	, m_EditorMode(true)
{

}

Engine::~Engine()
{
}

int Engine::Progress()
{
	// DeltaTime 계산
	TimeMgr::GetInst()->Tick();

	// Key 상태 계산
	KeyMgr::GetInst()->Tick();

	// LevelMgr 업데이트
	LevelMgr::GetInst()->Progress();

	// RenderMgr 렌더링
	RenderMgr::GetInst()->Progress();

	// FPS Render
	TimeMgr::GetInst()->Render();
	
	FontMgr::GetInst()->Render();


	if (m_EditorMode)
	{
		// EditorMgr 
		/// Render 끝나고(RenderMgr) UI 가 추가로 그려진 것(EditorMgr)을 화면에 보여줌(Device)
		/// (RenderMgr=>EditerMgr=>Device)
		EditorMgr::GetInst()->Progress();
	}

	// 렌더타겟에 그려진 그림을, 윈도우 비트맵으로 복사
	Device::GetInst()->Present();

	// 다음 프레임에 적용될 작업 처리 /// 가장 마지막에 호출
	// 여기서 처리한 작업은 다음 프레임에 적용된다
	TaskMgr::GetInst()->Progress();

	return S_OK;
}
