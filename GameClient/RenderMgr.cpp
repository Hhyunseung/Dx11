#include "pch.h"

#include "Device.h"

#include "RenderMgr.h"
#include "AssetMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

RenderMgr::RenderMgr()
	: m_bDebugRender(true)
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

	m_Light2DBuffer = new StructuredBuffer;
}

void RenderMgr::Progress()
{
	if (KEY_TAP(KEY::F9))
		m_bDebugRender ? m_bDebugRender = false : m_bDebugRender = true;

	// 렌더링 시작전에 할 일 
	Render_Start();

	// 카메라 기반 렌더링
	if (m_MainCam == nullptr)
		return;

	m_MainCam->Render();

	// 디버그 렌더링 요청 처리
	if (m_bDebugRender)
		Render_Debug();

	Render_End();
}

void RenderMgr::Render_Start()
{
	// 타겟 설정
	Device::GetInst()->OMSetTarget();

	// 렌더타겟 클리어
	Device::GetInst()->ClearTarget();


	// 등록받은 Light2D 의 광원 정보를 구조화 버퍼에 담는다
	// 구조화버퍼를 특정 t 레지스터에 바인딩 한다
	vector<Light2DInfo> vecInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecInfo.push_back(m_vecLight2D[i]->GetInfo());
	}


	// 등록된 광원이 최소 1개 이상인 경우에만 데이터를 구조화 버퍼로 보낸다
	if (!vecInfo.empty())
	{
		// 구조화버퍼 공간이 모자라면 재확장 및 데이터 전달
		if (vecInfo.size() > m_Light2DBuffer->GetElementCount())
			m_Light2DBuffer->Create(sizeof(Light2DInfo), vecInfo.size(), SB_TYPE::SRV_ONLY, true, vecInfo.data());

		// 공간이 여유가 있으면 바로 광원데이터 전달
		else
			m_Light2DBuffer->SetData(vecInfo.data(), sizeof(Light2DInfo) * vecInfo.size());
	}
	

	// t12 레지스터로 바인딩
	m_Light2DBuffer->Binding(12);

	g_Global.Light2DCount = m_vecLight2D.size();

	// Global 데이터를 상수버퍼를 통해 b2 레지스터에 바인딩
	Device::GetInst()->GetCB(CB_TYPE::GLOBAL)->SetData(&g_Global);
	Device::GetInst()->GetCB(CB_TYPE::GLOBAL)->Binding();
}

void RenderMgr::Render_End()
{
	// 구조화버퍼 클리어
	// 등록받았던 광원들 해제
	m_Light2DBuffer->Clear();
	m_vecLight2D.clear();
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
