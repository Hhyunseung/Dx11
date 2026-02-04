#include "pch.h"
#include "CPlanet.h"

#include "TimeMgr.h"
#include "CCamera.h"
#include "RenderMgr.h"

#include "GameObject.h"

CPlanet::CPlanet()
	: m_Radius(100.f)
	, m_RatationSpeed(0.5f)
	, m_IsSun(false)
{
}

CPlanet::CPlanet(float _Radius, float _Speed, bool _IsSun = false)
	: m_Radius(_Radius)
	, m_RatationSpeed(_Speed)
	, m_IsSun(_IsSun)
{
}

CPlanet::~CPlanet()
{
}

void CPlanet::Begin()
{
	GetOwner()->Transform()->SetRelativePos(Vec3(m_Radius, 0.f, 0.f));
}

void CPlanet::Tick()
{
	// °øÀü
	Vec3 PlanetRot = GetOwner()->Transform()->GetRelativeRot();
	PlanetRot.y += m_RatationSpeed * DT;
	GetOwner()->Transform()->SetRelativeRot(PlanetRot);

	Vec3 PlanetPos = m_Radius * GetOwner()->Transform()->GetDir(DIR::RIGHT);
	GetOwner()->Transform()->SetRelativePos(PlanetPos);
}