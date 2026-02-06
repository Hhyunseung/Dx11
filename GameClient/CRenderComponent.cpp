#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: Component(_Type)
{
	// 가상함수는 생성자시점에 호출불가능
	//CreateMaterial();
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::Init()
{
	CreateMaterial();
}
