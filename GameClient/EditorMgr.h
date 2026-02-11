#pragma once

#include "EditorUI.h"

class EditorMgr
	: public singleton<EditorMgr>
{
	SINGLE(EditorMgr);

private:
	map<string, Ptr<EditorUI>> m_mapUI;
	bool					   m_ShowDemo;

public:
	bool IsShowDemo() { return m_ShowDemo; }
	void ShowDemo(bool _Show) { m_ShowDemo = _Show; }

	void AddUI(const string& _UIName, Ptr<EditorUI> _UI);
	Ptr<EditorUI> FindUI(const string& _UIName);

public:
	void Init();
	void Progress();

private:
	void Tick();
	void Render();

private:
	void CreateEditorUI();
};

