#include "pch.h"
#include "CollisionMgr.h"

#include "AssetMgr.h"

CollisionMgr::CollisionMgr()
{

}

CollisionMgr::~CollisionMgr()
{

}


void CollisionMgr::Progress(Ptr<ALevel> _Level)
{
	UINT* pMatrix = _Level->GetCollisionMatrix();

	for (UINT Row = 0; Row < MAX_LAYER; ++Row)
	{
		for (UINT Col = Row; Col < MAX_LAYER; ++Col)
		{
			if (false == (pMatrix[Row] & (1 << Col)))
				continue;

			/// 충돌 진행
			CollisionBtwLayer(_Level->GetLayer(Row), _Level->GetLayer(Col));
		}
	}

}

void CollisionMgr::CollisionBtwLayer(Layer* _Left, Layer* _Right)
{
	/// 받은 레퍼런스를 복사하게 되어 복사 비용 발생 
	// vector<Ptr<GameObject>> vecLeft = _Left->GetAllObjects();
	
	/// 원본 유지 // 수정하지 않기 위해 const 
	const vector<Ptr<GameObject>>& vecLeft = _Left->GetAllObjects();
	const vector<Ptr<GameObject>>& vecRight = _Right->GetAllObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->Collider2D())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->Collider2D())
				continue;

			// 두 충돌체의 고유 ID 로 조합을 한 키값 생성
			COL_ID colID;
			colID.LeftID = vecLeft[i]->Collider2D()->GetID();
			colID.RightID = vecRight[j]->Collider2D()->GetID();

			map<ULONGLONG, bool>::iterator iter = m_mapColID.find(colID.ID);

			if (iter == m_mapColID.end())
			{
				m_mapColID.insert(make_pair(colID.ID, false));
				iter = m_mapColID.find(colID.ID);
			}


			vecLeft[i]->Collider2D()->GetID();
			vecRight[j]->Collider2D()->GetID();

			if (IsCollision(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				if (iter->second)
				{
					vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
				}

				else
				{
					vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
				}

				iter->second = true;
			}
			else
			{
				if (iter->second)
				{
					vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
				}

				iter->second = false;
			}
		}
	}
}

bool CollisionMgr::IsCollision(Ptr<CCollider2D> _LeftCol, Ptr<CCollider2D> _RightCol)
{
	/// 표면벡터 하나하나를 기준으로 전부 다 투영
	/// 하나만 실패해도 충돌하지 않는 것
	/// 각 중점사이의 거리보다 투영된 길이의 합 / 2 가 더 길면 

	Ptr<AMesh> pRectMesh = FIND(AMesh, L"RectMesh");

	const Vtx* pVtx = pRectMesh->GetVtxSysMem();
	
	const Matrix& matWorldLeft = _LeftCol->GetWorldMat();
	const Matrix& matWorldRight = _RightCol->GetWorldMat();

	/// XMVector2TransformCoord 동차좌표 1로 (이동시켜야하니까)
	/// 정점을 행렬연산해서 이동시킨다음 물체의 표면 벡터들 구함
	
	// 월드 공간상에서 충돌을 검사하기 위해서, RectMesh 모델을 각 충돌체의 월드행렬을 곱해서 정점을 충돌체 꼭지점에 배치시킨다
	// 각 꼭지점끼리 빼서 두 충돌체의 표면 방향벡터를 각 충돌체로부터 2개씩 구한다
	Vec3 Axis[4] = {};
	Axis[0] = XMVector2TransformCoord(pVtx[1].vPos, matWorldLeft) - XMVector2TransformCoord(pVtx[0].vPos, matWorldLeft);
	Axis[1] = XMVector2TransformCoord(pVtx[3].vPos, matWorldLeft) - XMVector2TransformCoord(pVtx[0].vPos, matWorldLeft);
	Axis[2] = XMVector2TransformCoord(pVtx[1].vPos, matWorldRight) - XMVector2TransformCoord(pVtx[0].vPos, matWorldRight);
	Axis[3] = XMVector2TransformCoord(pVtx[3].vPos, matWorldRight) - XMVector2TransformCoord(pVtx[0].vPos, matWorldRight);


	/// 원점을 이동행렬 곱해서 크기.회전 무시하고 중심점 구함
	Vec3 vCenter = XMVector2TransformCoord(Vec3(0.f, 0.f, 0.f), matWorldRight) - XMVector2TransformCoord(Vec3(0.f, 0.f, 0.f), matWorldLeft);


	/// 각 벡터에 대해 투영 
	for (int i = 0; i < 4; i++)
	{
		// 4 개의 축 중에서, 하나를 투영 목적지로 정함
		// 원본값을 훼손하면 나중에 투영할 때 문제가 생기기 때문에, 정규화한 벡터를 따로 지역변수로 둠
		Vec3 vProjAxis = Axis[i];
		vProjAxis.Normalize();
		
		// 투영축으로 4개의 벡터를 투영시켜서 얻은 면적의 절반 길이를 구함
		float Dot = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			Dot += fabs(vProjAxis.Dot(Axis[j])); /// 내적 값 누적 (절댓값으로 양수*길이*만 나오게)
		}
		Dot /= 2.f;
		
		// 두 충돌체의 중심끼리 이은 벡터도 투영시킴
		float fCenter = fabs(vCenter.Dot(vProjAxis));

		// 중심끼리 이은 벡터의 면적이 더 크다면, 두 충돌체를 나눌 수 있는 분리축이 존재함
		if (fCenter > Dot)
			return false;
	}

	return true;
}
