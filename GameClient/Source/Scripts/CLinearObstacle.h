#pragma once
#include "CObstructScript.h"
class CLinearObstacle :
    public CObstructScript
{
private:
	float m_SpeedX; // 이동 속도 X
	float m_SpeedY; // 이동 속도 Y

    float m_GroundX;
    float m_GroundY;

	bool m_IsGroundedX;
	bool m_IsGroundedY;

public:
    virtual void OnSpawn() override;
    virtual void Move() override;

    virtual void ApplySpawnInfo(const FSpawnInfo& info) override;

    virtual void Init() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLinearObstacle);

public:
    CLinearObstacle();
    virtual ~CLinearObstacle();
};

