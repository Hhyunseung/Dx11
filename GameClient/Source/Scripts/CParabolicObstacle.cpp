#include "pch.h"
#include "CParabolicObstacle.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"

CParabolicObstacle::CParabolicObstacle()
    : CObstructScript(SCRIPT_TYPE::PARABOLICOBSTACLE) 
	, m_IsGrounded(false)
	, m_CurrentVelocityY(300.f)
	, m_Gravity(980.f) 
	, m_StopOnGround(true)
	, m_GroundY(0.f)
{
}

CParabolicObstacle::~CParabolicObstacle()
{
}


void CParabolicObstacle::Move()
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

void CParabolicObstacle::SaveToLevelFile(FILE* _File)
{
}

void CParabolicObstacle::LoadFromLevelFile(FILE* _File)
{
}
