#pragma once
#include "EditorUI.h"

class AStageData;

class StageDataUI :
    public EditorUI
{
private:
    int     m_SelectedObjectID;     // 선택된 오브젝트 ID
    int     m_InputLayerIdx;        // 입력용 레이어 인덱스
    float   m_InputWorldPos[2];     // 입력용 월드 좌표
    float   m_InputScale[2];        // 입력용 스케일
    int     m_SelectedSpawnIdx;     // 선택된 SpawnInfo 인덱스
    char    m_FileNameBuffer[256];  // 저장할 파일명 버퍼

public:
    virtual void Tick_UI() override;

private:
    void DrawSpawnInfoList();       // SpawnInfo 목록 출력
    void DrawAddSpawnInfo();        // SpawnInfo 추가 UI
    void DrawSaveButton();          // 저장 버튼
    void DrawFetchFromTarget();     // TargetObject에서 정보 가져오기

    int GetObjectIDFromName(const wstring& _Name);  // 이름으로 ObjectID 추론

public:
    StageDataUI();
    virtual ~StageDataUI();
};
