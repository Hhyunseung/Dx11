#pragma once
#include "CScript.h"
class CCamMoveScript :
    public CScript
{

private:

public:
    virtual void Tick() override;

private:
    void MovePespective();
    void MoveOrthographic();

public:
    CCamMoveScript();
	virtual ~CCamMoveScript();
};

