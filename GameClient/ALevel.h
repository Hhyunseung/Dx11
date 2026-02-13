#pragma once
#include "Asset.h"

#include "Layer.h"

// 게임 오브젝트들을 분류할건데 그걸 레이어별로 나눠서 분류
class ALevel
	: public Asset
{
private:
	Layer		m_arrLayer[MAX_LAYER]; // 하나의 레벨안에 총 32개의 레이어가 존재 

	/// 가로세로 32(UINT 비트수) x 32(레이어 수) 행렬
    /// -> 같은 열, 같은 행 충돌이면 같은 레이어 끼리의 충돌... 개념
	/// 더 작은 숫자가 행이고 더 큰 숫자가 열
	UINT        m_Matrix[MAX_LAYER];   // 어떤 레이어와, 어떤 레이어가 충돌검사를 진행할지 마킹한 데이터

public:
	void AddObject(int _LayerLevel, Ptr<GameObject> _Obj);
	Layer* GetLayer(int _Idx) { assert(0 <= _Idx && _Idx < MAX_LAYER); return &m_arrLayer[_Idx]; }
	
	void Deregister();
	void CheckCollisionLayer(UINT _LayerIdx1, UINT _LayerIdx2);
	void CheckCollisionLayer(const wstring& _LayerName1, const wstring& _LayerName2);
	
	UINT* GetCollisionMatrix() { return m_Matrix; }

	Ptr<GameObject> FindObjectByName(const wstring& _Name);

public:
	void Begin();
	void Tick();
	void FinalTick();
	
public:
	ALevel();
	virtual ~ALevel();

};

