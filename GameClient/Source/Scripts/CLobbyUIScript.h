#pragma once
#include "CScript.h"

class CCharSelectBtnScript;

class CLobbyUIScript :
    public CScript
{
private:
    // 캐릭터별 미리보기 전용 프리팹 (에디터에서 ScriptParam으로 설정)
    Ptr<APrefab>    m_PreviewPrefabTK;      // 시간지기 쿠키 미리보기 프리팹
    Ptr<APrefab>    m_PreviewPrefabLychee;  // 리치 쿠키 미리보기 프리팹

    int             m_PreviewLayerIdx;      // 미리보기 오브젝트를 생성할 레이어 인덱스
    GameObject*     m_CurPreviewObj;        // 현재 보여주고 있는 미리보기 오브젝트

public:
    // CCharSelectBtnScript::OnButtonClick() 에서 호출
    void OnCharacterSelected(ECharacterType _Type);

public:
    virtual void Begin() override;
    virtual void Tick() override {}

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLobbyUIScript);

public:
    CLobbyUIScript();
    virtual ~CLobbyUIScript();

private:
    void SpawnPreview(ECharacterType _Type);
    void DestroyPreview();
    Ptr<APrefab> GetPreviewPrefab(ECharacterType _Type) const;
};
