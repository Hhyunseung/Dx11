#include "pch.h"
#include "CObstructScript.h"

#include "TimeMgr.h"
#include "GamePlayMgr.h"
#include "ObjectPoolMgr.h"
#include "GameObject.h"

CObstructScript::CObstructScript()
	: CScript(SCRIPT_TYPE::OBSTRUCTSCRIPT)
	, m_MoveType(EObstacleMovement::Static)
	, m_Speed(100.f)
	, m_Direction(Vec2(-1.f, 0.f))
	, m_Gravity(980.f)
	, m_InitialVelocityY(0.f)
	, m_StopOnGround(true)
	, m_GroundY(-200.f)
	, m_Amplitude(50.f)
	, m_Frequency(2.f)
	, m_MaxAngle(45.f)
	, m_PendulumLength(100.f)
	, m_AccTime(0.f)
	, m_CurrentVelocityY(0.f)
	, m_InitialPos(Vec3(0.f, 0.f, 0.f))
	, m_IsGrounded(false)
{
}

CObstructScript::~CObstructScript()
{
}

void CObstructScript::Init()
{
	// Inspector에서 편집 가능한 파라미터 등록
	AddScriptParam(SCRIPT_PARAM::INT, &m_MoveType, L"MoveType(0:Static,1:Linear,2:Para,3:Sine,4:Pendulum)");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed");

	// Linear
	AddScriptParam(SCRIPT_PARAM::VEC2, &m_Direction, L"Direction");

	// Parabolic
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Gravity, L"Gravity");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitialVelocityY, L"InitialVelocityY");
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_StopOnGround, L"StopOnGround");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_GroundY, L"GroundY");

	// Sine
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Amplitude, L"Amplitude");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Frequency, L"Frequency");

	// Pendulum (바이킹)
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxAngle, L"MaxAngle(deg)");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PendulumLength, L"PendulumLength");
}

void CObstructScript::OnSpawn()
{
	// 스폰 시 상태 초기화
	m_AccTime = 0.f;
	m_CurrentVelocityY = m_InitialVelocityY;
	m_InitialPos = Transform()->GetRelativePos();
	m_IsGrounded = false;
}

void CObstructScript::Tick()
{
	m_AccTime += DT;

	// 이동 타입에 따라 처리
	switch (m_MoveType)
	{
	case EObstacleMovement::Static:
		MoveStatic();
		break;
	case EObstacleMovement::Linear:
		MoveLinear();
		break;
	case EObstacleMovement::Parabolic:
		MoveParabolic();
		break;
	case EObstacleMovement::Sine:
		MoveSine();
		break;
	case EObstacleMovement::Pendulum:
		MovePendulum();
		break;
	}
}

void CObstructScript::MoveStatic()
{
	// 월드 스크롤만 적용 (GamePlayMgr에서 속도 읽기)
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();
	pos.x -= scrollSpeed * DT;
	Transform()->SetRelativePos(pos);
}

void CObstructScript::MoveLinear()
{
	// 월드 스크롤 + 자체 이동
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();

	// 월드 스크롤 적용
	pos.x -= scrollSpeed * DT;

	// 자체 방향으로 이동
	pos.x += m_Direction.x * m_Speed * DT;
	pos.y += m_Direction.y * m_Speed * DT;

	Transform()->SetRelativePos(pos);
}

void CObstructScript::MoveParabolic()
{
	// 월드 스크롤 + 포물선 이동
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();

	// 월드 스크롤 적용
	pos.x -= scrollSpeed * DT;

	// 바닥에 닿지 않았을 때만 Y 이동
	if (!m_IsGrounded)
	{
		// 중력 적용
		m_CurrentVelocityY -= m_Gravity * DT;
		pos.y += m_CurrentVelocityY * DT;

		// 바닥 체크
		if (m_StopOnGround && pos.y <= m_GroundY)
		{
			pos.y = m_GroundY;
			m_CurrentVelocityY = 0.f;
			m_IsGrounded = true;
		}
	}

	Transform()->SetRelativePos(pos);
}

void CObstructScript::MoveSine()
{
	// 월드 스크롤 + 사인파 이동
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();

	// 월드 스크롤 적용
	pos.x -= scrollSpeed * DT;

	// 사인파로 Y 위치 조정 (초기 위치 기준)
	float sineOffset = m_Amplitude * sinf(m_Frequency * m_AccTime * XM_2PI);
	pos.y = m_InitialPos.y + sineOffset;

	Transform()->SetRelativePos(pos);
}

void CObstructScript::MovePendulum()
{
	// 바이킹처럼 진자 운동 (회전 중심에서 흔들림)
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();

	// 각도 계산 (도 -> 라디안)
	float maxAngleRad = m_MaxAngle * XM_PI / 180.f;
	float currentAngle = maxAngleRad * sinf(m_Frequency * m_AccTime * XM_2PI);

	// 회전 중심(피봇)에서 진자 길이만큼 떨어진 위치 계산
	// 피봇은 초기 위치의 위쪽 (m_PendulumLength만큼)
	Vec3 pivotPos = m_InitialPos;
	pivotPos.y += m_PendulumLength;

	// 월드 스크롤 적용 (피봇 X 위치 업데이트)
	pivotPos.x -= scrollSpeed * m_AccTime;  // 누적 시간 기반

	// 실제로는 매 프레임 InitialPos.x를 업데이트해야 정확함
	Vec3 pos;
	pos.x = pivotPos.x + m_PendulumLength * sinf(currentAngle);
	pos.y = pivotPos.y - m_PendulumLength * cosf(currentAngle);
	pos.z = m_InitialPos.z;

	Transform()->SetRelativePos(pos);

	// 오브젝트 회전 (바이킹 배처럼 기울어짐)
	Vec3 rot = Transform()->GetRelativeRot();
	rot.z = currentAngle;  // Z축 회전
	Transform()->SetRelativeRot(rot);
}

void CObstructScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_MoveType, sizeof(EObstacleMovement), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File);
	fwrite(&m_Direction, sizeof(Vec2), 1, _File);
	fwrite(&m_Gravity, sizeof(float), 1, _File);
	fwrite(&m_InitialVelocityY, sizeof(float), 1, _File);
	fwrite(&m_StopOnGround, sizeof(bool), 1, _File);
	fwrite(&m_GroundY, sizeof(float), 1, _File);
	fwrite(&m_Amplitude, sizeof(float), 1, _File);
	fwrite(&m_Frequency, sizeof(float), 1, _File);
	fwrite(&m_MaxAngle, sizeof(float), 1, _File);
	fwrite(&m_PendulumLength, sizeof(float), 1, _File);
}

void CObstructScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_MoveType, sizeof(EObstacleMovement), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
	fread(&m_Direction, sizeof(Vec2), 1, _File);
	fread(&m_Gravity, sizeof(float), 1, _File);
	fread(&m_InitialVelocityY, sizeof(float), 1, _File);
	fread(&m_StopOnGround, sizeof(bool), 1, _File);
	fread(&m_GroundY, sizeof(float), 1, _File);
	fread(&m_Amplitude, sizeof(float), 1, _File);
	fread(&m_Frequency, sizeof(float), 1, _File);
	fread(&m_MaxAngle, sizeof(float), 1, _File);
	fread(&m_PendulumLength, sizeof(float), 1, _File);
}

