#pragma once
#include "CScript.h"
class CEnemy :
    public CScript
{
private:
	float m_Speed;

public:
    virtual void Tick() override;

public:
    CEnemy();
    CEnemy(float Speed);
    virtual ~CEnemy();
};

