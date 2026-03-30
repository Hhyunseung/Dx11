#pragma once
#include "CObstructScript.h"

class CParabolicObstacle :
    public CObstructScript
{
private:
	bool        m_IsGrounded; // 바닥에 닿았는지 여부
	float 	    m_CurrentVelocityY; // 현재 수직 속도
	float 	    m_Gravity; // 중력 가속도

	bool 	    m_StopOnGround; // 바닥에 닿으면 멈출지 여부
	float 	    m_GroundY; // 바닥 Y 좌표 (멈출 경우)

public:
    // 순수 가상 함수 구현
    virtual void Move() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParabolicObstacle);

public:
    CParabolicObstacle();
    virtual ~CParabolicObstacle();
};

