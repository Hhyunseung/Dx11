#include "pch.h"
#include "TaskMgr.h"

#include "AssetMgr.h"
#include "LevelMgr.h"
#include "GameObject.h"
#include "SceneFlowMgr.h"

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

	// 현재 프레임에 처리할 Task만 복사
	vector<TaskInfo> vecTask = m_vecTask;
	m_vecTask.clear();

	bool bLevelRelatedTaskProcessed = false;

	// 1차 : 레벨 관련 Task 우선 처리
	for (size_t i = 0; i < vecTask.size(); ++i)
	{
		switch (vecTask[i].Type)
		{
		case TASK_TYPE::CHANGE_LEVEL:
		{
			const wchar_t* pLevelName = (const wchar_t*)vecTask[i].Param_0;
			if (nullptr == pLevelName)
				continue;

			Ptr<ALevel> pLevel = AssetMgr::GetInst()->FindAsset<ALevel>(pLevelName);
			if (nullptr == pLevel)
				continue;

			LevelMgr::GetInst()->ChangeLevel(pLevel);
			bLevelRelatedTaskProcessed = true;
		}
		break;

		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)vecTask[i].Param_0;
			LevelMgr::GetInst()->ChangeLevelState(NextState);
			bLevelRelatedTaskProcessed = true;
		}
		break;
		}
	}

	// 같은 프레임에 레벨 관련 작업이 있었다면
	// 일반 생성/삭제는 건너뜀
	if (bLevelRelatedTaskProcessed)
	{
		SceneFlowMgr::GetInst()->OnLevelEntered();
		return;
	}

	// 2차 : 일반 Task 처리
	for (size_t i = 0; i < vecTask.size(); ++i)
	{
		switch (vecTask[i].Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			Ptr<GameObject> pNewObj = (GameObject*)vecTask[i].Param_0;
			if (nullptr == pNewObj)
				continue;

			Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
			if (nullptr == pCurLevel)
				continue;

			pCurLevel->AddObject((int)vecTask[i].Param_1, pNewObj);
			pCurLevel->SetChanged();

			if (LevelMgr::GetInst()->GetLevelState() == LEVEL_STATE::PLAY)
				pNewObj->Begin();
		}
		break;

		case TASK_TYPE::DESTROY_OBJECT:
		{
			Ptr<GameObject> pObj = (GameObject*)vecTask[i].Param_0;
			if (nullptr == pObj)
				continue;

			if (pObj->m_Dead == false)
			{
				pObj->m_Dead = true;
				m_Garbage.push_back(pObj);

				Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
				if (nullptr != pCurLevel)
					pCurLevel->SetChanged();
			}
		}
		break;
		}
	}
}