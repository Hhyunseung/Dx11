#include "pch.h"
#include "CSceneChangeButtonScript.h"

#include "SceneFlowMgr.h"

CSceneChangeButtonScript::CSceneChangeButtonScript()
	: CUIButtonScript(SCRIPT_TYPE::SCENECHANGEBUTTONSCRIPT)
	, m_Action(SCENE_FLOW_ACTION::GO_LOBBY)
{
}

CSceneChangeButtonScript::~CSceneChangeButtonScript()
{
}


void CSceneChangeButtonScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_Action, L"Action", true, 0.f);
}

void CSceneChangeButtonScript::OnButtonClick()
{
	SceneFlowMgr::GetInst()->ExecuteAction(m_Action);
}

void CSceneChangeButtonScript::SaveToLevelFile(FILE* _File)
{
	int action = (int)m_Action;
	fwrite(&action, sizeof(int), 1, _File);
}

void CSceneChangeButtonScript::LoadFromLevelFile(FILE* _File)
{
	int action = 0;
	fread(&action, sizeof(int), 1, _File);
	m_Action = (SCENE_FLOW_ACTION)action;
}
