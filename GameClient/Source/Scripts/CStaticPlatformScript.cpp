#include "pch.h"
#include "CStaticPlatformScript.h"


CStaticPlatformScript::CStaticPlatformScript()
	: CScript(SCRIPT_TYPE::STATICPLATFORMSCRIPT)
{
}

CStaticPlatformScript::~CStaticPlatformScript()
{
}


void CStaticPlatformScript::Init()
{
}

void CStaticPlatformScript::ApplySpawnInfo(const FSpawnInfo& info)
{
}

void CStaticPlatformScript::Begin()
{
}

void CStaticPlatformScript::Tick()
{
}

void CStaticPlatformScript::SaveToLevelFile(FILE* _File)
{
}

void CStaticPlatformScript::LoadFromLevelFile(FILE* _File)
{
}
