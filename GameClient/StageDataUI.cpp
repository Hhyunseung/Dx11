#include "pch.h"
#include "StageDataUI.h"

#include "PathMgr.h"
#include "AStageData.h"
#include "GamePlayMgr.h"
#include "EditorMgr.h"
#include "Inspector.h"

StageDataUI::StageDataUI()
    : EditorUI("StageDataUI")
    , m_SelectedObjectID((int)EObjectID::DefaultJelly)
    , m_InputLayerIdx(0)
    , m_InputWorldPos{0.f, 0.f}
    , m_InputScale{100.f, 100.f}
    , m_SelectedSpawnIdx(-1)
{
    memset(m_FileNameBuffer, 0, sizeof(m_FileNameBuffer));
    m_LastFetchedTarget = nullptr;
    m_LastEditedSpawnIdx = -1;
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

    // 선택된 TargetObject에서 정보 가져오기
    DrawFetchFromTarget();

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
        sprintf_s(label, "[%d] ID: %d, Layer: %d, Pos: (%.1f, %.1f), Scale: (%.1f, %.1f)",
            i, info.ObjectID, info.LayerIdx, info.WorldPos.x, info.WorldPos.y, info.Scale.x, info.Scale.y);

        if (ImGui::Selectable(label, isSelected))
        {
            m_SelectedSpawnIdx = i;

            // Load selected spawn's params into temp maps for editing
            m_TempFloatParams.clear();
            m_TempIntParams.clear();
            m_TempBoolParams.clear();

            const vector<FSpawnInfo>& vecSpawnInfo = GamePlayMgr::GetInst()->GetStageData()->GetSpawnInfo();
            if (i >= 0 && i < (int)vecSpawnInfo.size())
            {
                const FSpawnInfo& sel = vecSpawnInfo[i];
                for (const auto& kv : sel.FloatParams)
                    m_TempFloatParams[kv.first] = kv.second;
                for (const auto& kv : sel.IntParams)
                    m_TempIntParams[kv.first] = kv.second;
                for (const auto& kv : sel.BoolParams)
                    m_TempBoolParams[kv.first] = kv.second;

                m_LastEditedSpawnIdx = i;
            }
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

    // Object ID 선택 (콤보박스) - EObjectID 열거에서 동적으로 목록 생성
    std::vector<std::string> objectNames;
    std::vector<int> objectIDs;
    objectNames.reserve(g_ObjectIDCount);
    objectIDs.reserve(g_ObjectIDCount);

    for (int i = 0; i < g_ObjectIDCount; ++i)
    {
        EObjectID id = GetObjectIDByIndex(i);
        objectIDs.push_back((int)id);
        objectNames.push_back(std::string(EObjectIDToString(id)));
    }

    // 현재 선택된 ID에 해당하는 인덱스 찾기
    int currentIdx = 0;
    for (int i = 0; i < (int)objectIDs.size(); ++i)
    {
        if (objectIDs[i] == m_SelectedObjectID)
        {
            currentIdx = i;
            break;
        }
    }

    ImGui::Text("Object Type");
    ImGui::SameLine(120);

    // ImGui용 const char* 배열 준비
    std::vector<const char*> itemPtrs;
    itemPtrs.reserve(objectNames.size());
    for (auto &s : objectNames)
        itemPtrs.push_back(s.c_str());

    if (ImGui::Combo("##ObjectType", &currentIdx, itemPtrs.data(), (int)itemPtrs.size()))
    {
        m_SelectedObjectID = objectIDs[currentIdx];
    }

    // Layer Index 입력
    ImGui::Text("Layer Index");
    ImGui::SameLine(120);
    ImGui::InputInt("##LayerIdx", &m_InputLayerIdx);

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
        newInfo.LayerIdx = m_InputLayerIdx;
        newInfo.WorldPos = Vec2(m_InputWorldPos[0], m_InputWorldPos[1]);
        newInfo.Scale = Vec2(m_InputScale[0], m_InputScale[1]);

        // If there are any temporary params collected from the Inspector fetch,
        // copy them into the new spawn info so they are saved.
        for (const auto& kv : m_TempFloatParams)
        {
            size_t pos = kv.first.find('_');
            std::string descKey = (pos != std::string::npos) ? kv.first.substr(pos + 1) : kv.first;
            newInfo.FloatParams[descKey] = kv.second;
        }
        for (const auto& kv : m_TempIntParams)
        {
            size_t pos = kv.first.find('_');
            std::string descKey = (pos != std::string::npos) ? kv.first.substr(pos + 1) : kv.first;
            newInfo.IntParams[descKey] = kv.second;
        }
        for (const auto& kv : m_TempBoolParams)
        {
            size_t pos = kv.first.find('_');
            std::string descKey = (pos != std::string::npos) ? kv.first.substr(pos + 1) : kv.first;
            newInfo.BoolParams[descKey] = kv.second;
        }

        if (m_LastEditedSpawnIdx >= 0 && m_LastEditedSpawnIdx < pStageData->GetSpawnInfoCount())
        {
            // update existing
            pStageData->UpdateSpawnInfo(m_LastEditedSpawnIdx, newInfo);
        }
        else
        {
            pStageData->AddSpawnInfo(newInfo);
        }
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

void StageDataUI::DrawFetchFromTarget()
{
    ImGui::Text("Fetch From Target Object");

    // Inspector에서 현재 선택된 TargetObject 가져오기
    Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
    if (nullptr == pInspector)
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Inspector not found!");
        return;
    }

    Ptr<GameObject> pTargetObject = pInspector->GetTargetObejct();
    if (nullptr == pTargetObject)
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "No object selected in Inspector");
        return;
    }

    // 선택된 오브젝트 정보 표시
    string objName = string(pTargetObject->GetName().begin(), pTargetObject->GetName().end());
    ImGui::Text("Selected: %s", objName.c_str());

    // Transform 컴포넌트에서 위치와 스케일 가져오기
    Ptr<CTransform> pTransform = pTargetObject->Transform();
    if (nullptr == pTransform)
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "No Transform component!");
        return;
    }

    Vec3 worldPos = pTransform->GetWorldPos();
    Vec3 worldScale = pTransform->GetWorldScale();

    ImGui::Text("Position: (%.1f, %.1f)", worldPos.x, worldPos.y);
    ImGui::Text("Scale: (%.1f, %.1f)", worldScale.x, worldScale.y);

    // 가져오기 버튼
    if (ImGui::Button("Fetch to Input Fields", ImVec2(200, 30)))
    {
        // 위치 설정
        m_InputWorldPos[0] = worldPos.x;
        m_InputWorldPos[1] = worldPos.y;

        // 스케일 설정
        m_InputScale[0] = worldScale.x;
        m_InputScale[1] = worldScale.y;

        // 레이어 인덱스 설정
        m_InputLayerIdx = pTargetObject->GetLayerIdx();

        // 오브젝트 이름에서 ObjectID
        m_SelectedObjectID = GetObjectIDFromName(pTargetObject->GetName());
    }

    ImGui::SameLine();

    if (m_LastFetchedTarget != pTargetObject.Get())
    {
        m_TempFloatParams.clear();
        m_TempIntParams.clear();
        m_TempBoolParams.clear();

        const vector<Ptr<CScript>>& vecScripts = pTargetObject->GetScripts();
        for (const auto& pScript : vecScripts)
        {
            const vector<tScriptParam>& params = pScript->GetScriptParam();
            for (const auto& sp : params)
            {
                if (!sp.IsInput) continue;

                char keyBuf[256];
                sprintf_s(keyBuf, "%d_%S", pScript->GetScriptType(), sp.Desc.c_str());

                switch (sp.Param)
                {
                case SCRIPT_PARAM::FLOAT:
                    m_TempFloatParams[keyBuf] = *(float*)(sp.Data);
                    break;
                case SCRIPT_PARAM::INT:
                    m_TempIntParams[keyBuf] = *(int*)(sp.Data);
                    break;
                case SCRIPT_PARAM::BOOL:
                    m_TempBoolParams[keyBuf] = *(bool*)(sp.Data);
                    break;
                default:
                    break;
                }
            }
        }

        m_LastFetchedTarget = pTargetObject.Get();
    }

    ImGui::Separator();
    ImGui::Text("Script Params (from selected target)");
    for (auto& kv : m_TempFloatParams)
    {
        float v = kv.second;
        if (ImGui::InputFloat(kv.first.c_str(), &v))
            kv.second = v;
    }
    for (auto& kv : m_TempIntParams)
    {
        int v = kv.second;
        if (ImGui::InputInt(kv.first.c_str(), &v))
            kv.second = v;
    }
    for (auto& kv : m_TempBoolParams)
    {
        bool v = kv.second;
        if (ImGui::Checkbox(kv.first.c_str(), &v))
            kv.second = v;
    }

    ImGui::SameLine();

    // 바로 추가 버튼
    if (ImGui::Button("Add Directly", ImVec2(150, 30)))
    {
        Ptr<AStageData> pStageData = GamePlayMgr::GetInst()->GetStageData();
        if (nullptr != pStageData)
        {
            FSpawnInfo newInfo;
            newInfo.ObjectID = GetObjectIDFromName(pTargetObject->GetName());
            newInfo.LayerIdx = pTargetObject->GetLayerIdx();
            newInfo.WorldPos = Vec2(worldPos.x, worldPos.y);
            newInfo.Scale = Vec2(worldScale.x, worldScale.y);

            for (const auto& kv : m_TempFloatParams)
            {
                std::string fullKey = kv.first;
                size_t pos = fullKey.find('_');
                std::string descKey = (pos != std::string::npos) ? fullKey.substr(pos + 1) : fullKey;
                newInfo.FloatParams[descKey] = kv.second;
            }
            for (const auto& kv : m_TempIntParams)
            {
                std::string fullKey = kv.first;
                size_t pos = fullKey.find('_');
                std::string descKey = (pos != std::string::npos) ? fullKey.substr(pos + 1) : fullKey;
                newInfo.IntParams[descKey] = kv.second;
            }
            for (const auto& kv : m_TempBoolParams)
            {
                std::string fullKey = kv.first;
                size_t pos = fullKey.find('_');
                std::string descKey = (pos != std::string::npos) ? fullKey.substr(pos + 1) : fullKey;
                newInfo.BoolParams[descKey] = kv.second;
            }

            pStageData->AddSpawnInfo(newInfo);
        }
    }
}

int StageDataUI::GetObjectIDFromName(const wstring& _Name)
{
    // Normalize input to narrow string for comparison
    std::string nameStr(_Name.begin(), _Name.end());

    // Try direct mapping using EObjectIDToString values
    for (int i = 0; i < g_ObjectIDCount; ++i)
    {
        EObjectID id = GetObjectIDByIndex(i);
        std::string idName = EObjectIDToString(id);
        if (nameStr == idName)
            return (int)id;
    }

    for (int i = 0; i < g_ObjectIDCount; ++i)
    {
        EObjectID id = GetObjectIDByIndex(i);
        std::string idName = EObjectIDToString(id);
        if (nameStr.find(idName) != std::string::npos)
            return (int)id;
    }

    return (int)EObjectID::None;
}
