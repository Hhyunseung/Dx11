#include "pch.h"
#include "CScoreUIScript.h"

#include "GamePlayMgr.h"
#include "FontMgr.h"

CScoreUIScript::CScoreUIScript()
	: CScript(SCRIPT_TYPE::SCOREUISCRIPT)
	, m_FontText(L"0")
	, PosX(800.f)
	, PosY(180.f)
{
}

CScoreUIScript::~CScoreUIScript()
{
}


void CScoreUIScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &PosX, L"PosX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &PosY, L"PosY", true, 0.f);
}

void CScoreUIScript::Begin()
{
}

void CScoreUIScript::Tick()
{
	int score = GamePlayMgr::GetInst()->GetScore();

	m_FontText = to_wstring(score);

	FontMgr::GetInst()->DrawFontOutline(m_FontText.c_str(), PosX, PosY, 50, FONT_RGBA(255, 255, 255, 255), FONT_RGBA(0, 0, 0, 255), 3.f);
}
