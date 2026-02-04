#include "pch.h"
#include "AMaterial.h"

#include "Device.h"

AMaterial::AMaterial()
	: Asset(ASSET_TYPE::MATERIAL)
	, m_Const{}
{

}

AMaterial::~AMaterial()
{
}


void AMaterial::Binding()
{
	m_Shader->Binding();

	for (int i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_Tex[i])
			continue;

		/// 각 텍스쳐가 자신의 텍스쳐 레지스터에 바인딩 하도록 요청
		m_Tex[i]->Binding(i);
	}

	Device::GetInst()->GetCB(CB_TYPE::MATERIAL)->SetData(&m_Const);
	Device::GetInst()->GetCB(CB_TYPE::MATERIAL)->Binding();
}

void AMaterial::Clear()
{
	for (int i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_Tex[i])
			continue;

		m_Tex[i]->Clear();
	}
}
