#include "pch.h"
#include "CParabolicObstacle.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"

CParabolicObstacle::CParabolicObstacle()
    : CObstructScript(SCRIPT_TYPE::PARABOLICOBSTACLE) 
	, m_IsGrounded(false)
	, m_InitialVelocityY(300.f)
	, m_CurrentVelocityY(300.f)
	, m_Gravity(980.f) 
	, m_Speed(0.f)
	, m_StopOnGround(true)
	, m_GroundY(-170.f)
{
}

CParabolicObstacle::~CParabolicObstacle()
{
}

void CParabolicObstacle::Init()
{
	CObstructScript::Init(); // 부모 클래스의 Init() 호출

	AddScriptParam(SCRIPT_PARAM::BOOL, &m_StopOnGround, L"StopGround", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitialVelocityY, L"CurrentVelocityY", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Gravity, L"Gravity", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_GroundY, L"GroundY", true, 0.f);
}


void CParabolicObstacle::OnSpawn()
{
	CObstructScript::OnSpawn();

	m_IsGrounded = false;
	m_CurrentVelocityY = m_InitialVelocityY;
}

void CParabolicObstacle::Move()
{
	// 월드 스크롤 + 포물선 이동
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();

	// 월드 스크롤 적용
	pos.x -= (scrollSpeed + m_Speed) * DT;

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

			GetOwner()->FlipbookRender()->Play(1, 8.f, 1); // 바닥에 닿았을 때 다른 애니메이션 재생
		}
	}

	Transform()->SetRelativePos(pos);
}


void CParabolicObstacle::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_StopOnGround, sizeof(bool), 1, _File);
	fwrite(&m_InitialVelocityY, sizeof(float), 1, _File);
	fwrite(&m_Gravity, sizeof(float), 1, _File);
	fwrite(&m_GroundY, sizeof(bool), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CParabolicObstacle::LoadFromLevelFile(FILE* _File)
{
	fread(&m_StopOnGround, sizeof(bool), 1, _File);
	fread(&m_InitialVelocityY, sizeof(float), 1, _File);
	fread(&m_Gravity, sizeof(float), 1, _File);
	fread(&m_GroundY, sizeof(bool), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
}
