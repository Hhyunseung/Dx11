#include "pch.h"
#include "CCamera.h"

#include "LevelMgr.h"
#include "ALevel.h"
#include "Layer.h"
#include "RenderMgr.h"

#include "GameObject.h"
#include "CTransform.h"
#include "Device.h"

CCamera::CCamera()
	: Component(COMPONENT_TYPE::CAMERA)
	, m_LayerCheck(0)
	, m_OrthoScale(1.f)
{
	// 아직 
}

CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	/// AddComponent 로 들어간 상태여서 다른 게임 오브젝트로 접근 가능
	/// 렌더매니저에 카메라를 등록

	// 레벨이 시작될 때 호출됨
	// RenderMgr 에 카메라(본인)를 등록

	RenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::FinalTick()
{
	// 뷰(View) 행렬 계산
	/// 오브젝트(나)를 찍고있는 카메라를 기준인 좌표계로 오브젝트가 이동 (월드 -> 뷰)
	/// 카메라가 원점으로부터 이동한 거리만큼 오브젝트도 이동시켜주면(상대거리) NDC 좌표계에서 

	// 카메라의 위치
	Vec3 vPos = Transform()->GetRelativePos();

	// 이동 (카메라 위치를 원점으로 되돌리는 만큼의 이동행렬)
	Matrix matTrans = XMMatrixIdentity();

	/// 카메라와 찍은 물체의 상대적인 위치를 구하기 위해
	/// 카메라 위치 반대로 빠져야 하기 때문에 음수를 넣음
	matTrans._41 = -vPos.x;
	matTrans._42 = -vPos.y;
	matTrans._43 = -vPos.z;

	// View 행렬 회전
	// - 카메라가 바라보는 방향을 z 축이 되도록 회전하는 부분이 추가
	// 카메라의 Right, Up, Front 방향 벡터에 회전 행렬을 곱하면 이 값이 x축, y축, z축이 되는 회전행렬을 구해야함
	// 

	// vR			  ( 1 0 0 )
	// vU	x	R	= ( 0 1 0 )
	// vF			  ( 0 0 1 )

	//			vR
	// R 행렬은	vU		의 역행렬
	//			vF

	// vR
	// vU	행렬은 행끼리의 관계가 직교상태이기 때문에, 전치(Transpose) 를 통해서 역행렬을 쉽게 구할 수 있음
	// zF

	// 전치한 행렬과 곱해보면, 자기 자신과 내적을 한 경우 결과가 1, 다른 직교벡터랑 내적을 한 경우 0 이 나오기 때문에
	// 내적은 각 성분끼리의 곱을 합친 결과
	// 내적 결과값의 의미는 (벡터의 길이) x (벡터의 길이) x (두 벡터가 이루는 각도의 cos 값)

	//		( vR.x  vU.x  vF.x   0 )
	// R == ( vR.y  vU.y  vF.y   0 )
	//		( vR.z  vU.z  vF.z   0 )
	//		(  0     0     0     1 )
	 
	Vec3 vR = Transform()->GetDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetDir(DIR::UP);
	Vec3 vF = Transform()->GetDir(DIR::FRONT);

	Matrix matRot = XMMatrixIdentity();

	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;


	/// 원점으로 이동한 다음 회전  // 이동 -> 회전
	// 카메라가 원점인 공간으로 이동, 카메라가 바라보는 방향을 z 축으로 회전하는 회전을 적용
	m_matView = matTrans * matRot;
	 
	// 투영 행렬
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교 투영(Projection) 행렬 계산
		/// NDC 좌표계가 정사각형이라 직사각형이 될 수 있기 때문에 종횡비가 필요
		m_matProj = XMMatrixOrthographicLH(m_Width * m_OrthoScale, (m_Width / m_AspectRatio) * m_OrthoScale, 1.f, m_Far);
	}
	else
	{
		// 원근 투영(Perspective) 행렬 계산
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

}


void CCamera::SortObject()
{
	// 렌더링 할 물체들을 정렬한다
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTrapsnarent.clear();
	m_vePostProcess.clear();

	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 카메라가 레이어를 볼 수 있어야 함
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		// 레이어에 소속된 모든 오브젝트를 가져온다
		Layer* pLayer = pCurLevel->GetLayer(i);
		const vector<Ptr<GameObject>>& vecObjects = pLayer->GetAllObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 오브젝트가 렌더링 할 수 있는 상태인지 확인
			 if (nullptr == vecObjects[j]->GetRenderCom()
				|| nullptr == vecObjects[j]->GetRenderCom()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderCom()->GetMaterial())
			{
				continue;
			}

			 RENDER_DOMAIN domain = vecObjects[j]->GetRenderCom()->GetMaterial()->GetDomain();

			 switch (domain)
			 {
			 case RENDER_DOMAIN::DOMAIN_OPAQUE:
				 m_vecOpaque.push_back(vecObjects[j].Get());
				 break;
			 case RENDER_DOMAIN::DOMAIN_MASKED:
				 m_vecMasked.push_back(vecObjects[j].Get());
				 break;
			 case RENDER_DOMAIN::DOMAIN_TRANSPARENT:
				 m_vecTrapsnarent.push_back(vecObjects[j].Get());
				 break;
			 case RENDER_DOMAIN::DOMAIN_POSTPROCESS:
				 m_vePostProcess.push_back(vecObjects[j].Get());
				 break;
			 }
		}
	}
}

void CCamera::Render()
{
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// Domain 순서대로 렌더링 진행
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
		m_vecOpaque[i]->Render();

	for (size_t i = 0; i < m_vecMasked.size(); ++i)
		m_vecMasked[i]->Render();

	for (size_t i = 0; i < m_vecTrapsnarent.size(); ++i)
		m_vecTrapsnarent[i]->Render();

	for (size_t i = 0; i < m_vePostProcess.size(); ++i)
		m_vePostProcess[i]->Render();
}

void CCamera::LayerCheck(int _Idx)
{
	m_LayerCheck ^= (1 << _Idx);

}