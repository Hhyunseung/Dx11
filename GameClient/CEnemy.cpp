#include "pch.h"
#include "CEnemy.h"

#include "TimeMgr.h"
#include "GameObject.h"

CEnemy::CEnemy()
	: m_Speed(0.5f)
{
}

CEnemy::CEnemy(float Speed)
	: m_Speed(Speed)
{
}

CEnemy::~CEnemy()
{
}


void CEnemy::Tick()
{
	Vec3 Pos = GetOwner()->Transform()->GetRelativePos();

	Pos.x -= m_Speed * DT;

	GetOwner()->Transform()->SetRelativePos(Pos);
}