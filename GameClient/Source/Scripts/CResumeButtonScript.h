#pragma once
#include "CUIButtonScript.h"

class CResumeButtonScript :
    public CUIButtonScript
{
public:
	virtual void OnButtonClick() override;

	CLONE(CResumeButtonScript);

public:
	CResumeButtonScript();
	virtual ~CResumeButtonScript();
};

