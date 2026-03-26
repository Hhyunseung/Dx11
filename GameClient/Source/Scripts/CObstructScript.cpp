#include "pch.h"
#include "CObstructScript.h"

#include "TimeMgr.h"
#include "GamePlayMgr.h"
#include "GameObject.h"

CObstructScript::CObstructScript(SCRIPT_TYPE _Type)
	: CScript(_Type)
{
}

CObstructScript::CObstructScript()
	: CScript(SCRIPT_TYPE::OBSTRUCTSCRIPT)
{
}

CObstructScript::~CObstructScript()
{
}

void CObstructScript::OnSpawn()
{
	// 기본 구현: 자식에서 오버라이드 가능
}

void CObstructScript::Move()
{
	// 기본 구현: 월드 스크롤만 적용
	ApplyWorldScroll();
}

void CObstructScript::ApplyWorldScroll()
{
	// 공통 유틸리티: 월드 스크롤 적용
	float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	Vec3 pos = Transform()->GetRelativePos();
	pos.x -= scrollSpeed * DT;
	Transform()->SetRelativePos(pos);
}

void CObstructScript::Tick()
{
	// 자식의 Move() 호출
	Move();
}

void CObstructScript::SaveToLevelFile(FILE* _File)
{
	// 기본 클래스는 저장할 데이터 없음
	// 자식에서 오버라이드
}

void CObstructScript::LoadFromLevelFile(FILE* _File)
{
	// 기본 클래스는 로드할 데이터 없음
	// 자식에서 오버라이드
}


