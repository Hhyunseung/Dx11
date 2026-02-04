#pragma once

#include "assets.h"


class AssetMgr
	: public singleton<AssetMgr>
{
	SINGLE(AssetMgr)
private:
	/// 파생되는 Asset 객체들을 관리할 수 있음
	/// 다운 캐스팅을해서 각 Asset 타입별로 반환하여 사용해야함
	///		잘못된 다운캐스팅을 할 수 있으므로 분류해서 관리
	map<wstring, Ptr<Asset>>	m_mapAsset[(UINT)ASSET_TYPE::END];

public:
	void Init();

private:
	void CreateEngineMesh();
	void CreateEngineShader();
	void CreateEngineTexture();
	void CreateEngineMaterial();

public:
	void AddAsset(const wstring& _Key, Ptr<Asset> _Asset);

	Ptr<Asset> FindAsset(const wstring& _Key, ASSET_TYPE _Type);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _key);
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


#define FIND(Type, Key) AssetMgr::GetInst()->FindAsset<Type>(Key)