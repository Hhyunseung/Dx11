#pragma once
#include "CObstructScript.h"

class CStaticObstacle : public CObstructScript
{
public:
    // 순수 가상 함수 구현
    virtual void Move() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CStaticObstacle);

public:
    CStaticObstacle();
    virtual ~CStaticObstacle();
};

