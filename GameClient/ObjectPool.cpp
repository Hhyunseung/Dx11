#include "pch.h"
#include "ObjectPool.h"

#include "APrefab.h"
#include "LevelMgr.h"

void ObjectPool::Init(Ptr<APrefab> _Prefab, int _InitSize, int _LayerIdx)
{
	m_Prefab = _Prefab;
	m_LayerIdx = _LayerIdx;

	// 초기 오브젝트 생성
	for (int i = 0; i < _InitSize; ++i)
	{
		GameObject* pObject = CreateNewObject();
		// 처음에는 모두 비활성화 상태로 시작
		pObject->SetActive(false);
		m_queAvailable.push(i);
	}
}

GameObject* ObjectPool::Get()
{
	GameObject* pObject = nullptr;

	if (m_queAvailable.empty())
	{
		// 풀이 부족하면 새로 생성
		int idx = (int)m_vecPool.size();
		pObject = CreateNewObject();
		// 새로 생성된 것은 바로 사용하므로 큐에 넣지 않음
	}
	else
	{
		int idx = m_queAvailable.front();
		m_queAvailable.pop();
		pObject = m_vecPool[idx].Get();
	}

	pObject->SetActive(true);
	return pObject;
}

void ObjectPool::Return(GameObject* _Object)
{
	if (_Object == nullptr)
		return;

	_Object->SetActive(false);

	// 인덱스 찾기
	for (int i = 0; i < m_vecPool.size(); ++i)
	{
		if (m_vecPool[i].Get() == _Object)
		{
			m_queAvailable.push(i);
			break;
		}
	}
}

void ObjectPool::ReturnAll()
{
	// 큐 비우기
	while (!m_queAvailable.empty())
		m_queAvailable.pop();

	// 모든 오브젝트 비활성화 및 큐에 추가
	for (int i = 0; i < m_vecPool.size(); ++i)
	{
		m_vecPool[i]->SetActive(false);
		m_queAvailable.push(i);
	}
}

void ObjectPool::Clear()
{
	m_vecPool.clear();
	while (!m_queAvailable.empty())
		m_queAvailable.pop();
}

GameObject* ObjectPool::CreateNewObject()
{
	GameObject* pObject = m_Prefab->Instantiate();
	m_vecPool.push_back(pObject);

	// 레벨에 추가 (비활성 상태로)
	CreateObject(pObject, m_LayerIdx);

	return pObject;
}
