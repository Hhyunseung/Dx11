#include "pch.h"
#include "CBGScrollScript.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"
#include "GameObject.h"
#include "CMeshRender.h"

CBGScrollScript::CBGScrollScript()
	:CScript((UINT)SCRIPT_TYPE::BGSCROLLSCRIPT)
	, m_SpeedX(0.05f)
	, m_SpeedY(0.f)
	, m_UV(0.f, 0.f)
{
}

CBGScrollScript::~CBGScrollScript()
{
}

void CBGScrollScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpeedX, L"SpeedX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpeedY, L"SpeedY", true, 0.f);
}

void CBGScrollScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	Ptr<CMeshRender> pMeshRender = GetOwner()->MeshRender();

	if (pMeshRender == nullptr || pMeshRender->GetMaterial() == nullptr)
		return;

	m_UV.x += m_SpeedX * DT;
	m_UV.y += m_SpeedY * DT;

	pMeshRender->GetMaterial()->SetScalar(VEC2_0, m_UV);
}

void CBGScrollScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_SpeedX, sizeof(float), 1, _File);
	fwrite(&m_SpeedY, sizeof(float), 1, _File);
}

void CBGScrollScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_SpeedX, sizeof(float), 1, _File);
	fread(&m_SpeedY, sizeof(float), 1, _File);
}
