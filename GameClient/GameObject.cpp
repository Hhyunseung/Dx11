#include "pch.h"
#include "GameObject.h"

#include "LevelMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "TaskMgr.h"


GameObject::GameObject()
	: m_Com{}
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
{
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
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_Com[i] != nullptr)
			m_Com[i]->FinalTick();
	}

	// 자신이 소속된 Layer 에 자기자신을 알림 (등록)
	RegisterLayer();


	// 
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

void GameObject::Destroy()
{
	if (m_Dead)
		return;

	TaskInfo info = {};

	info.Type = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)this;

	TaskMgr::GetInst()->AddTask(info);
}