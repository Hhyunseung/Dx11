#include "pch.h"
#include "CMovingPlatformScirpt.h"

#include "GamePlayMgr.h"
#include "GameObject.h"
#include "TimeMgr.h"

CMovingPlatformScirpt::CMovingPlatformScirpt()
	: CScript(SCRIPT_TYPE::MOVINGPLATFORMSCIRPT)
	, m_StartPos(Vec3(0.f, 0.f, 0.f))
	, m_EndPos(Vec3(0.f, 0.f, 0.f))
	, m_PrevPos(Vec3(0.f, 0.f, 0.f))
	, m_FrameDelta(Vec3(0.f, 0.f, 0.f))
	, m_EndPosX(300.f)
	, m_EndPosY(0.f)
	, m_Speed(100.f)
	, m_IsMovingToEnd(true)
	, m_ObjectID(EObjectID::TileShipeStage_1_fh_m)
{
}

CMovingPlatformScirpt::~CMovingPlatformScirpt()
{
}

void CMovingPlatformScirpt::Init()
{
	AddScriptParam(SCRIPT_PARAM::EObjectID, &m_ObjectID, L"ObjectID", true, 0.f);

	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EndPosX, L"EndPosX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EndPosY, L"EndPosY", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);


}

void CMovingPlatformScirpt::OnSpawn()
{

}

void CMovingPlatformScirpt::ApplySpawnInfo(const FSpawnInfo& info)
{
	auto itF = info.FloatParams.find("EndPosX");
	if (itF != info.FloatParams.end())
		m_EndPosX = itF->second;

	itF = info.FloatParams.find("EndPosY");
	if (itF != info.FloatParams.end())
		m_EndPosY = itF->second;

	itF = info.FloatParams.find("Speed");
	if (itF != info.FloatParams.end())
		m_Speed = itF->second;

	m_StartPos = GetOwner()->Transform()->GetRelativePos();
	m_EndPos = Vec3(m_EndPosX, m_EndPosY, 100.f) + m_StartPos;
	m_PrevPos = m_StartPos;
	m_FrameDelta = Vec3(0.f, 0.f, 0.f);
}

void CMovingPlatformScirpt::Begin()
{

}

void CMovingPlatformScirpt::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vTarget = m_IsMovingToEnd ? m_EndPos : m_StartPos;

	Vec3 vDir = vTarget - vPos;


	vDir.Normalize();
	Vec3 vNext = vPos + vDir * m_Speed * DT;

	if ((vTarget - vPos).Length() < m_Speed * DT)
		vNext = vTarget;

	vPos = vNext;

	Transform()->SetRelativePos(vPos);

	m_FrameDelta = vPos - m_PrevPos;
	m_PrevPos = vPos;
}

void CMovingPlatformScirpt::SaveToLevelFile(FILE* _File)
{
}

void CMovingPlatformScirpt::LoadFromLevelFile(FILE* _File)
{
}

