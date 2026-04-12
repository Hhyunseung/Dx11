#include "pch.h"
#include "CTextUIScript.h"

#include "GamePlayMgr.h"
#include "GameDataMgr.h"
#include "FontMgr.h"

CTextUIScript::CTextUIScript()
	: CScript(SCRIPT_TYPE::TEXTUISCRIPT)
	, m_BindType(TEXT_BIND_TYPE::NONE)
	, m_Text(L"0")
	, m_PosX(0.f)
	, m_PosY(0.f)
	, m_FontSize(50.f)
	, m_OutlineThickness(3.f)
{
}

CTextUIScript::~CTextUIScript()
{
}

void CTextUIScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::TEXT, &m_Text, L"Text", true, 0);
	AddScriptParam(SCRIPT_PARAM::INT, &m_BindType, L"BindType", true, 0);

	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PosX, L"PosX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PosY, L"PosY", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FontSize, L"FontSize", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_OutlineThickness, L"OutlineThickness", true, 0.f);
}

void CTextUIScript::Tick()
{
	switch (m_BindType)
	{
	case TEXT_BIND_TYPE::NONE:
		// m_Text를 그대로 사용
		break;

	case TEXT_BIND_TYPE::SCORE:
	{
		int score = GamePlayMgr::GetInst()->GetScore();
		m_Text = FormatWithComma(score);
	}
	break;

	case TEXT_BIND_TYPE::RUN_COIN:
	{
		int coin = GamePlayMgr::GetInst()->GetRunCoin();
		m_Text = FormatWithComma(coin);
	}
	break;

	case TEXT_BIND_TYPE::TOTAL_COIN:
	{
		int coin = GameDataMgr::GetInst()->GetTotalCoin();
		m_Text = FormatWithComma(coin);
	}
	break;

	case TEXT_BIND_TYPE::CRYSTAL:
	{
		//int crystal = GamePlayMgr::GetInst()->GetCrystal();
		//m_Text = FormatWithComma(crystal);
	}
	break;

	case TEXT_BIND_TYPE::CURRENT_LEVEL:
	{
		//int level = GamePlayMgr::GetInst()->GetCurrentLevel();
		//m_Text = to_wstring(level);
	}
	break;

	default:
		break;
	}

	FontMgr::GetInst()->DrawFontOutline(
		m_Text.c_str(),
		m_PosX,
		m_PosY,
		m_FontSize,
		FONT_RGBA(255, 255, 255, 255),
		FONT_RGBA(0, 0, 0, 255),
		m_OutlineThickness
	);
}

void CTextUIScript::SaveToLevelFile(FILE* _File)
{
	size_t len = m_Text.size();
	fwrite(&len, sizeof(size_t), 1, _File);

	if (len > 0)
		fwrite(m_Text.c_str(), sizeof(wchar_t), len, _File);

	fwrite(&m_BindType, sizeof(TEXT_BIND_TYPE), 1, _File);
	fwrite(&m_PosX, sizeof(float), 1, _File);
	fwrite(&m_PosY, sizeof(float), 1, _File);
	fwrite(&m_FontSize, sizeof(float), 1, _File);
	fwrite(&m_OutlineThickness, sizeof(float), 1, _File);
}

void CTextUIScript::LoadFromLevelFile(FILE* _File)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);

	m_Text.clear();
	if (len > 0)
	{
		m_Text.resize(len);
		fread(m_Text.data(), sizeof(wchar_t), len, _File);
	}

	fread(&m_BindType, sizeof(TEXT_BIND_TYPE), 1, _File);
	fread(&m_PosX, sizeof(float), 1, _File);
	fread(&m_PosY, sizeof(float), 1, _File);
	fread(&m_FontSize, sizeof(float), 1, _File);
	fread(&m_OutlineThickness, sizeof(float), 1, _File);
}