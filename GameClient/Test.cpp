#include "pch.h"

#include "Device.h"
#include "PathMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

#include "AMesh.h"
#include "AGraphicShader.h"
#include "ATexture.h"


#include "GameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

#include "AssetMgr.h"

// 오브젝트
Ptr<GameObject> g_Object1;
Ptr<GameObject> g_Object2;

// 오브젝트 위치
Vec4 g_ObjectPos = Vec4(0.f, 0.f, 0.f, 0.f);
Vec4 g_ObjectScale = Vec4(1.0f, 1.0f, 1.0f, 0.f);
Vec4 g_ObjectRot = Vec4(0.f, 0.f, 0.f, 0.f);

int TestInit()
{
	// 각 자료형의 정보
	const type_info& info = typeid(Entity);
	const char* pStr = info.name();
	DWORD num = info.hash_code();

	//AMesh* p = new AMesh;
	//Ptr<AMesh> pMesh = p;
	//p = pMesh.Get();

	//p = new AMesh;
	//p->AddRef();
	//Ptr<AMesh> pMesh2 = nullptr;
	//*pMesh.GetAdressOf() = p;

	//pMesh == pMesh2;
	//pMesh == p;
	//p == pMesh;




	return S_OK;
}

void TestTick()
{
	/// 버텍스쉐이더에서는 정점을 이동시켜서 찍어서 레스터라이저로 보냄
	g_Object1->Tick();
	g_Object2->Tick();
}

void TestRender()
{
	// 이전에 그려진 그림을 지운다
	// 렌더타겟은 검은색으로, 깊이 타겟은 최대값인 1.f 로 초기화
	Device::GetInst()->ClearTarget();

	g_Object1->Render();
	g_Object2->Render();
}

int TestFunc()
{
	static bool bInit = false;
	if (!bInit)
	{
		if (FAILED(TestInit()))
			return E_FAIL;
			
		bInit = true;
	}

	TestTick();
	TestRender();

	return S_OK;
}