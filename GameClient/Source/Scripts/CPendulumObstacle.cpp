#include "pch.h"
#include "CPendulumObstacle.h"

#include "GameObject.h"
#include "TimeMgr.h"

CPendulumObstacle::CPendulumObstacle()
	: CObstructScript(SCRIPT_TYPE::PENDULUMOBSTACLE)
	, m_InitialPos(100.f, 0.f, 0.f)
	, m_AccTime(0.f)
	, m_MaxAngle(30.f) // 최대 각도 (예시: 30도)
	, m_Frequency(1.f) // 진동 주파수 (예시: 1Hz)
	, m_PendulumLength(100.f) // 진자의 길이 (예시: 100 units)
{
}

CPendulumObstacle::~CPendulumObstacle()
{
}


void CPendulumObstacle::Init()
{
	CObstructScript::Init(); // 부모 클래스의 Init() 호출

	AddScriptParam(SCRIPT_PARAM::VEC4, &m_InitialPos, L"InitialPos", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AccTime, L"AccTime", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxAngle, L"MaxAngle", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Frequency, L"Frequency", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PendulumLength, L"PendulumLength", true, 0.f);


}

void CPendulumObstacle::ApplySpawnInfo(const FSpawnInfo& info)
{
}


void CPendulumObstacle::Move()
{
	m_AccTime += DT;

	// 최대 각도(degree -> radian)
	float maxAngleRad = XMConvertToRadians(m_MaxAngle);

	// 현재 각도
	float currentAngle = maxAngleRad * sinf(XM_2PI * m_Frequency * m_AccTime);

	// 진자의 회전 중심에서 현재 각도에 따른 위치 계산
	Vec3 pivotPos = m_InitialPos;
	pivotPos.y += m_PendulumLength;

	Vec3 pos;
	pos.x = pivotPos.x + m_PendulumLength * sinf(currentAngle);
	pos.y = pivotPos.y - m_PendulumLength * cosf(currentAngle);
	pos.z = m_InitialPos.z;

	GetOwner()->Transform()->SetRelativePos(pos);


	// 오브젝트 회전
	Vec3 rot = Transform()->GetRelativeRot();
	rot.z = currentAngle; // Z축 회전 적용
	GetOwner()->Transform()->SetRelativeRot(rot);
}

void CPendulumObstacle::SaveToLevelFile(FILE* _File)
{
}

void CPendulumObstacle::LoadFromLevelFile(FILE* _File)
{
}

