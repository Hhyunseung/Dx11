#include "pch.h"
#include "CPendulumObstacle.h"

#include "GameObject.h"
#include "TimeMgr.h"

CPendulumObstacle::CPendulumObstacle()
	: CObstructScript(SCRIPT_TYPE::PENDULUMOBSTACLE)
	, m_PivotPos(0.f, 0.f, 0.f)
	, m_PivotToCenter(0.f, 200.f, 0.f)
	, m_AccTime(0.f)
	, m_MaxAngle(30.f) // 최대 각도 (예시: 30도)
	, m_Frequency(1.f) // 진동 주파수 (예시: 1Hz)
	, m_PendulumLength(100.f) 
	, m_OneMove(false)
	, m_IsStopped(false)
	, m_OneMoveTime(0.5f)
{
}

CPendulumObstacle::~CPendulumObstacle()
{
}


void CPendulumObstacle::Init()
{
	CObstructScript::Init(); // 부모 클래스의 Init() 호출

	AddScriptParam(SCRIPT_PARAM::VEC4, &m_PivotToCenter, L"PivotToCenter", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxAngle, L"MaxAngle", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Frequency, L"Frequency", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PendulumLength, L"PendulumLength", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_OneMove, L"OneMove", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_OneMoveTime, L"OneMoveTime", true, 0.f);

	Vec3 startPos = GetOwner()->Transform()->GetRelativePos();
	m_PivotPos = startPos; // 회전 중심 위치는 초기 위치를 기준으로 설정
	m_PivotPos += m_PivotToCenter;
}

void CPendulumObstacle::ApplySpawnInfo(const FSpawnInfo& info)
{

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


	// 피봇에서 중심까지의 벡터 // O -----> P 에서 P -----> O로 바꿔준다 (회전 방향이 반대이므로)
	Vec3 centerOffset = -m_PivotToCenter;


	float c = cosf(currentAngle);
	float s = sinf(currentAngle);
	
	// 2D 회전 공식
	// x' = x cosθ - y sinθ
	// y' = x sinθ + y cosθ
	Vec3 rotatedOffset;
	rotatedOffset.x = centerOffset.x * c - centerOffset.y * s;
	rotatedOffset.y = centerOffset.x * s + centerOffset.y * c;
	rotatedOffset.z = centerOffset.z;

	// 회전된 위치 = 피봇 위치 + 회전된 오프셋
	Vec3 pos = m_PivotPos + rotatedOffset;
	GetOwner()->Transform()->SetRelativePos(pos);

	Vec3 rot = GetOwner()->Transform()->GetRelativeRot();
	rot.z = currentAngle; // Z-axis rotation
	GetOwner()->Transform()->SetRelativeRot(rot);
}

void CPendulumObstacle::SaveToLevelFile(FILE* _File)
{
}

void CPendulumObstacle::LoadFromLevelFile(FILE* _File)
{
}

