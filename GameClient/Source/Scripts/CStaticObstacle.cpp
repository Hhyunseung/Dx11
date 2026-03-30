#include "pch.h"
#include "CStaticObstacle.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"

CStaticObstacle::CStaticObstacle()
    : CObstructScript(SCRIPT_TYPE::STATICOBSTACLE)  // 자신의 타입 전달
	, m_Speed(0.f)  // 정적 장애물은 기본적으로 속도가 0
{
}

CStaticObstacle::~CStaticObstacle()
{
}

void CStaticObstacle::Init()
{
	CObstructScript::Init(); // 부모 클래스의 Init() 호출

	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, L"Speed", true, 0.f);
}


void CStaticObstacle::Move()
{
	// 공통 유틸리티: 월드 스크롤 적용
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();
	pos.x -= (scrollSpeed + m_Speed) * DT;
	Transform()->SetRelativePos(pos);
}

void CStaticObstacle::SaveToLevelFile(FILE* _File)
{
    // 추가 데이터 없음
}

void CStaticObstacle::LoadFromLevelFile(FILE* _File)
{
    // 추가 데이터 없음
}
