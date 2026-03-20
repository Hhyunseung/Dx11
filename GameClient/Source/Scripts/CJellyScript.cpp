#include "pch.h"
#include "CJellyScript.h"

#include "GameObject.h"

CJellyScript::CJellyScript()
	: CScript(SCRIPT_TYPE::JELLYSCRIPT)
	, m_Score(1000)
	, m_Magnet(true)
	, m_MagnetRange(300.f)
	, m_MagnetSpeed(200.f)
{

}

CJellyScript::~CJellyScript()
{
}


void CJellyScript::Begin()
{
	ADD_DYNAMIC_BEGIN_OVERLAP(CJellyScript::BeginOverlap);
}

void CJellyScript::Tick()
{
}

void CJellyScript::BeginOverlap(CCollider2D* _This, CCollider2D* _Other)
{

}


void CJellyScript::SaveToLevelFile(FILE* _File)
{
}

void CJellyScript::LoadFromLevelFile(FILE* _File)
{
}
