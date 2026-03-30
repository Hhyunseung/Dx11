#include "pch.h"
#include "CLinearObstacle.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"

CLinearObstacle::CLinearObstacle()
	: CObstructScript(SCRIPT_TYPE::LINEAROBSTACLE)
	, m_SpeedX(0.f)
	, m_SpeedY(0.f)
	, m_GroundX(0.f)
	, m_GroundY(0.f)
{
}

CLinearObstacle::~CLinearObstacle()
{
}


void CLinearObstacle::OnSpawn()
{

}

void CLinearObstacle::ApplySpawnInfo(const FSpawnInfo& info)
{
	// float params
	auto itF = info.FloatParams.find("SpeedX");
	if (itF != info.FloatParams.end())
		m_SpeedX = itF->second;

	itF = info.FloatParams.find("SpeedY");
	if (itF != info.FloatParams.end())
		m_SpeedY = itF->second;

    itF = info.FloatParams.find("GroundX");
	if (itF != info.FloatParams.end())
		m_GroundX = itF->second;

	itF = info.FloatParams.find("GroundY");
	if (itF != info.FloatParams.end())
		m_GroundY = itF->second;

    // bool params: support specific X/Y flags or generic IsGrounded
	auto itBX = info.BoolParams.find("IsGroundedX");
	if (itBX != info.BoolParams.end())
		m_IsGroundedX = itBX->second;

	auto itBY = info.BoolParams.find("IsGroundedY");
	if (itBY != info.BoolParams.end())
		m_IsGroundedY = itBY->second;
}

void CLinearObstacle::Init()
{
	CObstructScript::Init();
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpeedX, L"SpeedX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpeedY, L"SpeedY", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_GroundX, L"GroundX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_GroundY, L"GroundY", true, 0.f);
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsGroundedX, L"IsGroundedX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsGroundedY, L"IsGroundedY", true, 0.f);
}

void CLinearObstacle::Move()
{
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();

	if (m_IsGroundedX)
	{
		pos.x += m_SpeedX * DT;
		pos.y += m_SpeedY * DT;

		if (pos.x <= m_GroundX)
		{
			pos.x = m_GroundX;
			m_IsGroundedX = false;
			GetOwner()->FlipbookRender()->Play(1, 4.f, 1);
		}
	}

	if (m_IsGroundedY)
	{
		pos.x += m_SpeedX * DT;
		pos.y += m_SpeedY * DT;

		if (pos.y <= m_GroundY)
		{
			pos.y = m_GroundY;
			m_IsGroundedY = false;
			GetOwner()->FlipbookRender()->Play(1, 4.f, 1);
		}
	}

	// always apply world scroll
	pos.x -= scrollSpeed * DT;

	Transform()->SetRelativePos(pos);
}


void CLinearObstacle::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_SpeedX, sizeof(float), 1, _File);
	fwrite(&m_SpeedY, sizeof(float), 1, _File);
	fwrite(&m_GroundX, sizeof(float), 1, _File);
	fwrite(&m_GroundY, sizeof(float), 1, _File);
	fwrite(&m_IsGroundedX, sizeof(bool), 1, _File);
	fwrite(&m_IsGroundedY, sizeof(bool), 1, _File);
}

void CLinearObstacle::LoadFromLevelFile(FILE* _File)
{
	fread(&m_SpeedX, sizeof(float), 1, _File);
	fread(&m_SpeedY, sizeof(float), 1, _File);
	fread(&m_GroundX, sizeof(float), 1, _File);
	fread(&m_GroundY, sizeof(float), 1, _File);
	fread(&m_IsGroundedX, sizeof(bool), 1, _File);
	fread(&m_IsGroundedY, sizeof(bool), 1, _File);
}
