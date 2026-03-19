#include "pch.h"
#include "CRunState.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "GameObject.h"
#include "CTransform.h"


CRunState::CRunState()
	: PlayerState((UINT)SCRIPT_TYPE::RUNSTATE, nullptr, PLAYER_STATE_ID::RUN)
{
}

CRunState::CRunState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::RUNSTATE, _Owner, PLAYER_STATE_ID::RUN)
{
}

CRunState::~CRunState()
{
}

void CRunState::Enter(PLAYER_STATE_ID _prev)
{


	GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::RUN, 8.f, -1);
}

void CRunState::Tick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetOwner()->Transform()->GetRelativeRot();

	Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
	Vec3 vDown = -vUp;



	if (KEY_PRESSED(KEY::UP))
		vPos += vUp * 150.f * DT;
	if (KEY_PRESSED(KEY::DOWN))
		vPos += vDown * 150.f * DT;

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += 150.f * DT;
	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= 150.f * DT;


	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);
	GetOwner()->Transform()->SetRelativeRot(vRotation);
}

void CRunState::Exit(PLAYER_STATE_ID _Next)
{

}
