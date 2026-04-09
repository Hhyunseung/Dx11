#pragma once
#include "CScript.h"

#include "CMissileScript.h"
#include "CStateMachine.h"
#include "CCookieSkillScript.h"

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
	GameObject*			m_FeetObject; // 땅 체크용 충돌체 오브젝트
	Ptr<CStateMachine>	m_StateMachine; // 상태 머신

	CCookieSkillScript* m_CookieSkill; // 현재 장착된 쿠키 스킬 (없으면 nullptr)

	vector<CCollider2D*>	m_GroundColliders;			// 발과 충돌 중인 모든 플랫폼들
	CMovingPlatformScirpt*	m_CurrentMovingPlatform;	// 현재 타고 있는 이동 플랫폼 스크립트 (없으면 nullptr)

	// HP
	float				m_AutoHPDecreaseTimer; // 자동 체력 감소 타이머
	int 				m_MaxHP; // 체력
	int					m_CurrentHP; // 현재 체력
	int 				m_Damage; // 피격 시 받는 피해량
	int 				m_FallDamage; // 낙하 피해량

	// 충돌체
	float				m_PrevFeetY; // 땅 체크용 충돌체의 이전 y 좌표
	float			    m_CurFeetY; // 땅 체크용 충돌체의 y 좌표

	// 중력
	float			    m_gravity; // 중력 가속도

	// 점프
	float				m_VelY; // 수직 속도
	float 			    m_JumpPower; // 점프 힘
	float			    m_DoubleJumpPower; // 더블 점프 힘

	// 무적
	bool				m_BlockFall; // 낙하 안되게
	float 			    m_InvincibleTime; // 무적 지속 시간
	float               m_InvincibleTimer; // 무적 경과 시간
	float				m_BlinkTime; // 깜빡임 주파수 (초당 깜빡임 횟수)
	float				m_InvincibleFloorLine; // 무적 중에는 이 선 아래로 떨어질 수 없음

	// 낙하
	Vec3 				m_LastSafePos; // 마지막 안전 위치 (낙사 구출 시 이동할 위치)
	float				m_FallDeadLine; // 낙사 y 좌표
	float 			    m_FallRescueSpeed; // 낙사 구출 스피드
	float				m_FallRescueInvincibleDuration; // 낙사 구출 후 무적 지속 시간

	// 장애물
	float				m_HitInvincibleDuration; // 장애물 피격 후 무적 지속 시간

	int 				m_JumpCount; // 현재 점프 횟수
	const int			m_MaxJumpCount; // 최대 점프 횟수 (2)

	bool				m_JumpRequest; // 점프 입력이 들어왔는지 여부
	bool 				m_IsSkillMoveMode; // 스킬 사용 입력이 들어왔는지 여부

	bool				m_IsTimeKeeperSkillAnim2;

	// ========= 상태 ============
	bool 			    m_IsLand; // 땅에 닿아있는지 여부
	bool                m_IsJump; // 점프 중인지 여부
	bool 			    m_IsDoubleJump; // 점프 중인지 여부
	bool				m_IsSlide; // 슬라이드 중인지 여부
	bool				m_IsInvincible; // 무적 상태인지 여부
	bool				m_IsFallRescue; // 낙사 구출 중인지 여부
	bool				m_DeathPending; // 죽음 예정
	bool 			    m_IsDead; // 죽었는지 여부

	// ========== 아이템 ============
	// 거대화
	bool				m_IsGiant;
	float				m_GiantTimer;
	float				m_GiantDuration;
	float				m_DefaultPlayerScale; 
	float				m_GiantTargetScale;
	float				m_GiantScaleSpeed;

	bool				m_IsReturningFromGiant; // 거대화에서 원래 크기로 돌아오는 중

	// 부스트
	bool				m_IsBoost;
	float 				m_BoostTimer;
	float 				m_BoostDuration;
	float				m_DefaultScrollSpeed;
	float				m_DashScrollSpeed;

	// 자석
	bool				m_IsMagnet;
	float 				m_MagnetTimer;
	float 				m_MagnetDuration;
	float 				m_MagnetRange;

