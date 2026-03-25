#pragma once
#include "Entity.h"

#include "components.h"

#define GET_COMPONENT(COM_NAME, COM_TYPE) Ptr<C##COM_NAME> COM_NAME() { return (C##COM_NAME*)m_Com[(UINT)COMPONENT_TYPE::COM_TYPE].Get(); }



class GameObject :
    public Entity
{
private:
    Ptr<Component>          m_Com[(UINT)COMPONENT_TYPE::END];
    Ptr<CRenderComponent>   m_RenderCom;
    vector<Ptr<CScript>>    m_vecScripts;

    /// 서로를 Ptr 로 가리키면 refCount 가 안줄어들이니까 자식입장에서는 부모를 일반 포인터로 가리키게 함 
    GameObject*             m_Parent;
    vector<Ptr<GameObject>> m_vecChild;

    // GameObject 본인이 속한 LayerIndex
    // -1 인 경우, 어떤 레이어에도 속하지 않는다 == 레벨안에 있지 않은 오브젝트
    int                     m_LayerIdx; 
    bool 				    m_Dead;
    bool                    m_Active;   // 활성화 상태 (오브젝트 풀링용)

public:
    void Begin();

    // 매 프레임마다 할 일
    void Tick();

    void FinalTick();

	// 레이어에 없는 오브젝트는 에디터에서만 존재하기 때문에, 에디터에서만 호출되는 Tick
    void FinalTick_Editor();

    // 자신을 타겟에 그림
    void Render();

public:
    void AddComponent(Ptr<Component> _Com);
    Ptr<Component> GetComponent(COMPONENT_TYPE _Type) { return m_Com[(UINT)_Type]; }

    vector<Ptr<CScript>> GetScripts() { return m_vecScripts; }

    template<typename T>
    Ptr<T> GetScript();

    void AddChild(Ptr<GameObject> _Child);
	void DisconnectWithParent();
	void DeregisterAsParent();
    void RegisterAsParent();

	// return 값은 사라지므로 스마트 포인터로 반환 ?
	Ptr<GameObject> GetParent() { return m_Parent; }
	Ptr<GameObject> GetChild(int _idx) { return m_vecChild[_idx]; }
	const vector<Ptr<GameObject>>& GetChild() { return m_vecChild; }

	bool IsDead() { return m_Dead; }
	void Destroy();

	bool IsActive() { return m_Active; }
	void SetActive(bool _Active) { m_Active = _Active; }

	int GetLayerIdx() { return m_LayerIdx; }
	void SetLayerIdx(int _LayerIdx) { m_LayerIdx = _LayerIdx; }

    // GET_COMPONENT
	GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(TileRender, TILE_RENDER);
    GET_COMPONENT(FlipbookRender, FLIPBOOK_RENDER);
    GET_COMPONENT(SpriteRender, SPRITE_RENDER);
    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
	GET_COMPONENT(BillboardRender, BILLBOARD_RENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);

    Ptr<CRenderComponent> GetRenderCom() { return m_RenderCom; }

public:
    void SaveToLevelFile(FILE* _File);
	void LoadFromLevelFile(FILE* _File);

private:
    void RegisterLayer();

public:
    CLONE(GameObject);

public:
    GameObject();
    GameObject(const GameObject& _Origin);
    virtual ~GameObject();

    friend class Layer;
    // public 으로 Dead 처리를 하지 않기 위해
    friend class TaskMgr;
};

// 스마트 포인터 원본 참조
bool IsValid(Ptr<GameObject>& _Object);

template<typename T>
inline Ptr<T> GameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScripts.size(); ++i)
    {
        T* pScript = dynamic_cast<T*>(m_vecScripts[i].Get());
        if (nullptr == pScript)
            continue;

        return pScript;
    }

    return nullptr;
}