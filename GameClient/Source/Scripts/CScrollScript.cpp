#include "pch.h"

#include "TimeMgr.h"
#include "GameObject.h"

#include "GamePlayMgr.h"
#include "CScrollScript.h"

CScrollScript::CScrollScript()
	: CScript(SCRIPT_TYPE::SCROLLSCRIPT)
	, m_fSpeed(200.f)
{
}

CScrollScript::~CScrollScript()
{
}

void CScrollScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x -= m_fSpeed * DT;
	Transform()->SetRelativePos(vPos);
}

void CScrollScript::SaveToLevelFile(FILE* _File)
{
}

void CScrollScript::LoadFromLevelFile(FILE* _File)
{
}

