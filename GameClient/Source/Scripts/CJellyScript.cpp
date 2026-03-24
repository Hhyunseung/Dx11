#include "pch.h"
#include "CJellyScript.h"

#include "GamePlayMgr.h"
#include "GameObject.h"


CJellyScript::CJellyScript()
	: CScript(SCRIPT_TYPE::JELLYSCRIPT)
	, m_Score(1000)
	, m_Magnet(true)
	, m_MagnetRange(300.f)
	, m_MagnetSpeed(200.f)
{

}

CJellyScript::~CJellyScript()
{
}


void CJellyScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_Score, L"Score", true, 0.f);
	//AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Missile, L"Missile", true, 0.f);
	//AddScriptParam(SCRIPT_PARAM::TEXTURE, &m_Tex, L"ChangeTex");
}

void CJellyScript::Begin()
{
	// MeshRender와 FlipbookRender가 있는지 먼저 확인
	if (GetOwner()->FlipbookRender() != nullptr)
	{
		GetOwner()->FlipbookRender()->Play(0, 12.f, -1);
	}


	ADD_DYNAMIC_BEGIN_OVERLAP(CJellyScript::BeginOverlap);
}

void CJellyScript::Tick()
{
	// 위아래로 움직이는 젤리

	// 플레이어가 자석 상태일 때, 플레이어가 범위 안에 있으면 젤리가 플레이어를 향해서 움직이는 코드
}

void CJellyScript::BeginOverlap(CCollider2D* _This, CCollider2D* _Other)
{
	if (_Other->GetOwner() == GamePlayMgr::GetInst()->GetPlayerObject())
	{
		// 플레이어와 충돌했을 때의 처리
		// 점수 추가
		GamePlayMgr::GetInst()->AddScore(m_Score);

		// ============== 오브젝트 풀링을 사용할 때는 Destroy 대신 비활성화 하는 방식으로 변경 ================
		// 젤리 오브젝트 제거
		GetOwner()->Destroy();
	}
}


void CJellyScript::SaveToLevelFile(FILE* _File)
{
}

void CJellyScript::LoadFromLevelFile(FILE* _File)
{
}
