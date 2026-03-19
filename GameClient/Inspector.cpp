#include "pch.h"
#include "Inspector.h"

#include "LevelMgr.h"
#include "PathMgr.h"
#include "AssetMgr.h"
#include "APrefab.h"

#include "TransformUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"

Inspector::Inspector()
	: EditorUI("Inspector")
	, m_PrevTargetObject(nullptr)
{
	memset(m_PrefabNameBuffer, 0, sizeof(m_PrefabNameBuffer));
	memset(m_ObjectNameBuffer, 0, sizeof(m_ObjectNameBuffer));
	CreateChildUI();

	SetTargetObject(nullptr);
}

Inspector::~Inspector()
{

}

void Inspector::SetTargetObject(Ptr<GameObject> _Object)
{
	// 입력된 게임 오브젝트의 정보를 보여줄 ComponentUI 들을 활성화 시킨다
	m_TargetObject = _Object;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComUI[i] == nullptr)
			continue;

		m_arrComUI[i]->SetTarget(m_TargetObject);
	}

	if (nullptr != m_TargetObject)
	{
		// 오브젝트의 Script 에 대응하는 ScriptUI 를 활성화 / 비활성화 시킨다
		const vector<Ptr<CScript>>& vecScripts = m_TargetObject->GetScripts();

		// 오브젝트가 보유한 Script 개수에 비해서 대응할 ScriptUI 의 개수가 모자라면 추가한다
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			int AddCount = vecScripts.size() - m_vecScriptUI.size();

			for (int i = 0; i < AddCount; ++i)
			{
				ScriptUI* pScriptUI = new ScriptUI;
				pScriptUI->SetSizeAsChild(Vec2(0.f, 150.f));
				AddChildUI(pScriptUI);

				m_vecScriptUI.push_back(pScriptUI);
			}
		}

		// 오브젝트에서 가져온 Script 를 각각의 ScriptUI 에게 세팅해준다
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (vecScripts.size() <= i)
				m_vecScriptUI[i]->SetScript(nullptr);
			else
				m_vecScriptUI[i]->SetScript(vecScripts[i].Get());
		}
	}

	else
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}

	// AssetUI 를 비활성화한다.
	m_TargetAsset = nullptr;
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (nullptr != m_arrAssetUI[i])
			m_arrAssetUI[i]->SetActive(false);
	}
}

void Inspector::SetTargetAsset(Ptr<Asset> _Asset)
{
	// ComponentUI 들을 비활성화 시킨다
	SetTargetObject(nullptr);
	
	m_TargetAsset = _Asset;
	if (nullptr == m_TargetAsset)
	{
		for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
			m_arrAssetUI[i]->SetActive(false);
	}

	else
	{
		ASSET_TYPE Type = m_TargetAsset->GetType();
		m_arrAssetUI[(UINT)Type]->SetActive(true);
		m_arrAssetUI[(UINT)Type]->SetTargetAsset(m_TargetAsset);
	}
}

