#pragma once

#include "assets.h"
#include "PathMgr.h"

class AssetMgr
	: public singleton<AssetMgr>
{
	SINGLE(AssetMgr)
private:
	/// 파생되는 Asset 객체들을 관리할 수 있음
	/// 다운 캐스팅을해서 각 Asset 타입별로 반환하여 사용해야함
	///		잘못된 다운캐스팅을 할 수 있으므로 분류해서 관리
	map<wstring, Ptr<Asset>>	m_mapAsset[(UINT)ASSET_TYPE::END];
	bool 						m_Changed;

public:
	void Init();
	bool IsChanged()
	{
		bool Changed = m_Changed;
		m_Changed = false;
		return Changed;
	}

private:
	void CreateEngineMesh();
	void CreateEngineShader();
	void CreateEngineTexture();
	void CreateEngineMaterial();
	void CreateEngineSprite();

public:
	void AddAsset(const wstring& _Key, Ptr<Asset> _Asset);

	void GetAssetNames(ASSET_TYPE _type, vector<wstring>& _vec);

	Ptr<Asset> FindAsset(const wstring& _Key, ASSET_TYPE _Type);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _key);

	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);
};

template<typename T1, typename T2>
constexpr bool IsSameType = false;
template<typename T1>
constexpr bool IsSameType<T1, T1> = true;

template<typename T>
ASSET_TYPE GetAssetType()
{
	/// 템플릿도 런타임이 아닌 컴파일 타임에 결정되는 녀석
	if constexpr (IsSameType<T, AMesh>) /// 컴파일 타임에 true or false 로 바꿔버림
		return ASSET_TYPE::MESH;
	else if constexpr (IsSameType<T, AGraphicShader>)
		return ASSET_TYPE::GRAPHICSHADER;
	else if constexpr (IsSameType<T, ATexture>)
		return ASSET_TYPE::TEXTURE;
	else if constexpr (IsSameType<T, AMaterial>)
		return ASSET_TYPE::MATERIAL;
	else if constexpr (IsSameType<T, ASprite>)
		return ASSET_TYPE::SPRITE;
	else if constexpr (IsSameType<T, AFlipbook>)
		return ASSET_TYPE::FLIPBOOK;
	else if constexpr (IsSameType<T, ATileMap>)
		return ASSET_TYPE::TILEMAP;
	else if constexpr (IsSameType<T, ALevel>)
		return ASSET_TYPE::LEVEL;

	//if (info.hash_code() == typeid(AMesh).hash_code())
	//	Type = ASSET_TYPE::MESH;
	//else if (info.hash_code() == typeid(AGraphicShader).hash_code())
	//	Type = ASSET_TYPE::GRAPHICSHADER;
	//else if (info.hash_code() == typeid(ATexture).hash_code())
	//	Type = ASSET_TYPE::TEXTURE;
	//else if (info.hash_code() == typeid(AMaterial).hash_code())
	//	Type = ASSET_TYPE::MATERIAL;

	return ASSET_TYPE::END;
}

template<typename T>
Ptr<T> AssetMgr::FindAsset(const wstring& _key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<Asset>>::iterator iter = m_mapAsset[(UINT)Type].find(_key);

	if (iter == m_mapAsset[(UINT)Type].end())
		return nullptr;

	// Ptr<Asset> 이므로 다운 캐스팅해서 해당 Type 의 포인터 반환
	return (T*)iter->second.Get();
}

template<typename T>
inline Ptr<T> AssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	// 동일키로 먼저 등록된 에셋이 있는지 확인
	Ptr<T> pAsset = FindAsset<T>(_Key);
	
	// 동일키로 먼저 등록된 에셋이 있으면, 그걸 반환
	if (nullptr != pAsset)
		return pAsset;

	// 에셋 객체 생성
	pAsset = new T;
	
	// 입력된 경로로부터 에셋 로딩작업 진행
	pAsset->Load(CONTENT_PATH + _RelativePath);

	// T 타입에 해당하는 실제 AssetType 확인
	ASSET_TYPE type = GetAssetType<T>();

	// 맵에 에셋 등록
	m_mapAsset[(UINT)type].insert(make_pair(_Key, pAsset.Get()));

	// 에셋이 자신이 매니저에 등록할 때 사용된 Key 와
	// 자신이 어떤 경로에 있는 파일로부터 로딩된 에셋인지 스스로 알 수 있도록 해줌
	pAsset->SetKey(_Key);
	pAsset->SetRelativePath(_RelativePath);

	m_Changed = true;

	return pAsset;
}


template<typename T>
Ptr<T> LoadAssetRef(FILE* _File)
{
	// Asset 이 Null 인지 아닌지 저장
	bool IsNull = false;
	fread(&IsNull, sizeof(bool), 1, _File);

	// Asset 의 Key, RelativePath 저장
	if (IsNull)
	{
		wstring Key = LoadWString(_File);
		wstring RelativePath = LoadWString(_File);
		return AssetMgr::GetInst()->Load<T>(Key, RelativePath);
	}

	return nullptr;
}


#define FIND(Type, Key) AssetMgr::GetInst()->FindAsset<Type>(Key)
#define LOAD(Type, AssetPath) AssetMgr::GetInst()->Load<Type>(AssetPath, AssetPath) 