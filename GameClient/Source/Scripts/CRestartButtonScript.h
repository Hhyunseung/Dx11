#pragma once
#include "CUIButtonScript.h"
class CRestartButtonScript :
    public CUIButtonScript
{
public:
	virtual void OnButtonClick() override;

	CLONE(CRestartButtonScript);

public:
	CRestartButtonScript();
	virtual ~CRestartButtonScript();
};

