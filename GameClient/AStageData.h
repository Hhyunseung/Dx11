#pragma once
#include "Asset.h"

class AStageData :
	public Asset
{
private:
	vector<FSpawnInfo> m_vecSpawnInfo;

public:
	const vector<FSpawnInfo>& GetSpawnInfo() { return m_vecSpawnInfo; }
	void AddSpawnInfo(const FSpawnInfo& _Info) { m_vecSpawnInfo.push_back(_Info); }
	void RemoveSpawnInfo(int _Idx) 
	{ 
		if (_Idx >= 0 && _Idx < (int)m_vecSpawnInfo.size())
			m_vecSpawnInfo.erase(m_vecSpawnInfo.begin() + _Idx); 
	}
	void ClearSpawnInfo() { m_vecSpawnInfo.clear(); }
	int GetSpawnInfoCount() const { return (int)m_vecSpawnInfo.size(); }


public:
	virtual int Save(const wstring& _FilePath);
	virtual int Load(const wstring& _FilePath);
	CLONE(AStageData);

public:
	AStageData();
	virtual ~AStageData();
};

