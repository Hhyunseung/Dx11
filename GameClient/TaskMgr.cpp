#include "pch.h"
#include "TaskMgr.h"

#include "LevelMgr.h"

#include "GameObject.h"


TaskMgr::TaskMgr()
{

}

TaskMgr::~TaskMgr()
{

}

void TaskMgr::Progress()
{
	// 쓰레기통 수거
	m_Garbage.clear();
	

	// Task 처리
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		switch (m_vecTask[i].Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			Ptr<GameObject> pNewObj = (GameObject*)m_vecTask[i].Param_0;

			Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(m_vecTask[i].Param_1, pNewObj); // 여기서 Level 이 스마트 포인터로 받아서 참조 카운트 증가
			/// 이 지역함수가 끝나서 pNewObj 의 참조 카운트가 줄어들어도 Level 이 들고 있으니까 문제 없음

			pNewObj->Begin();
		}
			break;
		case TASK_TYPE::DESTROY_OBJECT:
		{
			Ptr<GameObject> pObj = (GameObject*)m_vecTask[i].Param_0;

			/// 한 프레임에 여러번 Destroy 요청이 들어올 수 있으니까 중복 처리 방지
			if (pObj->m_Dead == false)
			{
				pObj->m_Dead = true;
				m_Garbage.push_back(pObj);
			}
		}
			break;
		}
	}

	m_vecTask.clear();
}
