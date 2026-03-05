#pragma once
#include "CScript.h"

class CLycheeScript :
    public CScript
{
public:
	void Tick();

	CLONE(CLycheeScript);

public:
	CLycheeScript();
	CLycheeScript(CScript* _Owner);
	virtual ~CLycheeScript();
};

