#include "pch.h"
#include "ObjectPoolMgr.h"

#include "AssetMgr.h"
#include "GamePlayMgr.h"

ObjectPoolMgr::ObjectPoolMgr()
{
}

ObjectPoolMgr::~ObjectPoolMgr()
{
	Clear();
}

void ObjectPoolMgr::Init()
{
	Clear();

	// ============================================
	// 게임에서 사용할 풀들을 여기서 초기화
	// ============================================

	 //젤리 풀 (예시)
	 CreatePool(L"Prefab\\Jelly_Bearice.pref", 50, 9);
	 CreatePool(L"Prefab\\Jelly_BearRainbow.pref", 50, 9);
	 //CreatePool(L"Prefab\\Jelly_Bearpink.pref", 50, 9);
	 //CreatePool(L"Prefab\\Jelly_Bearyellow.pref", 50, 9);

	// 장애물 풀 (예시)
	// CreatePool(L"Prefab\\Obstacle_Box.pref", 30, 0);

	// 이펙트 풀 (예시)
	// CreatePool(L"Prefab\\Effect_JellyCollect.pref", 20, 0);
}

void ObjectPoolMgr::Clear()
{
	for (auto& pair : m_mapPools)
	{
		pair.second->Clear();
		delete pair.second;
	}
	m_mapPools.clear();
}

void ObjectPoolMgr::CreatePool(const wstring& _PrefabKey, int _InitSize, int _LayerIdx)
{
	Ptr<APrefab> pPrefab = FIND(APrefab, _PrefabKey);
	if (pPrefab == nullptr)
	{
		assert(nullptr);
		return;
	}

	CreatePool(pPrefab, _InitSize, _LayerIdx);
}

void ObjectPoolMgr::CreatePool(Ptr<APrefab> _Prefab, int _InitSize, int _LayerIdx)
{
	if (_Prefab == nullptr)
		return;

	wstring key = _Prefab->GetKey();

	// 이미 존재하면 스킵
	if (m_mapPools.find(key) != m_mapPools.end())
		return;

	ObjectPool* pPool = new ObjectPool;
	pPool->Init(_Prefab, _InitSize, _LayerIdx);

	m_mapPools.insert(make_pair(key, pPool));
}

GameObject* ObjectPoolMgr::Get(const wstring& _PrefabKey)
{
	auto iter = m_mapPools.find(_PrefabKey);
	if (iter == m_mapPools.end())
		return nullptr;

	return iter->second->Get();
}

GameObject* ObjectPoolMgr::Get(EObjectID _ObjectID)
{
	wstring key = GamePlayMgr::GetPrefabKey((int)_ObjectID);
	return Get(key);
}

void ObjectPoolMgr::Return(const wstring& _PrefabKey, GameObject* _Object)
{
	auto iter = m_mapPools.find(_PrefabKey);
	if (iter == m_mapPools.end())
		return;

	iter->second->Return(_Object);
}

void ObjectPoolMgr::Return(EObjectID _ObjectID, GameObject* _Object)
{
	wstring key = GamePlayMgr::GetPrefabKey((int)_ObjectID);
	Return(key, _Object);
}

void ObjectPoolMgr::ReturnAll()
{
	for (auto& pair : m_mapPools)
	{
		pair.second->ReturnAll();
	}
}

GameObject* ObjectPoolMgr::SpawnEffect(const wstring& _EffectKey, Vec3 _Pos)
{
	GameObject* pEffect = Get(_EffectKey);
	if (pEffect == nullptr)
		return nullptr;

	pEffect->Transform()->SetRelativePos(_Pos);
	return pEffect;
}

bool ObjectPoolMgr::HasPool(const wstring& _PrefabKey) const
{
	return m_mapPools.find(_PrefabKey) != m_mapPools.end();
}
