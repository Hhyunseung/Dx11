#include "pch.h"
#include "ALevel.h"


ALevel::ALevel()
	: Asset(ASSET_TYPE::LEVEL)
	, m_Matrix{}
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_LayerIdx = i;
	}
}

ALevel::~ALevel()
{
}

void ALevel::AddObject(int _LayerIdx, Ptr<GameObject> _Obj)
{
	m_arrLayer[_LayerIdx].AddObject(_Obj);
}

void ALevel::Deregister()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].DeregisterObject();
	}
}

void ALevel::CheckCollisionLayer(UINT _LayerIdx1, UINT _LayerIdx2)
{
	UINT Row = _LayerIdx1;
	UINT Col = _LayerIdx2;

	// 더 작은 인덱스를 행 으로 사용한다
	if (_LayerIdx2 < _LayerIdx1)
	{
		Row = _LayerIdx2;
		Col = _LayerIdx1;
	}

	// 비트 값 체크
	m_Matrix[Row] ^= (1 << Col);
}

void ALevel::CheckCollisionLayer(const wstring& _LayerName1, const wstring& _LayerName2)
{

}


void ALevel::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Begin();
	}
}

void ALevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].Tick();
	}
}

void ALevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].FinalTick();
	}
}

Ptr<GameObject> ALevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer 의 최상위 부모 오브젝트들 가져오기 // GetAllObjects 는 Init 이후에 채워져서
		const vector<Ptr<GameObject>>& vecParents = m_arrLayer[i].GetParentObjects();

		// 자식 탐색
		for (size_t i = 0; i < vecParents.size(); ++i)
		{
			list<Ptr<GameObject>> queue;
			queue.push_back(vecParents[i]);

			while (!queue.empty())
			{
				Ptr<GameObject> pObject = queue.front();
				queue.pop_front();

				// 찾았다
				if (pObject->GetName() == _Name)
					return pObject;

				const vector<Ptr<GameObject>>& vecChild = pObject->GetChild();
				for (size_t j = 0; j < vecChild.size(); ++j)
				{
					queue.push_back(vecChild[j]);
				}
			}
		}
	}

	return nullptr;
}

int ALevel::Save(const wstring& _FilePath)
{
	// 파일 스트림 커널
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	// 레벨 이름
	wstring LevelName = GetName();
	SaveWString(pFile, LevelName);

	// 충돌 체크 정보
	fwrite(m_Matrix, sizeof(UINT), MAX_LAYER, pFile);

	// 레이어 정보
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어의 이름 저장
		SaveWString(pFile, m_arrLayer[i].GetName());

		// 레이어 소속 최상위 부모 오브젝트를 계층구조로 저장
		const vector<Ptr<GameObject>>& vecParents = m_arrLayer[i].GetParentObjects();
		
		// 오브젝트 개수
		size_t parentCount = vecParents.size();
		fwrite(&parentCount, sizeof(size_t), 1, pFile);

		for (const auto& Object : vecParents)
		{
			Object->SaveToLevelFile(pFile);
		}
	}

	fclose(pFile);

	return S_OK;
}


int ALevel::Load(const wstring& _FilePath)
{
	// 파일 스트림 커널
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 레벨 이름
	wstring LevelName = LoadWString(pFile);
	SetName(LevelName);

	// 충돌 체크 저장
	fread(m_Matrix, sizeof(UINT), MAX_LAYER, pFile);

	// 레이어 정보
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어의 이름 저장
		wstring LayerName = LoadWString(pFile);
		m_arrLayer[i].SetName(LayerName);

		// 레이어 소속 최상위 부모 오브젝트를 계층구조로 불러온다
		// 오브젝트 개수
		size_t parentCount = 0;
		fread(&parentCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < parentCount; ++j)
		{
			Ptr<GameObject> pObject = new GameObject;
			pObject->LoadFromLevelFile(pFile);
			AddObject(i, pObject);
		}
	}

	fclose(pFile);

	return S_OK;
}