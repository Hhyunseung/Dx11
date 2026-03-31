#pragma once
#include "CObstructScript.h"

class CPendulumObstacle :
    public CObstructScript
{
private:
	Vec3    m_LocalCenterOffset;
	
	float   m_AccTime; // 누적 시간 (진동 계산용)
	float   m_MaxAngle; // 진자의 최대 회전 각도 
	float   m_Frequency; // 진자의 진동 주파수
    float   m_PendulumLength; // 진자의 길이

    bool    m_OneMove; // 한번만 움직일거면 ture
    bool    m_IsStopped;
	float   m_OneMoveTime; // 한번만 움직이는 경우, 움직인 시간 기록

	GameObject* m_pBody; // 진자 몸체 (회전 중심에서 떨어진 부분)

public:
    virtual void Init() override;
    // 순수 가상 함수 구현
    virtual void Move() override;


    virtual void ApplySpawnInfo(const FSpawnInfo& info) override;


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPendulumObstacle);

public:
    CPendulumObstacle();
    virtual ~CPendulumObstacle();
};

