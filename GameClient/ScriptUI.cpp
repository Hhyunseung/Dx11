#include "pch.h"
#include "ScriptUI.h"

#include "TreeUI.h"
#include "APrefab.h"
#include <Source/ScriptMgr.h>



ScriptUI::ScriptUI()
	: ComponentUI(COMPONENT_TYPE::SCRIPT, "ScriptUI")
	, m_ItemHeight(0)
{
	// Inspector 가 표기하려는 GameObject 가 여러개의 Script 를 가지고 있을 수 있기 때문에
	// 각 Script 에 대응하는 ScriptUI 도 여러개가 될 수 있다.
	// 따라서 ScriptUI 끼리 Inspector 의 자식으로서 서로의 이름이 겹치지 않도록 추가로 Key 를 작성한다
	int idx = GetID();
	char szNum[50] = {};
	_itoa_s(idx, szNum, 50, 10);

	SetUIKey(szNum);
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::SetScript(CScript* _Script)
{
	m_TargetScript = _Script;

	if (nullptr == m_TargetScript)
		SetActive(false);
	else
		SetActive(true);
}

void ScriptUI::Tick_UI()
{
	m_ItemHeight = 0;

	// 스크립트 이름 출력
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.3f, 1.f));

	wstring WScriptName = ScriptMgr::GetScriptName(m_TargetScript.Get());
	string ScriptName = string(WScriptName.begin(), WScriptName.end());

	ImGui::Button(ScriptName.c_str());
	AddItemHeight();

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// Script 파라미터
	const vector<tScriptParam>& vecParam = m_TargetScript->GetScriptParam();

	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		char ID[255] = {};
		sprintf_s(ID, 255, "%d", i);

		switch (vecParam[i].Param)
		{
		case SCRIPT_PARAM::INT:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##Int";
			Key += ID;

			if (vecParam[i].IsInput)
				ImGui::InputInt(Key.c_str(), (int*)vecParam[i].Data, vecParam[i].Step);
			else
				ImGui::DragInt(Key.c_str(), (int*)vecParam[i].Data, vecParam[i].Step);

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::TEXT:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##Text";
			Key += ID;

			wstring* pWstr = (wstring*)vecParam[i].Data;
			if (pWstr == nullptr)
				break;

			char buf[1024] = {};

			// wstring -> UTF-8
			if (!pWstr->empty())
			{
				WideCharToMultiByte(
					CP_UTF8,
					0,
					pWstr->c_str(),
					-1,
					buf,
					(int)sizeof(buf),
					nullptr,
					nullptr
				);
			}
			else
			{
				buf[0] = '\0';
			}

			if (ImGui::InputText(Key.c_str(), buf, sizeof(buf)))
			{
				// UTF-8 -> wstring
				int wlen = MultiByteToWideChar(CP_UTF8, 0, buf, -1, nullptr, 0);
				if (wlen > 0)
				{
					std::wstring temp;
					temp.resize(wlen); // null 포함 크기로 확보

					MultiByteToWideChar(
						CP_UTF8,
						0,
						buf,
						-1,
						temp.data(),
						wlen
					);

					if (!temp.empty() && temp.back() == L'\0')
						temp.pop_back(); // 마지막 null 제거

					*pWstr = std::move(temp);
				}
				else
				{
					pWstr->clear();
				}
			}

			AddItemHeight();
		}
		break;
			break;
		case SCRIPT_PARAM::BOOL:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##Bool";
			Key += ID;

			ImGui::Checkbox(Key.c_str(), (bool*)vecParam[i].Data);

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::FLOAT:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##Float";
			Key += ID;

			if (vecParam[i].IsInput)
				ImGui::InputFloat(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step);
			else
				ImGui::DragFloat(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step);

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::VEC2:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);
			string Key = "##VEC2";
			Key += ID;
			if (vecParam[i].IsInput)
				ImGui::InputFloat2(Key.c_str(), (float*)vecParam[i].Data, "%.3f");
			else
				ImGui::DragFloat2(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step);
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::VEC4:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##VEC4";
			Key += ID;

			if (vecParam[i].IsInput)
				ImGui::InputFloat4(Key.c_str(), (float*)vecParam[i].Data, "%.3f");
			else
				ImGui::DragFloat4(Key.c_str(), (float*)vecParam[i].Data, vecParam[i].Step);

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::MATRIX:
			break;
		case SCRIPT_PARAM::TEXTURE:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			AddItemHeight();

			string Key = "##Tex";
			Key += ID;

			Ptr<ATexture> pTex = *((Ptr<ATexture>*)vecParam[i].Data);

			ImTextureRef TexID = nullptr;
			if (nullptr != pTex)
			{
				TexID = (ImTextureRef)pTex->GetSRV().Get();
			}

			ImGui::ImageWithBg(TexID, ImVec2(200, 200)
				, Vec2(0.f, 0.f), Vec2(1.f, 1.f)
				, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("Content");
				if (PayLoad)
				{
					DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
					Ptr<Asset> pAsset = (Asset*)data;

					if (ASSET_TYPE::TEXTURE == pAsset->GetType())
					{
						*((Ptr<ATexture>*)vecParam[i].Data) = ((ATexture*)pAsset.Get());
					}
				}

				ImGui::EndDragDropTarget();
			}

			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::MATERIAL:
			break;
		case SCRIPT_PARAM::PREFAB:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##Prefab";
			Key += ID;

			Ptr<APrefab> pPrefab = *((Ptr<APrefab>*)vecParam[i].Data);

			string PrefabName = "None";
			if (nullptr != pPrefab)
			{
				PrefabName = string(pPrefab->GetKey().begin(), pPrefab->GetKey().end());
			}

			ImGui::InputText(Key.c_str(), PrefabName.data(), PrefabName.length() + 1, ImGuiInputTextFlags_ReadOnly);
			AddItemHeight();

			// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("Content");
				if (PayLoad)
				{
					DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
					Ptr<Asset> pAsset = (Asset*)data;

					if (ASSET_TYPE::PREFAB == pAsset->GetType())
					{
						*((Ptr<APrefab>*)vecParam[i].Data) = ((APrefab*)pAsset.Get());
					}
				}

				ImGui::EndDragDropTarget();
			}
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::EObjectID:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(120);

			string Key = "##EObjectID";
			Key += ID;

			EObjectID* pObjectID = (EObjectID*)vecParam[i].Data;

			// 현재 선택된 항목의 인덱스 찾기
			int currentIdx = 0;
			for (int j = 0; j < g_ObjectIDCount; ++j)
			{
				if (GetObjectIDByIndex(j) == *pObjectID)
				{
					currentIdx = j;
					break;
				}
			}

			// 콤보박스 표시
			if (ImGui::BeginCombo(Key.c_str(), EObjectIDToString(*pObjectID)))
			{
				for (int j = 0; j < g_ObjectIDCount; ++j)
				{
					bool isSelected = (currentIdx == j);
					if (ImGui::Selectable(EObjectIDToString(GetObjectIDByIndex(j)), isSelected))
					{
						*pObjectID = GetObjectIDByIndex(j);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			AddItemHeight();
		}
			break;
		default:
			break;
		}
	}

	SetSizeAsChild(Vec2(0.f, (float)m_ItemHeight));
}

void ScriptUI::AddItemHeight()
{
	ImVec2 vSize = ImGui::GetItemRectSize();
	m_ItemHeight += vSize.y + 5.f;
}