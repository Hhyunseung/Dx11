#include "pch.h"
#include "CResumeButtonScript.h"

#include "GamePlayMgr.h"

CResumeButtonScript::CResumeButtonScript()
	:CUIButtonScript(SCRIPT_TYPE::RESUMEBUTTONSCRIPT)
{
}

CResumeButtonScript::~CResumeButtonScript()
{
}


void CResumeButtonScript::OnButtonClick()
{
	GamePlayMgr::GetInst()->ResumeGame();
}
