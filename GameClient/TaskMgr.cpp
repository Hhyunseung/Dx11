#include "pch.h"
#include "TaskMgr.h"

#include "AssetMgr.h"
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
			pCurLevel->SetChanged(); // 레벨 상태 변경

			// 레벨이 Play 상태일때 레벨에 추가된 오브젝트는, 레벨 시작시점때 Begin을 호출받지 못하기 때문에,
			// 레벨에 스폰될 때 Begin 을 호출받는다
			if (LevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
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
				Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
				pCurLevel->SetChanged(); // 레벨 상태 변경
			}
		}
		break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			const wchar_t* pLevelName = (const wchar_t*)m_vecTask[i].Param_0;

			Ptr<ALevel> pLevel = AssetMgr::GetInst()->FindAsset<ALevel>(pLevelName); // 레벨 에셋을 찾아서
			LevelMgr::GetInst()->ChangeLevel(pLevel);
		}
		break;
		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)m_vecTask[i].Param_0;
			LevelMgr::GetInst()->ChangeLevelState(NextState);
		}
		break;
		}
	}

	m_vecTask.clear();
}
