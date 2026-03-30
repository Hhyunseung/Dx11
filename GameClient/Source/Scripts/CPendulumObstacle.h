#pragma once
#include "CObstructScript.h"

class CPendulumObstacle :
    public CObstructScript
{
private:
	Vec3    m_PivotPos; // 진자의 회전 중심 위치
	
	float   m_AccTime; // 누적 시간 (진동 계산용)
	float   m_MaxAngle; // 진자의 최대 회전 각도 
	float   m_Frequency; // 진자의 진동 주파수
    float   m_PendulumLength; // 진자의 길이


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

