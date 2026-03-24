#include "pch.h"
#include "StageDataUI.h"

#include "PathMgr.h"
#include "AStageData.h"
#include "GamePlayMgr.h"

StageDataUI::StageDataUI()
    : EditorUI("StageDataUI")
    , m_SelectedObjectID((int)EObjectID::DefaultJelly)
    , m_InputWorldPos{0.f, 0.f}
    , m_InputScale{100.f, 100.f}
    , m_SelectedSpawnIdx(-1)
{
    memset(m_FileNameBuffer, 0, sizeof(m_FileNameBuffer));
}

StageDataUI::~StageDataUI()
{
}

void StageDataUI::Tick_UI()
{
    // GamePlayMgr에서 StageData 가져오기
    Ptr<AStageData> pStageData = GamePlayMgr::GetInst()->GetStageData();
    if (nullptr == pStageData)
    {
        ImGui::Text("No StageData. Click to create one.");
        if (ImGui::Button("Create New StageData"))
        {
            GamePlayMgr::GetInst()->CreateNewStageData();
        }
        return;
    }

    // 파일명 버퍼 초기화 (처음 로드될 때)
    if (strlen(m_FileNameBuffer) == 0)
    {
        string Key = string(pStageData->GetKey().begin(), pStageData->GetKey().end());
        strncpy_s(m_FileNameBuffer, Key.c_str(), sizeof(m_FileNameBuffer) - 1);
    }

    // SpawnInfo 목록 출력
    DrawSpawnInfoList();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // SpawnInfo 추가 UI
    DrawAddSpawnInfo();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // 저장 버튼
    DrawSaveButton();
}

void StageDataUI::DrawSpawnInfoList()
{
    Ptr<AStageData> pStageData = GamePlayMgr::GetInst()->GetStageData();

    ImGui::Text("SpawnInfo List (%d)", pStageData->GetSpawnInfoCount());

    // 스크롤 가능한 리스트 영역
    ImGui::BeginChild("SpawnInfoList", ImVec2(0, 200), true);

    const vector<FSpawnInfo>& vecSpawnInfo = pStageData->GetSpawnInfo();

    for (int i = 0; i < (int)vecSpawnInfo.size(); ++i)
    {
        const FSpawnInfo& info = vecSpawnInfo[i];

        // 선택 가능한 항목
        bool isSelected = (m_SelectedSpawnIdx == i);
        char label[128];
        sprintf_s(label, "[%d] ID: %d, Pos: (%.1f, %.1f), Scale: (%.1f, %.1f)",
            i, info.ObjectID, info.WorldPos.x, info.WorldPos.y, info.Scale.x, info.Scale.y);

        if (ImGui::Selectable(label, isSelected))
        {
            m_SelectedSpawnIdx = i;
        }

        // 우클릭 컨텍스트 메뉴
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete"))
            {
                pStageData->RemoveSpawnInfo(i);
                m_SelectedSpawnIdx = -1;
            }
            ImGui::EndPopup();
        }
    }

    ImGui::EndChild();

    // 선택된 항목 삭제 버튼
    if (m_SelectedSpawnIdx >= 0 && m_SelectedSpawnIdx < (int)vecSpawnInfo.size())
    {
        if (ImGui::Button("Delete Selected"))
        {
            pStageData->RemoveSpawnInfo(m_SelectedSpawnIdx);
            m_SelectedSpawnIdx = -1;
        }
        ImGui::SameLine();
    }

    // 전체 삭제 버튼
    if (ImGui::Button("Clear All"))
    {
        pStageData->ClearSpawnInfo();
        m_SelectedSpawnIdx = -1;
    }
}

void StageDataUI::DrawAddSpawnInfo()
{
    Ptr<AStageData> pStageData = GamePlayMgr::GetInst()->GetStageData();

    ImGui::Text("Add New SpawnInfo");

    // Object ID 선택 (콤보박스)
    const char* objectNames[] = {
        "None",
        "DefaultJelly",
        "Coin1",
        "Coin2",
        "BearYellow",
        "BearPink",
        "BearBlue",
        "BearBigYellow",
        "BearRainbow",
        "SpecialBonus_1",
        "SpecialBonus_2",
        "Item"
    };

    int objectIDs[] = {
        (int)EObjectID::None,
        (int)EObjectID::DefaultJelly,
        (int)EObjectID::Coin1,
        (int)EObjectID::Coin2,
        (int)EObjectID::BearYellow,
        (int)EObjectID::BearPink,
        (int)EObjectID::BearBlue,
        (int)EObjectID::BearBigYellow,
        (int)EObjectID::BearRainbow,
        (int)EObjectID::SpecialBonus_1,
        (int)EObjectID::SpecialBonus_2,
        (int)EObjectID::Item
    };

    // 현재 선택된 ID에 해당하는 인덱스 찾기
    int currentIdx = 0;
    for (int i = 0; i < IM_ARRAYSIZE(objectIDs); ++i)
    {
        if (objectIDs[i] == m_SelectedObjectID)
        {
            currentIdx = i;
            break;
        }
    }

    ImGui::Text("Object Type");
    ImGui::SameLine(120);
    if (ImGui::Combo("##ObjectType", &currentIdx, objectNames, IM_ARRAYSIZE(objectNames)))
    {
        m_SelectedObjectID = objectIDs[currentIdx];
    }

    // World Position 입력
    ImGui::Text("World Pos");
    ImGui::SameLine(120);
    ImGui::InputFloat2("##WorldPos", m_InputWorldPos);

    // Scale 입력
    ImGui::Text("Scale");
    ImGui::SameLine(120);
    ImGui::InputFloat2("##Scale", m_InputScale);

    // 추가 버튼
    if (ImGui::Button("Add SpawnInfo", ImVec2(150, 30)))
    {
        FSpawnInfo newInfo;
        newInfo.ObjectID = m_SelectedObjectID;
        newInfo.WorldPos = Vec2(m_InputWorldPos[0], m_InputWorldPos[1]);
        newInfo.Scale = Vec2(m_InputScale[0], m_InputScale[1]);

        pStageData->AddSpawnInfo(newInfo);
    }
}

void StageDataUI::DrawSaveButton()
{
    Ptr<AStageData> pStageData = GamePlayMgr::GetInst()->GetStageData();

    ImGui::Text("File Name");
    ImGui::SameLine(120);
    ImGui::InputText("##FileName", m_FileNameBuffer, sizeof(m_FileNameBuffer));

    ImGui::Spacing();

    // 저장 버튼
    if (ImGui::Button("Save", ImVec2(200, 30)))
    {
        string fileNameStr(m_FileNameBuffer);
        wstring FileName(fileNameStr.begin(), fileNameStr.end());

        wstring FilePath = CONTENT_PATH;
        FilePath += L"StageData\\";
        FilePath += FileName;

        // 확장자가 없으면 추가
        if (FileName.find(L".stage") == wstring::npos)
        {
            FilePath += L".stage";
        }

        if (pStageData->Save(FilePath) == S_OK)
        {
            ImGui::OpenPopup("Save Success");
        }
        else
        {
            ImGui::OpenPopup("Save Failed");
        }
    }

    // 저장 성공 팝업
    if (ImGui::BeginPopupModal("Save Success", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("StageData saved successfully!");
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // 저장 실패 팝업
    if (ImGui::BeginPopupModal("Save Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Failed to save StageData!");
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
