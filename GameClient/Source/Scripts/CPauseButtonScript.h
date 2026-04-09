#pragma once
#include "CUIButtonScript.h"
class CPauseButtonScript :
    public CUIButtonScript
{
public:
	virtual void OnButtonClick() override;

	CLONE(CPauseButtonScript);

public:
	CPauseButtonScript();
	virtual ~CPauseButtonScript();
};

