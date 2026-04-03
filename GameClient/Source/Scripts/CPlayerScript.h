#pragma once
#include "CScript.h"

#include "CMissileScript.h"
#include "CStateMachine.h"

class CMovingPlatformScirpt;


class CPlayerScript;

class PlayerState 
	: public CScript
{
private:
	CPlayerScript*      m_Owner;
	PLAYER_STATE_ID       m_Id;

public:
	CPlayerScript* GetOwner() { return m_Owner; }
	PLAYER_STATE_ID GetId() { return m_Id; }

public:
	virtual void Enter(PLAYER_STATE_ID _Prev) = 0;
	virtual void Exit(PLAYER_STATE_ID _Next) = 0;

public:
	PlayerState(int _ScriptType, CPlayerScript* _Owner, PLAYER_STATE_ID _Id) 
		: CScript(_ScriptType), m_Owner(_Owner), m_Id(_Id) {}
	virtual ~PlayerState() {}
};



class CPlayerScript :
    public CScript
{
private:
    ///  어떤 물체를 가리키고 있는지 ... 는 GUID 같은 걸로 관리해서 찾는게
    Ptr<GameObject>		m_Target;
	Ptr<ATexture>       m_Tex;
	Ptr<APrefab>        m_Missile;

	//CCollider2D*		m_BodyCollider; // 충돌체 컴포넌트
	CCollider2D*		m_FeetCollider; // 땅 체크용 충돌체 컴포넌트

	Ptr<CStateMachine>	m_StateMachine; // 상태 머신

	vector<CCollider2D*>	m_GroundColliders;			// 발과 충돌 중인 모든 플랫폼들
	CMovingPlatformScirpt*	m_CurrentMovingPlatform;	// 현재 타고 있는 이동 플랫폼 스크립트 (없으면 nullptr)

	int 				m_HP; // 체력
	int					m_CurrentHP; // 현재 체력
	int 				m_Damage; // 피격 시 받는 피해량

	float				m_PrevFeetY; // 땅 체크용 충돌체의 이전 y 좌표
	float			    m_CurFeetY; // 땅 체크용 충돌체의 y 좌표

	float			    m_gravity; // 중력 가속도

	float				m_VelY; // 수직 속도
	float 			    m_JumpPower; // 점프 힘
	float			    m_DoubleJumpPower; // 더블 점프 힘
	float 			    m_InvincibleTime; // 무적 지속 시간
	float               m_InvincibleTimer; // 무적 경과 시간
	float				m_BlinkTime; // 깜빡임 주파수 (초당 깜빡임 횟수)

	int 				m_JumpCount; // 현재 점프 횟수
	const int			m_MaxJumpCount; // 최대 점프 횟수 (2)

	bool				m_JumpRequest; // 점프 입력이 들어왔는지 여부

	bool 			    m_IsLand; // 땅에 닿아있는지 여부
	bool                m_IsJump; // 점프 중인지 여부
	bool 			    m_IsDoubleJump; // 점프 중인지 여부
	bool				m_IsSlide; // 슬라이드 중인지 여부
	bool				m_IsInvincible; // 무적 상태인지 여부

public:
	void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }
	//void SetBodyCollider(CCollider2D* _Collider) { m_BodyCollider = _Collider; }
	void SetFeetCollider(CCollider2D* _Collider) { m_FeetCollider = _Collider; }
	void SetStateMachine(Ptr<CStateMachine> _StateMachine) { m_StateMachine = _StateMachine; }

	void ChangeState(PLAYER_STATE_ID _NextId) { m_StateMachine->ChangeState(_NextId); }

	void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

	void FeetBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void FeetOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void FeetEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

public:
	void SetDefaultCollider();
	void SetSlideCollider();

private:
	void HandleJump();
	void ProcessJump();
	
	void HandleSlide();
	void HandleHit();

	void UpdateUIButton();
	void ApllyMovingPlatform();
	void GravityAndMove();
	void UpdateInvincibility();  // 이름 변경

private:
	bool HasGroundCollider(CCollider2D* _Collider);

public:
	void TakeDamage(int _Damage);  // 외부에서 호출 가능

public:
	GET_SET(bool, IsLand);
	GET_SET(bool, IsJump);
	GET_SET(bool, IsDoubleJump);
	GET_SET(bool, IsInvincible);

	void RequestJump() { m_JumpRequest = true; }

public:
	virtual void Init() override;
	void Begin() override;
    virtual void Tick() override;

private:
    void Slide();

public:
    virtual void Skill();

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CPlayerScript);

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

