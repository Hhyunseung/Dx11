#pragma once
#include <queue>

class ObjectPool
{
private:
	Ptr<APrefab>				m_Prefab;
	vector<Ptr<GameObject>>		m_vecPool;
	std::queue<int>				m_queAvailable;		// 사용 가능한 인덱스 (비활성화 되어있는 vecPool의 인덱스)
	int							m_LayerIdx;

public:
	void Init(Ptr<APrefab> _Prefab, int _InitSize, int _LayerIdx);
	GameObject* Get();
	void Return(GameObject* _Object);
	void ReturnAll();
	void Clear();

	int GetActiveCount() const { return (int)m_vecPool.size() - (int)m_queAvailable.size(); }
	int GetTotalCount() const { return (int)m_vecPool.size(); }

private:
	GameObject* CreateNewObject();
};
