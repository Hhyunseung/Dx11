#include "pch.h"
#include "CPendulumObstacle.h"

#include "GameObject.h"
#include "TimeMgr.h"
#include "GamePlayMgr.h"

CPendulumObstacle::CPendulumObstacle()
	: CObstructScript(SCRIPT_TYPE::PENDULUMOBSTACLE)
	, m_LocalCenterOffsetX(0.f)
	, m_LocalCenterOffsetY(0.f)
	, m_AccTime(0.f)
	, m_MaxAngle(30.f) // 최대 각도 (예시: 30도)
	, m_Frequency(1.f) // 진동 주파수 (예시: 1Hz)
	, m_PendulumLength(100.f)
	, m_OneMove(false)
	, m_IsStopped(false)
	, m_OneMoveTime(0.5f)
	, m_pBody(nullptr)
{
}

CPendulumObstacle::~CPendulumObstacle()
{
}


void CPendulumObstacle::Init()
{
	CObstructScript::Init(); // 부모 클래스의 Init() 호출
	// store X/Y separately so StageData can save them independently
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LocalCenterOffsetX, L"LocalCenterOffsetX", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LocalCenterOffsetY, L"LocalCenterOffsetY", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxAngle,			L"MaxAngle", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Frequency,			L"Frequency", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PendulumLength,		L"PendulumLength", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL,	&m_OneMove,				L"OneMove", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_OneMoveTime,			L"OneMoveTime", true, 0.f);
}

void CPendulumObstacle::ApplySpawnInfo(const FSpawnInfo& info)
{
	m_AccTime = 0.f;
	m_IsStopped = false;

	auto itF = info.FloatParams.find("LocalCenterOffsetX");
	if (itF != info.FloatParams.end())
		m_LocalCenterOffsetX = itF->second;

	itF = info.FloatParams.find("LocalCenterOffsetY");
	if (itF != info.FloatParams.end())
		m_LocalCenterOffsetY = itF->second;

	itF = info.FloatParams.find("MaxAngle");
	if (itF != info.FloatParams.end())
		m_MaxAngle = itF->second;

	itF = info.FloatParams.find("Frequency");
	if (itF != info.FloatParams.end())
		m_Frequency = itF->second;

	itF = info.FloatParams.find("PendulumLength");
	if (itF != info.FloatParams.end())
		m_PendulumLength = itF->second;

	itF = info.FloatParams.find("OneMoveTime");
	if (itF != info.FloatParams.end())
		m_OneMoveTime = itF->second;

	// bool params: support specific X/Y flags or generic IsGrounded
	auto itBX = info.BoolParams.find("OneMove");
	if (itBX != info.BoolParams.end())
		m_OneMove = itBX->second;



    Vec3 rot = GetOwner()->Transform()->GetRelativeRot();
	rot.z = 0.f;
	GetOwner()->Transform()->SetRelativeRot(rot);


	if (GetOwner()->GetChild().size() > 0)
	{
		m_pBody = GetOwner()->GetChild(0).Get(); // 첫 번째 자식 오브젝트를 몸체로 사용
	}

	// Body는 Pivot 아래쪽에 고정
    if (m_pBody)
	{
		// Position the pendulum body relative to the pivot once on spawn.
		//m_pBody->Transform()->SetRelativePos(Vec3(m_LocalCenterOffsetX, m_LocalCenterOffsetY, 0.f));

		Vec3 bodyRot = m_pBody->Transform()->GetRelativeRot();
		bodyRot.z = 0.f;
		m_pBody->Transform()->SetRelativeRot(bodyRot);
	}
}


void CPendulumObstacle::Move()
{
	if (m_OneMove && m_IsStopped)
		return;

	m_AccTime += DT;

	// (degree -> radian) 
	float maxAngleRad = XMConvertToRadians(m_MaxAngle);
	float currentAngle = 0.f;

	if (m_OneMove)
	{
		float t = m_AccTime / m_OneMoveTime; 

		if (t >= 1.f)
		{
			t = 1.f;
			m_IsStopped = true; // 움직임 멈춤
		}

		currentAngle = maxAngleRad + (2.f * -maxAngleRad * t);
	}
	else
	{
		// sin (2 * PI * frequency * time) 사인 그래프 ... maxAngleRad 범위 내에서 왕복하게 된다
		currentAngle = maxAngleRad * sinf(XM_2PI * m_Frequency * m_AccTime);
	}


	Vec3 rot = GetOwner()->Transform()->GetRelativeRot();
	rot.z = currentAngle; // Z-axis rotation
	GetOwner()->Transform()->SetRelativeRot(rot);

    if (m_pBody)
	{
		//m_pBody->Transform()->SetRelativePos(Vec3(m_LocalCenterOffsetX, m_LocalCenterOffsetY, 0.f));
	}
}

void CPendulumObstacle::SaveToLevelFile(FILE* _File)
{
}

void CPendulumObstacle::LoadFromLevelFile(FILE* _File)
{
}

