#include "pch.h"

#include "CRenderComponent.h"
#include "LevelMgr.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: Component(_Type)
{
	// 가상함수는 생성자시점에 호출불가능
	//CreateMaterial();
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<AMaterial> _Mtrl)
{
	m_Mtrl = m_SharedMtrl = _Mtrl;
}

Ptr<AMaterial> CRenderComponent::GetSharedMaterial()
{
	m_Mtrl = m_SharedMtrl;
	return m_SharedMtrl;
}

Ptr<AMaterial> CRenderComponent::CreateDynamicMaterial()
{
	// 동적 재질 생성은 반드시 레벨이 Play 모드일 경우에만 사용 가능한 기능
	assert(LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState());

	if (nullptr != m_DynamicMtrl)
	{
		m_Mtrl = m_DynamicMtrl;
		return m_DynamicMtrl;
	}
	else
	{
		m_Mtrl = m_DynamicMtrl = m_SharedMtrl->Clone();
		return m_DynamicMtrl;
	}
}

void CRenderComponent::Init()
{
	CreateMaterial();
}
