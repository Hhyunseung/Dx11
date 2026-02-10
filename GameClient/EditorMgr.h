#pragma once


class EditorMgr
	: public singleton<EditorMgr>
{
	SINGLE(EditorMgr);

public:
	void Init();
	void Progress();

private:
	void Tick();
	void Render();
};

