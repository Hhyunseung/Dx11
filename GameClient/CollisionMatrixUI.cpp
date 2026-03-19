#include "pch.h"
#include "CollisionMatrixUI.h"

#include "LevelMgr.h"
#include "ALevel.h"

CollisionMatrixUI::CollisionMatrixUI()
    : EditorUI("Collision Matrix")
{
    memset(m_LayerNameBuffer, 0, sizeof(m_LayerNameBuffer));
}

CollisionMatrixUI::~CollisionMatrixUI()
{
}

void CollisionMatrixUI::Tick_UI()
{
    Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
    if (nullptr == pCurLevel)
    {
        ImGui::Text("No Level Loaded");
        return;
    }

    UINT* pMatrix = pCurLevel->GetCollisionMatrix();

    ImGui::Text("Collision Matrix Configuration");
    ImGui::Separator();
    ImGui::Spacing();

    // 레이어 이름을 버퍼에 동기화
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        wstring LayerName = pCurLevel->GetLayer(i)->GetName();
        if (!LayerName.empty())
        {
            string LayerStr = string(LayerName.begin(), LayerName.end());
            strncpy_s(m_LayerNameBuffer[i], LayerStr.c_str(), sizeof(m_LayerNameBuffer[i]) - 1);
        }
        else
        {
            m_LayerNameBuffer[i][0] = '\0';
        }
    }

    // 테이블로 매트릭스 표시
    if (ImGui::BeginTable("CollisionMatrix", MAX_LAYER + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        // 헤더 행
        ImGui::TableSetupColumn("Layer", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        for (int i = 0; i < MAX_LAYER; ++i)
        {
            wstring LayerName = pCurLevel->GetLayer(i)->GetName();
            string LayerStr;
            
            if (LayerName.empty())
                LayerStr = to_string(i);
            else
                LayerStr = string(LayerName.begin(), LayerName.end());
            
            ImGui::TableSetupColumn(LayerStr.c_str(), ImGuiTableColumnFlags_WidthFixed, 30.0f);
        }
        ImGui::TableHeadersRow();

        // 각 레이어별 행
        for (int row = 0; row < MAX_LAYER; ++row)
        {
            ImGui::TableNextRow();
            
			// 첫 번째 열: 레이어 이름
			ImGui::TableSetColumnIndex(0);

			// 레이어 이름 수정 가능하게 InputText 사용
			ImGui::SetNextItemWidth(150.0f);
			if (ImGui::InputText(("##LayerName_" + to_string(row)).c_str(), 
								m_LayerNameBuffer[row], 
								sizeof(m_LayerNameBuffer[row]), 
								ImGuiInputTextFlags_EnterReturnsTrue))
			{
				// Enter 키로 레이어 이름 업데이트
				wstring NewName(m_LayerNameBuffer[row], m_LayerNameBuffer[row] + strlen(m_LayerNameBuffer[row]));
				pCurLevel->GetLayer(row)->SetName(NewName);
				pCurLevel->SetChanged();
			}

			// 레이어 번호 표시
			ImGui::SameLine();
			ImGui::TextDisabled("[%d]", row);


            // 각 열: 체크박스
            for (int col = 0; col < MAX_LAYER; ++col)
            {
                ImGui::TableSetColumnIndex(col + 1);

                // 자기 자신과의 충돌은 회색으로 비활성화
                if (row == col)
                {
                    ImGui::TextDisabled("-");
                    continue;
                }

                // 상삼각 행렬만 체크 (row < col인 경우만 편집 가능)
                if (row > col)
                {
                    // 하삼각은 읽기 전용으로 표시
                    bool IsColliding = (pMatrix[col] & (1 << row)) != 0;
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
                    ImGui::Checkbox(("##ReadOnly_" + to_string(row) + "_" + to_string(col)).c_str(), &IsColliding);
                    ImGui::PopStyleVar();
                    
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("This is mirrored from [%d][%d]", col, row);
                }
                else
                {
                    // 상삼각은 편집 가능
                    bool IsColliding = (pMatrix[row] & (1 << col)) != 0;
                    
                    if (ImGui::Checkbox(("##Check_" + to_string(row) + "_" + to_string(col)).c_str(), &IsColliding))
                    {
                        // CheckCollisionLayer 호출로 토글
                        pCurLevel->CheckCollisionLayer(row, col);
                        pCurLevel->SetChanged();
                    }

                    // 툴팁 표시
                    if (ImGui::IsItemHovered())
                    {
                        wstring RowName = pCurLevel->GetLayer(row)->GetName();
                        wstring ColName = pCurLevel->GetLayer(col)->GetName();
                        
                        string RowStr = RowName.empty() ? "Layer " + to_string(row) : string(RowName.begin(), RowName.end());
                        string ColStr = ColName.empty() ? "Layer " + to_string(col) : string(ColName.begin(), ColName.end());
                        
                        ImGui::SetTooltip("%s <-> %s", RowStr.c_str(), ColStr.c_str());
                    }
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}
