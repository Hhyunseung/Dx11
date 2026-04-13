#include "pch.h"
#include "GameObject.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "TaskMgr.h"

#include "Source/ScriptMgr.h"

GameObject::GameObject()
	: m_Com{}
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
	, m_Active(true)
{
}

GameObject::GameObject(const GameObject& _Origin)
	: Entity(_Origin)
	, m_Com{}
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
	, m_Active(true)
{
	// 원본 오브젝트와 동일한 세팅의 컴포넌트를 복사해서 나한테 넣어준다
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Origin.m_Com[i] == nullptr)
			continue;
		
		AddComponent(_Origin.m_Com[i]->Clone());
	}

	// 원본 오브젝트와 동일한 스크립트를 복사해서 낳나테 넣어준다
	for (const auto& Script : _Origin.m_vecScripts)
	{
		AddComponent(Script->Clone());
	}

	// 원본 오브젝트가 보유한 자식 오브젝트를 복사해서 나한테 넣어준다
	for (const auto& Child : _Origin.m_vecChild)
	{
		// 자식 오브젝트들도 Clone() 내부에서 복사 생성자가 호출되어서 자식의 자식들도 복사되어 들어감
		AddChild(Child->Clone());
	}
}

GameObject::~GameObject()
{
}

void GameObject::Begin()
{
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Begin();
	}

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_Com[i] != nullptr)
			m_Com[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}

}

void GameObject::Tick()
{
	// 비활성화 상태면 Tick 스킵
	if (!m_Active)
		return;

	//// 다운 캐스팅 /// 안정성을 체크해야함 
	//Ptr<CTransform> pTrans = nullptr;
	//if (COMPONENT_TYPE::TRANSFORM == m_Com[(UINT)COMPONENT_TYPE::TRANSFORM]->GetType())
	//{
	//	pTrans = (CTransform*)m_Com[(UINT)COMPONENT_TYPE::TRANSFORM].Get();
	//}

	//// 상속 관계에서 부모 클래스에 가상함수가 1개라도 있으면, C++ 에서는 타입정보를 자료형 별로 생성한다
	//// 이것을 기반으로 다운캐스팅 성공 여부를 판단할 수 있다
	//	/// 가상함수테이블의 타입정보를 확인 // 따라서 가상함수가 무조건 있어야함
	//	/// 비교연산자가 쓰이기 때문에 조금 느림
	//pTrans = dynamic_cast<CTransform*> (m_Com[UINT(COMPONENT_TYPE::TRANSFORM)].Get());


	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Tick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void GameObject::FinalTick()
{
	// 비활성화 상태면 FinalTick 스킵
	if (!m_Active)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_Com[i] != nullptr)
			m_Com[i]->FinalTick();
	}

	// 자신이 소속된 Layer 에 자기자신을 알림 (등록)
	RegisterLayer();

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식 오브젝트가 있으면, Vector 에서 제거한다
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

/// 원래 override 해서 스크립트에서 FinalTick 구현할 수 있게 하는게 더 좋긴하나 우리 프로젝트는 규모가 적으니까..
void GameObject::FinalTick_Editor()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_Com[i] != nullptr)
			m_Com[i]->FinalTick();
	}

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식 오브젝트가 있으면, Vector 에서 제거한다
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}


void GameObject::RegisterLayer()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();

	Layer* pLayer = pCurLevel->GetLayer(m_LayerIdx);

	pLayer->RegisterObject(this);
}

void GameObject::Render()
{
	// 렌더링 관련 기능을 보유한 컴포넌트가 없으면 GameObject 는 Rendering 될 수 없다
	if (m_RenderCom != nullptr)
	{
		Transform()->Binding();
		m_RenderCom->Render();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}
}

void GameObject::AddComponent(Ptr<Component> _Com)
{
	// 렌더링 기능 컴포넌트는 하나만 가질 수 있음
	if (dynamic_cast<CRenderComponent*>(_Com.Get())) // 실패 시 nullptr
	{
		assert(!m_RenderCom.Get());

		m_RenderCom = (CRenderComponent*)_Com.Get();
	}

	// 입력으로 들어온 컴포넌트가 스크립트면 vector 로 관리
	if (_Com->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScripts.push_back((CScript*)_Com.Get());
	}

	// 입력으로 들어온 컴포넌트가 스크립트가 아니면, 알맞은 배열 포인터로 가리킴
	else
	{
		// 해당 컴포넌트를 이미 가지고 있지 않아야 한다
		assert(nullptr == m_Com[(UINT)_Com->GetType()]);

		m_Com[(UINT)_Com->GetType()] = _Com;
	}

	_Com->m_Owner = this;

	_Com->Init();
}

