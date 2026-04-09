#include "pch.h"
#include "CRestartButtonScript.h"

#include "SceneFlowMgr.h"
#include "GamePlayMgr.h"

CRestartButtonScript::CRestartButtonScript()
	:CUIButtonScript(SCRIPT_TYPE::RESTARTBUTTONSCRIPT)
{
}

CRestartButtonScript::~CRestartButtonScript()
{
}

void CRestartButtonScript::OnButtonClick()
{
	GamePlayMgr::GetInst()->ResumeGame();
	SceneFlowMgr::GetInst()->ExecuteAction(SCENE_FLOW_ACTION::RESTART_GAME);
}
