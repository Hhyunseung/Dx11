#include "pch.h"
#include "AMaterial.h"

#include "Device.h"


AMaterial::AMaterial()
	: Asset(ASSET_TYPE::MATERIAL)
	, m_Const{}
	, m_Domain(RENDER_DOMAIN::DOMAIN_NONE)
{

}

AMaterial::AMaterial(const AMaterial& _Other)
	: Asset(_Other) // 부모 클래스의 복사 생성자 호출
	, m_Shader(_Other.m_Shader)
	, m_Tex{}
	, m_Domain(_Other.m_Domain)
	, m_Const(_Other.m_Const)
{
	for (int i = 0; i < TEX_END; ++i)
	{
		m_Tex[i] = _Other.m_Tex[i];
	}
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
		{
			m_Const.IsTex[i] = 0; // 텍스쳐가 없는 경우, 셰이더에서 해당 텍스쳐를 사용하지 않도록 플래그 설정
			continue;
		}

		/// 각 텍스쳐가 자신의 텍스쳐 레지스터에 바인딩 하도록 요청
		m_Tex[i]->Binding(i);
		m_Const.IsTex[0] = 1; // 텍스쳐가 있는 경우, 셰이더에서 해당 텍스쳐를 사용하도록 플래그 설정 
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


// 복사생성을 하려면 그 부모 클래스인 Asset 의 복사생성자도 만들어야 하는데, Asset 클래스는 복사생성자를 만들 필요가 없다
// 하지만 Entity 클래스는 복사생성자를 만들어야 하는데 (객체 마다 고유 ID 가 있기 때문에) Entity 클래스의 복사생성자에서 ID 가 겹치지 않도록 구현해야 한다
// 이미 만들어 두었으므로 AMaterial 클래스의 복사생성자는 만들어 줄 필요가 없다

AMaterial* AMaterial::Clone()
{
	return new AMaterial(*this);
}