void GameObject::AddChild(Ptr<GameObject> _Child)
{
	/// 부모 오브젝트가 있는지 확인
	if (_Child->GetParent().Get())
	{
		// 기존 부모 오브젝트와 관계를 해제한다
		_Child->DisconnectWithParent();
	}

	// 최상위 부모 오브젝트 였다면
	else
	{
		// 레벨 내부에 있던 오브젝트라면
		if (_Child->m_LayerIdx != -1)
		{
			// Layer 에서 최상위 부모로 가리키던 포인터를 제거
			_Child->DeregisterAsParent();
		}
	}

	m_vecChild.push_back(_Child); 
	_Child->m_Parent = this;

	// 자식으로 들어오려는 오브젝트가 원래 이 레벨 소속이 아니라 외부에서 들어온 경우
	/// 레이어가 없는 소속이면
	if (_Child->m_LayerIdx == -1)
	{
		/// 지금 나의 레이어 인덱스를 자식에게 물려준다
		_Child->m_LayerIdx = m_LayerIdx;

		// 부모가 될 오브젝트는 레벨 내부 소속인 경우 + 레벨이 Play 모드
		if (m_LayerIdx != -1 && LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState())
		{
			// Play 중인 레벨 안에 있는 어떤 오브젝트의 자식으로서 레벨에 합류했기 때문에 Begin 호출
  			_Child->Begin();
		}
	}

	// 부모 오브젝트가 레벨 소속이 아니면
	if (m_LayerIdx != -1)
	{
		/// 둘다 레벨 밖 소속이면 레이어 인덱스는 -1 이니까 Level 에 변경사항이 생긴걸 알릴 필요가 없다
		LevelMgr::GetInst()->GetCurrentLevel()->SetChanged();
	}
}

void GameObject::DisconnectWithParent()
{
	if (nullptr == m_Parent)
		return;

	if (m_LayerIdx != -1)
	{
		LevelMgr::GetInst()->GetCurrentLevel()->SetChanged();
	}

	vector<Ptr<GameObject>>::iterator iter = m_Parent->m_vecChild.begin();

	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void GameObject::DeregisterAsParent()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();

	Layer* pLayer = pCurLevel->GetLayer(m_LayerIdx);

	pLayer->DeregisterAsParent(this);
}

void GameObject::RegisterAsParent()
{
	if (m_LayerIdx == -1)
		return;

	LevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_LayerIdx)->AddObject(this);
}

void GameObject::Destroy()
{
	if (m_Dead)
		return;

	TaskInfo info = {};

	info.Type = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)this;

	TaskMgr::GetInst()->AddTask(info);
}

void GameObject::SaveToLevelFile(FILE* _File)
{
	// 이름
	SaveWString(_File, GetName());

	// 컴포넌트
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_Com[i])
			continue;

		// 컴포넌트 타입
		fwrite(&i, sizeof(UINT), 1, _File);

		// 컴포넌트 내용
		m_Com[i]->SaveToLevelFile(_File);
	}

	// 컴포넌트 끝
	UINT ComEnd = (UINT)COMPONENT_TYPE::END;
	fwrite(&ComEnd, sizeof(UINT), 1, _File);

	// 스크립트
	size_t ScriptCount = m_vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (const auto& Script : m_vecScripts)
	{
		wstring ScriptName = ScriptMgr::GetScriptName(Script.Get());
		SaveWString(_File, ScriptName);

		Script->SaveToLevelFile(_File);
	}

	// Child 오브젝트
	size_t ChildCount = m_vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (const auto& Child : m_vecChild)
	{
		Child->SaveToLevelFile(_File);
	}


}

void GameObject::LoadFromLevelFile(FILE* _File)
{
	// 이름
	SetName(LoadWString(_File));

	// 컴포넌트
	UINT ComType = 0;

	while (true)
	{
		fread(&ComType, sizeof(UINT), 1, _File);

		if (ComType == (UINT)COMPONENT_TYPE::END)
			break;

		Ptr<Component> pComponent = nullptr;

		switch ((COMPONENT_TYPE)ComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			break;
		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//pComponent = new CCollider3D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			//pComponent = new CTransform;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;
		case COMPONENT_TYPE::BILLBOARD_RENDER:
			pComponent = new CBillboardRender;
			break;
		case COMPONENT_TYPE::SPRITE_RENDER:
			pComponent = new CSpriteRender;
			break;
		case COMPONENT_TYPE::FLIPBOOK_RENDER:
			pComponent = new CFlipbookRender;
			break;
		case COMPONENT_TYPE::PARTICLE_RENDER:
			//pComponent = new CTransform;
			break;
		case COMPONENT_TYPE::TILE_RENDER:
			pComponent = new CTileRender;
			break;
		}

		AddComponent(pComponent);
		pComponent->LoadFromLevelFile(_File);
	}



	// 스크립트
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = LoadWString(_File);
		Ptr<CScript> pScript = ScriptMgr::GetScript(ScriptName);
		AddComponent(pScript.Get());

		pScript->LoadFromLevelFile(_File);
	}

	// 자식 오브젝트 복구
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		Ptr<GameObject> ChildObject = new GameObject;
		AddChild(ChildObject);
		ChildObject->LoadFromLevelFile(_File);
	}

}