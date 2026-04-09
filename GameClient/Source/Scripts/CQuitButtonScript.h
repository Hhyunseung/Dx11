#pragma once
#include "CUIButtonScript.h"

class CQuitButtonScript :
    public CUIButtonScript
{
public:
	virtual void OnButtonClick() override;

	CLONE(CQuitButtonScript);

public:
	CQuitButtonScript();
	virtual ~CQuitButtonScript();
};

