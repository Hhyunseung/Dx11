#pragma once
#include "ObjectPool.h"

#include "APrefab.h"

class ObjectPoolMgr 
	: public singleton<ObjectPoolMgr>
{
	SINGLE(ObjectPoolMgr);

private:
	map<wstring, ObjectPool*>	m_mapPools;

public:
	void Init();
	void Clear();

	// 풀 생성/관리
	void CreatePool(const wstring& _PrefabKey, int _InitSize, int _LayerIdx);
	void CreatePool(Ptr<APrefab> _Prefab, int _InitSize, int _LayerIdx);

	// 오브젝트 가져오기/반환
	GameObject* Get(const wstring& _PrefabKey);
	GameObject* Get(EObjectID _ObjectID);
	void Return(const wstring& _PrefabKey, GameObject* _Object);
	void Return(EObjectID _ObjectID, GameObject* _Object);
	void ReturnAll();

	// 이펙트 전용 (편의 함수)
	GameObject* SpawnEffect(const wstring& _EffectKey, Vec3 _Pos);

	bool HasPool(const wstring& _PrefabKey) const;
};
