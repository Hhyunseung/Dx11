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
	if (GetMtrl() == nullptr || GetMesh() == nullptr)
		return;

	GetMtrl()->Binding();
	GetMesh()->Render();
	GetMtrl()->Clear();
}
