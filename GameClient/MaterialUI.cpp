#include "pch.h"
#include "MaterialUI.h"

#include "PathMgr.h"
#include "AMaterial.h"
#include "EditorMgr.h"
#include "ListUI.h"
#include "AssetMgr.h"


MaterialUI::MaterialUI()
	: AssetUI(ASSET_TYPE::MATERIAL)
	, m_PrevMaterial(nullptr)
{
	memset(m_MaterialNameBuffer, 0, sizeof(m_MaterialNameBuffer));
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::Tick_UI()
{
	OutputTitle();

	Ptr<AMaterial> pMtrl = (AMaterial*)GetTargetAsset().Get();

	// Material이 변경되면 버퍼 업데이트
	if (m_PrevMaterial != pMtrl.Get())
	{
		m_PrevMaterial = pMtrl.Get();

		string Key = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
		strncpy_s(m_MaterialNameBuffer, Key.c_str(), sizeof(m_MaterialNameBuffer) - 1);
	}

	ImGui::Text("Name");
	ImGui::SameLine(100);

	// 이름 편집 가능하게 변경
	if (ImGui::InputText("##MtrlName", m_MaterialNameBuffer, sizeof(m_MaterialNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// Enter 키를 누르면 Material 이름 변경
		wstring NewName = wstring(m_MaterialNameBuffer, m_MaterialNameBuffer + strlen(m_MaterialNameBuffer));

		// 빈 이름은 허용하지 않음
		if (!NewName.empty())
		{
			// 기존 Material 이름과 다르면 변경
			if (pMtrl->GetKey() != NewName)
			{
				pMtrl->SetName(NewName);
				pMtrl->SetChanged();
			}
		}
	}

	// =======
	// Shader
	// =======
	ImGui::Text("Shader");
	ImGui::SameLine(120);

	Ptr<AGraphicShader> pShader = pMtrl->GetShader();

	wstring ShaderKey = L"None";
	if (nullptr != pShader)
	{
		ShaderKey = pShader->GetKey();
	}

	ImGui::InputText("##ShaderName", string(ShaderKey.begin(), ShaderKey.end()).data(), ShaderKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::GRAPHICSHADER == pAsset->GetType())
			{
				pMtrl->SetShader((AGraphicShader*)pAsset.Get());
				pMtrl->SetChanged();
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", Vec2(20.f, 20.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다.
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorMgr::GetInst()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Shader List");

		vector<wstring> vecShaderNames;
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHICSHADER, vecShaderNames);
		pUI->AddString(vecShaderNames);
		pUI->AddDelegate(this, (DELEGATE_1)&MaterialUI::SelectShader);
		pUI->SetActive(true);
	}


	// Shader Param
	ShaderParameter();

	// Save Button
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// 변경사항이 있으면 표시
	if (pMtrl->IsChanged())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.5f, 0.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 0.6f, 0.2f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 0.4f, 0.f, 1.f));

		if (ImGui::Button("Save##MtrlSaveBtn"))
		{
			// char* → string → wstring 안전한 변환
			string fileNameStr(m_MaterialNameBuffer);
			wstring FileName(fileNameStr.begin(), fileNameStr.end());

			wstring FilePath = CONTENT_PATH;
			FilePath += L"Material\\";
			FilePath += FileName;
			FilePath += L".mtrl";

			if (pMtrl->Save(FilePath) == S_OK)
			{
				// 저장 성공 메시지
				ImGui::OpenPopup("Save Success");
			}
		}

		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button("Save", ImVec2(200, 30)))
		{
			wstring NewFilePath = CONTENT_PATH + pMtrl->GetKey();
			pMtrl->Save(NewFilePath);
		}
	}

	// 다른 이름으로 저장 버튼
	ImGui::SameLine();
	if (ImGui::Button("Save As...", ImVec2(120, 30)))
	{
		ImGui::OpenPopup("Save Material As");
	}

	// Save As 팝업
	if (ImGui::BeginPopupModal("Save Material As", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter new material name:");
		ImGui::Spacing();

		static char newNameBuffer[256] = {};
		if (ImGui::IsWindowAppearing())
		{
			strncpy_s(newNameBuffer, m_MaterialNameBuffer, sizeof(newNameBuffer) - 1);
		}

		ImGui::InputText("##NewMtrlName", newNameBuffer, sizeof(newNameBuffer));

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Save##MtrlSaveBtn"))
		{
			// char* → string → wstring 안전한 변환
			string fileNameStr(newNameBuffer);
			wstring FileName(fileNameStr.begin(), fileNameStr.end());

			wstring FilePath = CONTENT_PATH;
			FilePath += L"Material\\";
			FilePath += FileName;
			FilePath += L".mtrl";

			if (pMtrl->Save(FilePath) == S_OK)
			{
				// 저장 성공 시 Material 이름 업데이트
				pMtrl->SetName(FileName);
				strncpy_s(m_MaterialNameBuffer, newNameBuffer, sizeof(m_MaterialNameBuffer) - 1);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void MaterialUI::ShaderParameter()
{
	Ptr<AMaterial> pMtrl = (AMaterial*)GetTargetAsset().Get();
	if (nullptr == pMtrl->GetShader())
		return;

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::Text("Shader Parameter");
	ImGui::Separator();

	const vector<ShaderParam>& vecParam = pMtrl->GetShader()->GetShaderparam();
	
	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		switch (vecParam[i].Type)
		{
		case SHADER_PARAM::INT:
			break;
		case SHADER_PARAM::FLOAT:
			break;
		case SHADER_PARAM::VEC2:
			break;
		case SHADER_PARAM::VEC4: 
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());
			ImGui::SameLine(150);

			SCALAR_PARAM Param = (SCALAR_PARAM)((UINT)SCALAR_PARAM::VEC4_0 + vecParam[i].Index);
			Vec4& Data = pMtrl->GetScalar<Vec4>(Param);

			string Key = "##VEC4_" + to_string(i);
			if (ImGui::DragFloat4(Key.c_str(), Data, 0.01f))
			{
				// 값이 변경되면 Material을 변경됨으로 표시
				pMtrl->SetChanged();
			}
		}
			break;
		case SHADER_PARAM::MAT:
			break;
		case SHADER_PARAM::TEX:
		{
			ImGui::Text(string(vecParam[i].Desc.begin(), vecParam[i].Desc.end()).c_str());

			Ptr<ATexture> pTex = pMtrl->GetTexture((TEX_PARAM)vecParam[i].Index);
			ImTextureRef SRV = nullptr;

			if (nullptr != pTex)
			{
				SRV = pTex->GetSRV().Get();

				// 텍스처 이름 표시
				ImGui::SameLine(150);
				string TexName = string(pTex->GetKey().begin(), pTex->GetKey().end());
				ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s", TexName.c_str());
			}

			// 이미지 샘플
			ImGui::ImageWithBg(SRV, ImVec2(150, 150)
							, Vec2(0.f, 0.f), Vec2(1.f, 1.f)
							, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
				if (PayLoad)
				{
					DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
					Ptr<Asset> pAsset = (Asset*)data;

					if (ASSET_TYPE::TEXTURE == pAsset->GetType())
					{
						pMtrl->SetTexture((TEX_PARAM)vecParam[i].Index, (ATexture*)pAsset.Get());
						pMtrl->SetChanged();
					}
				}

				ImGui::EndDragDropTarget();
			}

			// 텍스처 제거 버튼
			if (nullptr != pTex)
			{
				ImGui::SameLine();
				string BtnKey = "Clear##Tex_" + to_string(i);
				if (ImGui::Button(BtnKey.c_str()))
				{
					pMtrl->SetTexture((TEX_PARAM)vecParam[i].Index, nullptr);
					pMtrl->SetChanged();
				}
			}

			ImGui::Spacing();
		}
			break;
		}
	}
}

void MaterialUI::SelectShader(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = (ListUI*)_ListUI;

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AGraphicShader> pShader = FIND(AGraphicShader, key);

	((AMaterial*)GetTargetAsset().Get())->SetShader(pShader);
}
