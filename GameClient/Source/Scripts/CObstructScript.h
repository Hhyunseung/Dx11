#pragma once
#include "CScript.h"

// 장애물 이동 타입
enum class EObstacleMovement
{
    Static,     // 고정 (월드 스크롤만 적용)
    Linear,     // 직선 이동
    Parabolic,  // 포물선 이동 (점프/낙하)
    Sine,       // 사인파 이동 (좌우/상하 흔들림)
    Pendulum,   // 진자 이동 (바이킹처럼 회전)
    END
};

class CObstructScript : public CScript
{
private:
    // 이동 타입
    EObstacleMovement   m_MoveType;

    // 공통 파라미터
    float               m_Speed;            // 이동 속도
	bool 			    m_ContinueAfterGround; // 바닥에 닿은 후에도 계속 이동할지 여부 (Parabolic용)

    // Linear 이동용
    Vec2                m_Direction;        // 이동 방향 (정규화)

    // Parabolic 이동용
    float               m_Gravity;          // 중력 가속도
    float               m_InitialVelocityY; // 초기 Y 속도
    bool                m_StopOnGround;     // 바닥에 닿으면 멈춤
    float               m_GroundY;          // 바닥 Y 좌표

    // Sine 이동용
    float               m_Amplitude;        // 진폭
    float               m_Frequency;        // 주파수

    // Pendulum(바이킹) 이동용
    float               m_MaxAngle;         // 최대 회전 각도 (도 단위)
    float               m_PendulumLength;   // 진자 길이 (회전 중심에서 오브젝트까지 거리)

    // 내부 상태
    float               m_AccTime;          // 경과 시간
    float               m_CurrentVelocityY; // 현재 Y 속도 (Parabolic용)
    Vec3                m_InitialPos;       // 초기 위치 (Sine용)
    bool                m_IsGrounded;       // 바닥에 닿음 여부

    // 풀 관리용
    wstring             m_PoolKey;

public:
    // Setter
    void SetMoveType(EObstacleMovement _Type) { m_MoveType = _Type; }
    void SetSpeed(float _Speed) { m_Speed = _Speed; }
    void SetDirection(Vec2 _Dir) { m_Direction = _Dir; }
    void SetGravity(float _Gravity) { m_Gravity = _Gravity; }
    void SetInitialVelocityY(float _Vel) { m_InitialVelocityY = _Vel; }
    void SetStopOnGround(bool _Stop) { m_StopOnGround = _Stop; }
    void SetGroundY(float _Y) { m_GroundY = _Y; }
    void SetAmplitude(float _Amp) { m_Amplitude = _Amp; }
    void SetFrequency(float _Freq) { m_Frequency = _Freq; }
    void SetMaxAngle(float _Angle) { m_MaxAngle = _Angle; }
    void SetPendulumLength(float _Length) { m_PendulumLength = _Length; }
    void SetPoolKey(const wstring& _Key) { m_PoolKey = _Key; }

    // 스폰 시 호출 (상태 초기화)
    void OnSpawn();

private:
    void MoveStatic();      // 월드 스크롤만 적용
    void MoveLinear();      // 직선 이동
    void MoveParabolic();   // 포물선 이동
    void MoveSine();        // 사인파 이동
    void MovePendulum();    // 진자 이동 (바이킹)

public:
    virtual void Init() override;
    virtual void Tick() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CObstructScript);

public:
    CObstructScript();
    virtual ~CObstructScript();
};

