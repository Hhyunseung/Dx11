#pragma once
class PathMgr
	: public singleton<PathMgr>
{
	SINGLE(PathMgr)

private:
	// 윈도우가 255자를 초과하는 경로를 지원하지 않음
	// 애초에 윈도우에서 경로 길이를 제한
	wchar_t		m_ContentPath[255];

public:
	void Init();

public:
	const wchar_t* GetContentPath() { return m_ContentPath; }

};

#define CONTENT_PATH wstring(PathMgr::GetInst()->GetContentPath())