public:
	void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }
	//void SetBodyCollider(CCollider2D* _Collider) { m_BodyCollider = _Collider; }
	void SetFeetCollider(CCollider2D* _Collider) { m_FeetCollider = _Collider; }
	void SetStateMachine(Ptr<CStateMachine> _StateMachine) { m_StateMachine = _StateMachine; }
	CStateMachine* GetStateMachine() const { return m_StateMachine.Get(); }

	void SetSkillScript(CCookieSkillScript* _Skill)
	{
		m_CookieSkill = _Skill;
		if (m_CookieSkill != nullptr)
		{
			m_CookieSkill->SetPlayer(this);
			m_CookieSkill->OnEquip();
		}
	}
	CCookieSkillScript* GetSkillScript() const { return m_CookieSkill; }

	void ChangeState(PLAYER_STATE_ID _NextId) { m_StateMachine->ChangeState(_NextId); }

	void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

	void FeetBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void FeetOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void FeetEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

public:
	void SetDefaultCollider();
	void SetSlideCollider();
	void SetGiantCollider();

private:
	void HandleJump();
	void ProcessJump();
	
	void HandleSlide();
	void HandleHit();

	void UpdateAutoHPDecrease(); // 자동 체력 감소 처리

	void UpdateUIButton(); // UI 버튼 업데이트 (체력, 스킬 등)
	void ApllyMovingPlatform(); // 이동 플랫폼 위에 올라탄 경우, 플레이어 위치에 이동 플랫폼의 이동량 더해주기
	void GravityAndMove(); // 중력 적용 및 이동 처리
	void UpdateInvincibility();  // 이름 변경 // 무적

	// 무적 코드
	void StartInvincibility(float _Duration);  // 무적 시작
	void ClampDuringInvincibility(); // 무적 중 낙사 방지 위해 y 좌표 제한

	// 낙하 코드
	void CheckFallOut(); // 낙사 체크

	void BeginFallRescue(); // 낙사 구출 시작
	void UpdateFallRescue(); // 낙사 구출

	// 죽음 처리
	void Die();
	void RequestDie();


	// 아이템 효과 처리
	void UpdateItemBuffs(); // 아이템 효과 업데이트 (거대화, 부스트, 자석)
	// 거대화
	void UpdateGiantMode();
	void UpdateGiantScaleUp();

	void InitFeetColliderShape();
	void SetDefaultFeetTransform();
	void SetGiantFeetTransform();
	void SetSlideFeetTransform();
	void KeepBottomAligned(float _PrevBottomY);


	// 현재 상태에 맞는 충돌체 모양 적용 (기본/슬라이드/거대화)
	void ApplyCurrentColliderState(); 


private:
	void UpdateHPUI();

	bool HasGroundCollider(CCollider2D* _Collider);

public:
	void SetIsSkillMoveMode(bool _Value);
	bool GetIsSkillMoveMode() const { return m_IsSkillMoveMode; }

	void TakeDamage(int _Damage);
	void Heal(int _Amount); 

	void ActivateGiant(float _Duration);
	void ActivateBoost(float _Duration);
	void ActivateMagnet(float _Duration);

public:
	GET_SET(int, MaxHP);
	GET_SET(int, CurrentHP);

	GET_SET(bool, IsTimeKeeperSkillAnim2);

	GET_SET(bool, IsLand);
	GET_SET(bool, IsJump);
	GET_SET(bool, IsDoubleJump);
	GET_SET(bool, IsInvincible);
	GET_SET(bool, IsDead);

	GET_SET(bool, IsGiant);
	GET_SET(bool, IsBoost);
	GET_SET(bool, IsMagnet);
	GET_SET(float, MagnetRange);

	void RequestJump() { m_JumpRequest = true; }

public:
	virtual void Init() override;
	void Begin() override;
    virtual void Tick() override;

private:
    void Slide();

public:

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CPlayerScript);

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

