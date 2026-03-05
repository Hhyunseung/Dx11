#pragma once
#include "CScript.h"
class CEnemy :
    public CScript
{
private:
	float m_Speed;

public:
    void Tick();

    CLONE(CEnemy);

public:
    CEnemy();
    CEnemy(float Speed);
    virtual ~CEnemy();
};

