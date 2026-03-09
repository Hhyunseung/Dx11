#include "pch.h"
#include "CCollider2D.h"

#include "RenderMgr.h"


CCollider2D::CCollider2D()
	: Component(COMPONENT_TYPE::COLLIDER2D)
	, m_Scale(Vec2(1.f, 1.f))
	, m_OverlapCount(0)
{
}

CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: Component(_Origin)
	, m_Offset(_Origin.m_Offset)
	, m_Scale(_Origin.m_Scale)
	, m_OverlapCount(0) /// 복사 생성자는 충돌이 시작된 상태가 아니므로 0으로 초기화
{
	/// 델리게이트는 복사 생성자에서 복사하지 않음
}

CCollider2D::~CCollider2D()
{
}


void CCollider2D::FinalTick()
{
	Matrix matTran = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, 0.f);

	m_matWorld = matScale * matTran;
	m_matWorld *= Transform()->GetWorldMat(); /// 부모 행렬 곱셈

	if (0 < m_OverlapCount)
		DrawDebugRect(m_matWorld, Vec4(1.f, 0.f, 0.f, 1.f), 0.f, false);
	else if (0 == m_OverlapCount)
		DrawDebugRect(m_matWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f, false);
	else
		assert(nullptr);
}


void CCollider2D::AddDynamicBeginOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecBeginDel.push_back(COLLISION_DELEGATE{ _Inst, _MemFunc }); 
}

void CCollider2D::AddDynamicOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecOverDel.push_back(COLLISION_DELEGATE{ _Inst, _MemFunc });
}

void CCollider2D::AddDynamicEndOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecEndDel.push_back(COLLISION_DELEGATE{ _Inst, _MemFunc });
}


float CCollider2D::GetTopY()
{
	float centerY = m_matWorld._42; /// 월드 행렬의 Y축 이동값이 충돌체의 중심 Y좌표가 된다
	float halfH = fabsf(m_matWorld._22) * 0.5f; /// 월드 행렬의 Y축 스케일값이 충돌체의 높이가 된다

	return centerY + halfH;
}

float CCollider2D::GetBottomY()
{
	float centerY = m_matWorld._42; /// 월드 행렬의 Y축 이동값이 충돌체의 중심 Y좌표가 된다
	float halfH = fabsf(m_matWorld._22) * 0.5f; /// 월드 행렬의 Y축 스케일값이 충돌체의 높이가 된다

	return centerY - halfH;
}

// 충돌이 처음 시작되었을 때 처리
void CCollider2D::BeginOverlap(Ptr<CCollider2D> _Other)
{
	++m_OverlapCount;

	for (size_t i = 0; i < m_vecBeginDel.size(); ++i)
	{
		(m_vecBeginDel[i].Inst->*m_vecBeginDel[i].MemFunc)(this, _Other.Get());
	}
}


// 충돌이 진행 중일 때 처리
void CCollider2D::Overlap(Ptr<CCollider2D> _Other)
{
	for (size_t i = 0; i < m_vecOverDel.size(); ++i)
	{
		(m_vecOverDel[i].Inst->*m_vecOverDel[i].MemFunc)(this, _Other.Get());
	}

}

// 충돌이 끝났을 때 처리
void CCollider2D::EndOverlap(Ptr<CCollider2D> _Other)
{
	--m_OverlapCount;

	for (size_t i = 0; i < m_vecEndDel.size(); ++i)
	{
		(m_vecEndDel[i].Inst->*m_vecEndDel[i].MemFunc)(this, _Other.Get());
	}
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Offset, sizeof(Vec2), 1, _File);
	fwrite(&m_Scale, sizeof(Vec2), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Offset, sizeof(Vec2), 1, _File);
	fread(&m_Scale, sizeof(Vec2), 1, _File);
}