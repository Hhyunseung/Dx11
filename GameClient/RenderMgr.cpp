#include "pch.h"
#include "RenderMgr.h"
#include "AssetMgr.h"
#include "TimeMgr.h"

#include "Device.h"

RenderMgr::RenderMgr()
{
}

RenderMgr::~RenderMgr()
{

}

void RenderMgr::Init()
{
	m_DbgObj = new GameObject;
	m_DbgObj->AddComponent(new CTransform);
	m_DbgObj->AddComponent(new CMeshRender);


	/// 디버그 렌더링 전용 재질
	m_DbgObj->MeshRender()->SetMtrl(FIND(AMaterial, L"DbgMtrl"));

}

void RenderMgr::Progress()
{
	// 렌더타겟 클리어
	Device::GetInst()->ClearTarget();

	// 카메라 기반 렌더링
	if (m_MainCam == nullptr)
		return;

	m_MainCam->Render();

	// 디버그 렌더링 요청 처리
	Render_Debug();
}

void RenderMgr::Render_Debug()
{
	list<DbgInfo>::iterator iter = m_DbgInfoList.begin();
	for (; iter != m_DbgInfoList.end(); /*++iter*/)
	{
		// Mesh 설정
		switch ((*iter).Shape)
		{
		case DBG_SHAPE::RECT:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh_LineStrip"));
			break;
		case DBG_SHAPE::CIRCLE:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"CircleMesh_LineStrip"));
			break;
		case DBG_SHAPE::CUBE:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"CubeMesh"));
			break;
		case DBG_SHAPE::SPHERE:
			m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"SphereMesh"));
			break;
		}

		if ((*iter).matWorld == XMMatrixIdentity())
		{
			// Transform 설정
			m_DbgObj->Transform()->SetRelativePos((*iter).Pos);
			m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
			m_DbgObj->Transform()->SetRelativeRot((*iter).Rotation);
			/// 월드 행렬 제작 안되어 있으므로 수동 제작
			m_DbgObj->Transform()->FinalTick(); // 월드행렬 계산
		}
		else
		{
			m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
		}

		m_DbgObj->MeshRender()->GetMtrl()->SetScalar(VEC4_0, (*iter).Color);

		if ((*iter).DepthTest)
			m_DbgObj->MeshRender()->GetMtrl()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_DbgObj->MeshRender()->GetMtrl()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);


		// Render 요청
		m_DbgObj->Render(); /// 삭제 전에 수행되므로 Life 가 0.f 여도 최소 한번은 Render() 됨


		// 최대 수명에 도달하면 정보 삭제
		(*iter).Age += DT;
		if ((*iter).Life < (*iter).Age)
		{
			/// 다음 iter 를 가져옴
			iter = m_DbgInfoList.erase(iter);
		}
		else
		{
			/// 위에서 erase 시 다음 iter 를 가져올 수 있기 때문에 for 문에서 증가시키지 않고 여기서 증가
			iter++;
		}
	}
}
