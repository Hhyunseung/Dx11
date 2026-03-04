#include "pch.h"
#include "RunState.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "CTransform.h"


RunState::RunState(CPlayerScript* _Owner)
	: PlayerState(_Owner, PLAYER_STATE_ID::RUN)
{
}

RunState::~RunState()
{
}

void RunState::Enter(PLAYER_STATE_ID _prev)
{
}

void RunState::Tick()
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
		vRotation.z -= XM_PI * DT;
	if (KEY_PRESSED(KEY::LEFT))
		vRotation.z += XM_PI * DT;


	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);
	GetOwner()->Transform()->SetRelativeRot(vRotation);
}

void RunState::Exit(PLAYER_STATE_ID _Next)
{
}
