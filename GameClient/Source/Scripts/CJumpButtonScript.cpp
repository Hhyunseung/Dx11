#include "pch.h"
#include "CJumpButtonScript.h"

#include "GamePlayMgr.h"

CJumpButtonScript::CJumpButtonScript()
{
}

CJumpButtonScript::~CJumpButtonScript()
{
}


void CJumpButtonScript::Tick()
{
}

void CJumpButtonScript::OnButtonClick()
{
	GameObject* pPlayer = GamePlayMgr::GetInst()->GetPlayerObject();
	if (pPlayer == nullptr)
		return;


}

