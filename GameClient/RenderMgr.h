#pragma once

#include "GameObject.h"
#include "CCamera.h"

class RenderMgr
	: public singleton<RenderMgr>
{
	SINGLE(RenderMgr)

private:
	Ptr<CCamera>		m_MainCam;

	/// Level 안에 있는 GameObject 가 아님
	/// 렌더 매니저 전용으로 들고 있는 (오로지 디버그 렌더링 용도의) 게임 오브젝트
	Ptr<GameObject>		m_DbgObj; // 디버그 렌더링을 위한 GameObject
	list<DbgInfo>		m_DbgInfoList; // 디버그 렌더링 요청 정보 리스트

public:
	void RegisterCamera(Ptr<CCamera> _Cam) { m_MainCam = _Cam; }
	Ptr<CCamera> GetMainCam() { return m_MainCam; }
	void AddDebugInfo(const DbgInfo& _Info) { m_DbgInfoList.push_back(_Info); }

public:
	void Render_Debug();

public:
	void Init();
	void Progress();
};

