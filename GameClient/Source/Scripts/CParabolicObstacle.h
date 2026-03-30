#pragma once
#include "CObstructScript.h"

class CParabolicObstacle :
    public CObstructScript
{
private:
	bool        m_IsGrounded; // 바닥에 닿았는지 여부
    float       m_InitialVelocityY; // 초기 수직 속도 (원래 값 저장용)
    
    float       m_Speed;
    float 	    m_CurrentVelocityY; // 현재 수직 속도
	float 	    m_Gravity; // 중력 가속도


	bool 	    m_StopOnGround; // 바닥에 닿으면 멈출지 여부
	float 	    m_GroundY; // 바닥 Y 좌표 (멈출 경우)

public:
    virtual void OnSpawn() override;
    virtual void Move() override;

    virtual void ApplySpawnInfo(const FSpawnInfo& info) override;

    virtual void Init() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParabolicObstacle);

public:
    CParabolicObstacle();
    virtual ~CParabolicObstacle();
};

