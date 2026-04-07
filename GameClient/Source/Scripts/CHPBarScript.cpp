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

	Ptr<CMeshRender> pMR = m_HPBarFill->MeshRender();
	if (pMR == nullptr)
		return;

	Ptr<AMaterial> pMtrl = pMR->GetMaterial();
	if (pMtrl == nullptr)
		return;

	float ratio = 0.f;

	if (_MaxHP > 0)
		ratio = (float)_CurHP / (float)_MaxHP;

	// 안전 처리
	if (ratio < 0.f) ratio = 0.f;
	if (ratio > 1.f) ratio = 1.f;

	pMtrl->SetScalar(FLOAT_0, ratio); // 셰이더에서 이 값을 받아서 HPBarFill의 텍스처 좌표를 조절하도록 설정
}


void CHPBarScript::Tick()
{
}
