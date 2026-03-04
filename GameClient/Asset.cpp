#include "pch.h"
#include "Asset.h"



Asset::Asset(ASSET_TYPE _Type)
	: m_Type(_Type)
{
}

Asset::Asset(const Asset& _Origin)
	: Entity(_Origin) /// 부모 클래스의 복사 생성자 호출
	, m_Key(_Origin.m_Key)
	, m_RelativePath() /// 상대 경로는 복사 생성자에서 초기화 하지 않음 /// 경로에서 가져오는게 아니라 메모리에서 복사해서 가져와야하니까
	, m_Type(_Origin.m_Type)
{

}

Asset::~Asset()
{
}
