#include "pch.h"
#include "CHPBarScript.h"

#include "GamePlayMgr.h"
#include "GameObject.h"

CHPBarScript::CHPBarScript()
	: CScript(SCRIPT_TYPE::HPBARSCRIPT)
	, m_HPBarFill(nullptr)
	, m_OriginWidth(0.f)
	, m_OriginPos(Vec3(0.f, 0.f, 0.f))
	, m_OriginScale(Vec3(1.f, 1.f, 1.f))
{
}

CHPBarScript::~CHPBarScript()
{
}

void CHPBarScript::Begin()
{
	Ptr<GameObject> pChild = nullptr;

	pChild = GetOwner()->GetChild(0);
	if (pChild != nullptr)
	{
		m_HPBarFill = pChild.Get();

		m_OriginPos = m_HPBarFill->Transform()->GetRelativePos();
		m_OriginScale = m_HPBarFill->Transform()->GetRelativeScale();
		m_OriginWidth = m_OriginScale.x;
	}
}

void CHPBarScript::SetHP(int _CurHP, int _MaxHP)
{
	if (m_HPBarFill == nullptr)
		return;

	float ratio = 0.f;

	if (_MaxHP > 0)
		ratio = (float)_CurHP / (float)_MaxHP;

	// 안전 처리
	if (ratio < 0.f) ratio = 0.f;
	if (ratio > 1.f) ratio = 1.f;

	// 새로운 너비 계산
	float newWidth = m_OriginWidth * ratio;

	// ⭐ 왼쪽 고정을 위한 위치 보정
	float offsetX = (m_OriginWidth - newWidth) * 0.5f;

	// 스케일 적용
	Vec3 scale = m_OriginScale;
	scale.x = newWidth;
	m_HPBarFill->Transform()->SetRelativeScale(scale);

	// 위치 적용 (왼쪽 기준 유지)
	Vec3 pos = m_OriginPos;
	pos.x = m_OriginPos.x - offsetX;
	m_HPBarFill->Transform()->SetRelativePos(pos);
}


void CHPBarScript::Tick()
{
}
