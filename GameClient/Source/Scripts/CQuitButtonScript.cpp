#include "pch.h"
#include "CQuitButtonScript.h"

#include "SceneFlowMgr.h"
#include "GamePlayMgr.h"

CQuitButtonScript::CQuitButtonScript()
	:CUIButtonScript(SCRIPT_TYPE::QUITBUTTONSCRIPT)
{
}

CQuitButtonScript::~CQuitButtonScript()
{
}

void CQuitButtonScript::OnButtonClick()
{
	GamePlayMgr::GetInst()->ResumeGame(); // 게임이 일시정지 상태라면 해제
	SceneFlowMgr::GetInst()->ExecuteAction(SCENE_FLOW_ACTION::GO_LOBBY);	
}

