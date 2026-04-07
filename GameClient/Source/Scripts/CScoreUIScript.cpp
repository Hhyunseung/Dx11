#include "pch.h"
#include "CScoreUIScript.h"

#include "GamePlayMgr.h"
#include "FontMgr.h"

CScoreUIScript::CScoreUIScript()
	: CScript(SCRIPT_TYPE::SCOREUISCRIPT)
	, m_FontText(L"0")
{
}

CScoreUIScript::~CScoreUIScript()
{
}


void CScoreUIScript::Begin()
{
}

void CScoreUIScript::Tick()
{
	int score = GamePlayMgr::GetInst()->GetScore();

	m_FontText = to_wstring(score);

	FontMgr::GetInst()->DrawFont(m_FontText.c_str(), 600, 300, 50, FONT_RGBA(255, 255, 255, 255));
}