void Inspector::Tick_UI()
{
	if (m_TargetObject == nullptr)
		return;

	// GameObject가 변경되었을 때만 버퍼 업데이트
	if (m_PrevTargetObject != m_TargetObject.Get())
	{
		m_PrevTargetObject = m_TargetObject.Get();
		wstring Name = m_TargetObject->GetName();
		string strName(string(Name.begin(), Name.end()));

		if (strName.empty())
			strName = "No Name";

		strcpy_s(m_ObjectNameBuffer, sizeof(m_ObjectNameBuffer), strName.c_str());
	}

	// 오브젝트 이름 편집 (InputText)
	ImGui::Text("Name:");
	ImGui::SameLine();

	if (ImGui::InputText("##ObjectName", m_ObjectNameBuffer, sizeof(m_ObjectNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// Enter 키를 눌렀을 때 이름 변경
		LevelMgr::GetInst()->GetCurrentLevel()->SetChanged();

		wstring NewName = wstring(m_ObjectNameBuffer, m_ObjectNameBuffer + strlen(m_ObjectNameBuffer));
		m_TargetObject->SetName(NewName);
	}

	// Layer Index 편집
	int CurrentLayerIdx = m_TargetObject->GetLayerIdx();
	ImGui::Text("Layer Idx:");
	ImGui::SameLine();

	if (ImGui::InputInt("##LayerIdx", &CurrentLayerIdx, 1, 10))
	{
		if (CurrentLayerIdx < -1)
			CurrentLayerIdx = -1;

		if (CurrentLayerIdx > MAX_LAYER)
			CurrentLayerIdx = MAX_LAYER - 1;

		if (CurrentLayerIdx != m_TargetObject->GetLayerIdx())
		{
			m_TargetObject->SetLayerIdx(CurrentLayerIdx);
			LevelMgr::GetInst()->GetCurrentLevel()->SetChanged();
		}
	}

	// 충돌 매트릭스 표시
	if (CurrentLayerIdx >= 0 && CurrentLayerIdx < MAX_LAYER)
	{
		ShowCollisionMatrix();
	}

	ImGui::Spacing();

	SavePrefab();

	ImGui::SameLine();
	DeleteTargetObject();

	ImGui::Separator();
}

void Inspector::SavePrefab()
{
	// Save as Prefab 버튼
	if (ImGui::Button("Save as Prefab"))
	{
		// GameObject 이름을 기본값으로 설정
		wstring Name = m_TargetObject->GetName();
		string strName(string(Name.begin(), Name.end()));

		strcpy_s(m_PrefabNameBuffer, sizeof(m_PrefabNameBuffer), strName.c_str());
		ImGui::OpenPopup("Save Prefab");
	}

	// Prefab 저장 팝업
	if (ImGui::BeginPopupModal("Save Prefab", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Save GameObject as Prefab");
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Prefab Name:");
		ImGui::SameLine();
		ImGui::InputText("##PrefabNameInput", m_PrefabNameBuffer, sizeof(m_PrefabNameBuffer));

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Save", ImVec2(120, 0)))
		{
			if (strlen(m_PrefabNameBuffer) == 0)
			{
				ImGui::OpenPopup("Error##EmptyPrefabName");
			}
			else
			{
				// Prefab 생성
				Ptr<APrefab> pNewPrefab = new APrefab;
				pNewPrefab->SetObject(m_TargetObject);

				// 파일 경로 생성
				wstring FileName = wstring(m_PrefabNameBuffer, m_PrefabNameBuffer + strlen(m_PrefabNameBuffer));
				wstring FilePath = CONTENT_PATH;
				FilePath += L"Prefab\\";
				FilePath += FileName;
				FilePath += L".pref";

				// 저장
				if (pNewPrefab->Save(FilePath) == S_OK)
				{
					// AssetMgr에 등록
					AssetMgr::GetInst()->AddAsset(FileName, pNewPrefab.Get());
					ImGui::CloseCurrentPopup();
					ImGui::OpenPopup("Prefab Save Success");
				}
				else
				{
					ImGui::OpenPopup("Error##PrefabSaveFailed");
				}
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	// 이름 비어있음 에러 팝업
	if (ImGui::BeginPopupModal("Error##EmptyPrefabName", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Prefab name cannot be empty!");
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	// 저장 실패 에러 팝업
	if (ImGui::BeginPopupModal("Error##PrefabSaveFailed", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Failed to save Prefab!");
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	// 저장 성공 팝업
	if (ImGui::BeginPopupModal("Prefab Save Success", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Prefab saved successfully!");
		ImGui::Text("File: Prefab/%s.pref", m_PrefabNameBuffer);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Inspector::DeleteTargetObject()
{
	// Delete Object 버튼
	if (ImGui::Button("Delete Object"))
	{
		ImGui::OpenPopup("Delete Confirmation");
	}

	// 삭제 확인 팝업
	if (ImGui::BeginPopupModal("Delete Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		wstring Name = m_TargetObject->GetName();
		string strName(string(Name.begin(), Name.end()));

		if (strName.empty())
			strName = "No Name";

		ImGui::Text("Are you sure you want to delete this object?");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Object: %s", strName.c_str());
		ImGui::Spacing();
		ImGui::Text("This action cannot be undone!");
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Delete", ImVec2(120, 0)))
		{
			// GameObject를 Dead 상태로 만들어서 삭제
			m_TargetObject->Destroy();

			// Inspector 타겟 초기화
			SetTargetObject(nullptr);

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void Inspector::ShowCollisionMatrix()
{
	Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	int CurrentLayerIdx = m_TargetObject->GetLayerIdx();
	if (CurrentLayerIdx < 0 || CurrentLayerIdx >= MAX_LAYER)
		return;

	UINT* pMatrix = pCurLevel->GetCollisionMatrix();

	ImGui::Indent(10.f);
	ImGui::Text("Collision Layers:");
	ImGui::Spacing();

	// 현재 레이어와 충돌 체크되는 레이어들 표시
	bool HasCollision = false;

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		bool IsColliding = false;

		// 비트 플래그 체크
		if (CurrentLayerIdx <= i)
		{
			// CurrentLayerIdx가 Row인 경우
			IsColliding = (pMatrix[CurrentLayerIdx] & (1 << i)) != 0;
		}
		else
		{
			// CurrentLayerIdx가 Col인 경우
			IsColliding = (pMatrix[i] & (1 << CurrentLayerIdx)) != 0;
		}

		if (IsColliding)
		{
			HasCollision = true;

			// 레이어 이름 가져오기
			wstring LayerName = pCurLevel->GetLayer(i)->GetName();
			string LayerNameStr;

			if (LayerName.empty())
			{
				LayerNameStr = "Layer " + to_string(i);
			}
			else
			{
				LayerNameStr = string(LayerName.begin(), LayerName.end());
			}

			// 색상 박스로 표시
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f)); // 초록색
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));

			ImGui::Button((LayerNameStr + " [" + to_string(i) + "]").c_str(), ImVec2(200, 0));

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "✓ Collision Enabled");
		}
	}

	if (!HasCollision)
	{
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), "No collision layers set");
	}

	ImGui::Unindent(10.f);
	ImGui::Spacing();
}
