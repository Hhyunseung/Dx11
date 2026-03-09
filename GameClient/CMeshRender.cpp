#include "pch.h"
#include "CMeshRender.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::FinalTick()
{
}


void CMeshRender::Render()
{
	if (GetMaterial() == nullptr || GetMesh() == nullptr)
		return;

	GetMaterial()->Binding();
	GetMesh()->Render();
	GetMaterial()->Clear();
}

