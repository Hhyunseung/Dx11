#include "pch.h"
#include "CTransform.h"

#include "Device.h"
#include "GameObject.h"


CTransform::CTransform()
	: Component(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_Dir{}
	, m_IndependentScale(false)
{
	/// 회전하지 않은 상태(초기 상태)의 단위 벡터
	m_Dir[(UINT)DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_Dir[(UINT)DIR::UP]	= Vec3(0.f, 1.f, 0.f);
	m_Dir[(UINT)DIR::FRONT] = Vec3(0.f, 0.f, 1.f);
}

CTransform::~CTransform()
{
}


Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_RelativeScale;
	
	if (m_IndependentScale)
		return vWorldScale;

	Ptr<GameObject> pParent = GetOwner()->GetParent();

	// 부모의 크기 정보들을 곱한다
	while (pParent != nullptr)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		if (pParent->Transform()->m_IndependentScale)
			break;

		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

void CTransform::FinalTick()
{	
	// 크기 -> 회전 -> 이동 순으로 
	Matrix matTrans = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z); // 단위행렬로 초기화
	// Matrix matTrans = XMMatrixIdentity(); // 단위행렬로 초기화
	// matTrans._41 = m_RelativePos.x;
	// matTrans._42 = m_RelativePos.y;
	// matTrans._43 = m_RelativePos.z;

	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);
	//Matrix matScale = XMMatrixIdentity();
	//matScale._11 = m_RelativeScale.x;
	//matScale._22 = m_RelativeScale.y;
	//matScale._33 = m_RelativeScale.z;
	
	// 회전 행렬
	Matrix matRot = XMMatrixRotationX(m_RelativeRot.x) 
				  * XMMatrixRotationY(m_RelativeRot.y) 
				  * XMMatrixRotationZ(m_RelativeRot.z);
	
	// 방향벡터 계산
	m_Dir[(UINT)DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_Dir[(UINT)DIR::UP]	= Vec3(0.f, 1.f, 0.f);
	m_Dir[(UINT)DIR::FRONT] = Vec3(0.f, 0.f, 1.f);
	
	/// 방향벡터는 회전만 적용을 받아야함
	/// 만약 월드행렬만 가져올 수 있을때에는... 동차좌표가 0이 들어가주어야함 그래야 이동정보가 곱해지지 않음
	
	// 변환행렬을 적용할 Vec3 벡터를 좌표성 데이터로 본다(동차좌표 1로 확장 -> 4행 이동정보 적용)
	// XMVector3TransformCoord(m_Dir[(UINT)DIR::RIGHT], matRot);

	// 변환행렬을 적용할 Vec3 벡터를 방향성 데이터로 본다(동차좌표 0로 확장 -> 4행 이동정보 무시)
	m_Dir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(m_Dir[(UINT)DIR::RIGHT], matRot);
	m_Dir[(UINT)DIR::UP]	= XMVector3TransformNormal(m_Dir[(UINT)DIR::UP], matRot);
	m_Dir[(UINT)DIR::FRONT] = XMVector3TransformNormal(m_Dir[(UINT)DIR::FRONT], matRot);

	m_matWorld = matScale * matRot * matTrans; // 크기 -> 회전 -> 이동 순으로 곱함


	// 부모 오브젝트가 있었다면
	if (nullptr != GetOwner()->GetParent())
	{
		// 부모 오브젝트의 크기에 영향을 받지 않겠다
		if (m_IndependentScale)
		{
			Vec3 ParentScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
			
			// 부모의 크기 행렬
			Matrix matParentScale = XMMatrixScaling(ParentScale.x, ParentScale.y, ParentScale.z);

			// 역행렬
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// 부모의 크기 영향을 제거한 후에 부모의 월드 행렬을 곱해준다
			m_matWorld = m_matWorld * matParentScaleInv * GetOwner()->GetParent()->Transform()->GetWorldMat();
		}

		// 부모 오브젝트의 크기에 영향을 받는다
		else
		{
			m_matWorld *= GetOwner()->GetParent()->Transform()->GetWorldMat();
		}
	}
}

void CTransform::Binding()
{
	g_Trans.matWorld = m_matWorld;

	// 전역변수에 들어있는 오브젝트 위치 정보를 상수버퍼로 복사
	Device::GetInst()->GetCB(CB_TYPE::TRANSFORM)->SetData(&g_Trans);
	Device::GetInst()->GetCB(CB_TYPE::TRANSFORM)->Binding();
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _File)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_RelativeRot, sizeof(Vec3), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
}
