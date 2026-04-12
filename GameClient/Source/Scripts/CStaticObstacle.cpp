#include "pch.h"
#include "CStaticObstacle.h"

#include "GameObject.h"
#include "TimeMgr.h"

CStaticObstacle::CStaticObstacle()
	: CObstructScript(SCRIPT_TYPE::STATICOBSTACLE)
	, m_Speed(0.f)
{
}

CStaticObstacle::~CStaticObstacle()
{
}

void CStaticObstacle::Init()
{
	CObstructScript::Init();
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);
}

void CStaticObstacle::ApplySpawnInfo(const FSpawnInfo& info)
{
	auto itF = info.FloatParams.find("Speed");
	if (itF != info.FloatParams.end())
		m_Speed = itF->second;
}


void CStaticObstacle::Move()
{
	Vec3 pos = Transform()->GetRelativePos();
	pos.x -= m_Speed * DT;
	Transform()->SetRelativePos(pos);
}

void CStaticObstacle::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CStaticObstacle::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
