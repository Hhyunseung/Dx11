#include "pch.h"
#include "CStaticObstacle.h"

CStaticObstacle::CStaticObstacle()
    : CObstructScript(SCRIPT_TYPE::STATICOBSTACLE)  // 자신의 타입 전달
{
}

CStaticObstacle::~CStaticObstacle()
{
}

void CStaticObstacle::Move()
{
    // 월드 스크롤만 적용 (부모의 유틸리티 함수 사용)
    ApplyWorldScroll();
}

void CStaticObstacle::SaveToLevelFile(FILE* _File)
{
    // 추가 데이터 없음
}

void CStaticObstacle::LoadFromLevelFile(FILE* _File)
{
    // 추가 데이터 없음
}
