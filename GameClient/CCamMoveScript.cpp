#include "pch.h"
#include "CCamMoveScript.h"

#include "KeyMgr.h"
#include "CTransform.h"
#include "TimeMgr.h"

CCamMoveScript::CCamMoveScript()
{
}

CCamMoveScript::~CCamMoveScript()
{
}


void CCamMoveScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetDir(DIR::RIGHT);

	if (1 == KeyMgr::GetInst()->GetMouseWheel())
		vPos += vFront * 10.f;
	if (-1 == KeyMgr::GetInst()->GetMouseWheel())
		vPos -= vFront * 10.f;

	if (KEY_PRESSED(KEY::W))
		vPos += 500.f * vFront * DT;
	if (KEY_PRESSED(KEY::S))
		vPos -= 500.f * vFront * DT;
	if (KEY_PRESSED(KEY::A))
		vPos -= 500.f * vRight * DT;
	if (KEY_PRESSED(KEY::D))
		vPos += 500.f * vRight * DT;

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = KeyMgr::GetInst()->GetMouseDir();

		vRot.y += vMouseDir.x * XM_2PI * DT;
		vRot.x += vMouseDir.y * XM_2PI * DT;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}
