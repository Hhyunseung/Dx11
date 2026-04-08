#pragma once
#include "CUIButtonScript.h"

class CSceneChangeButtonScript :
    public CUIButtonScript
{
private:
    SCENE_FLOW_ACTION m_Action;

public:
	void SetAction(SCENE_FLOW_ACTION _Action) { m_Action = _Action; }  
	SCENE_FLOW_ACTION GetAction() const { return m_Action; }

public:
	virtual void Init() override;

	virtual void OnButtonClick() override;

public:
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CSceneChangeButtonScript);

public:
	CSceneChangeButtonScript();
	virtual ~CSceneChangeButtonScript();
};

