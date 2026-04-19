#include "pch.h"
#include "CShockWaveScript.h"

#include "CTransform.h"
#include "TimeMgr.h"

CShockWaveScript::CShockWaveScript()
	:CScript((int)SCRIPT_TYPE::SHOCKWAVESCRIPT)
{
}

CShockWaveScript::~CShockWaveScript()
{
}

void CShockWaveScript::Begin()
{
}

void CShockWaveScript::Tick()
{
	Vec3 vScale = Transform()->GetRelativeScale();

	vScale += Vec3(700.f * DT, 700.f * DT, 0.f);

	if (vScale.x >= 1000.f)
		vScale = Vec3(-150.f, -150.f, 1.f);

	Transform()->SetRelativeScale(vScale);
}

void CShockWaveScript::SaveToLevelFile(FILE* _File)
{
}

void CShockWaveScript::LoadFromLevelFile(FILE* _File)
{
}


