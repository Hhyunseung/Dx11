#pragma once
#include "CScript.h"
class CPlanet :
    public CScript
{
private:
    float m_Radius;
    float m_RatationSpeed;
    bool  m_IsSun;

public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    void SetIsSun(bool _IsSun) { m_IsSun = _IsSun; }

public:
    CPlanet();
    CPlanet(float _Radius, float _Speed, bool _IsSun);
    virtual ~CPlanet();
};

