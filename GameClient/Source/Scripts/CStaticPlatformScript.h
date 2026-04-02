#pragma once
#include "CScript.h"
class CStaticPlatformScript :
    public CScript
{
public:
    virtual void Init() override;
    virtual void ApplySpawnInfo(const FSpawnInfo& info) override;
    virtual void Begin() override;
    virtual void Tick() override;

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CStaticPlatformScript);

public:
    CStaticPlatformScript();
    virtual ~CStaticPlatformScript();
};

