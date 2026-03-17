#include "pch.h"
#include "Menu.h"

#include "AssetMgr.h"
#include "EditorMgr.h"
#include "ContentUI.h"
#include "LevelMgr.h"

#include "Inspector.h"
#include "GameObject.h"

#include "Source\\ScriptMgr.h"


Menu::Menu()
	: EditorUI("Menu")
{
}

Menu::~Menu()
{
}

void Menu::Tick_UI()
{
}

void Menu::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		File();

		Level();

		View();

		GameObjectMenu();

		Asset();
		
		ImGui::EndMainMenuBar();
	}
}

void Menu::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{
			Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurrentLevel();

			// 레벨을 파일로 저장
			wstring ContentPath = CONTENT_PATH;
			pLevel->Save(ContentPath + L"Level\\TestLevel.lv");
		}

		if (ImGui::BeginMenu("Level Load"))
		{
			if (ImGui::MenuItem("TestLevel"))
			{
				Ptr<ALevel> pLevel = LOAD(ALevel, L"Level\\TestLevel.lv");
				ChangeLevel(L"Level\\TestLevel.lv");
			}

			if (ImGui::MenuItem("...Level"))
			{

			}


			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void Menu::Level()
{
	if (ImGui::BeginMenu("Level"))
	{
		bool HasLevel = LevelMgr::GetInst()->GetCurrentLevel().Get();
		bool IsPlay = false, IsPause = false, IsStop = false;
		if (HasLevel)
		{
			LEVEL_STATE CurState = LevelMgr::GetInst()->GetLevelState();
			if (LEVEL_STATE::PLAY == CurState)
				IsPlay = true;
			else if (LEVEL_STATE::PAUSE == CurState)
				IsPause = true;
			else if (LEVEL_STATE::STOP == CurState)
				IsStop = true;
		}

		if (ImGui::MenuItem("Play", nullptr, nullptr, HasLevel && !IsPlay))
		{
			ChangeLevelState(LEVEL_STATE::PLAY);
		}

		if (ImGui::MenuItem("Pause", nullptr, nullptr, HasLevel && IsPlay))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}

		if (ImGui::MenuItem("Stop", nullptr, nullptr, HasLevel && !IsStop))
		{
			ChangeLevelState(LEVEL_STATE::STOP);
		}

		ImGui::EndMenu();
	}
}

void Menu::View()
{
	if (ImGui::BeginMenu("View"))
	{
		bool ShowDemo = EditorMgr::GetInst()->IsShowDemo();

		/// ShowDemo 쪽 인자... 체크 표시
		if (ImGui::MenuItem("Demo", nullptr, &ShowDemo, true))
		{
			/// 버튼을 눌러서 여기 들어오는것이니까 ShowDemo 가 바뀌어져 있다
			EditorMgr::GetInst()->ShowDemo(ShowDemo);
		}

		Ptr<EditorUI> pInspector = EditorMgr::GetInst()->FindUI("Inspector");
		bool InspectorActive = pInspector->IsActive();

		if (ImGui::MenuItem("Inspector", nullptr, &InspectorActive))
		{
			pInspector->SetActive(InspectorActive);
		}

		Ptr<EditorUI> pOutliner = EditorMgr::GetInst()->FindUI("Outliner");
		bool OutlinerActive = pOutliner->IsActive();

		if (ImGui::MenuItem("Outliner", nullptr, &OutlinerActive))
		{
			pOutliner->SetActive(OutlinerActive);
		}

		ImGui::EndMenu();
	}
}

void Menu::GameObjectMenu()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create GameObject"))
		{
			Ptr<GameObject> pObject = new GameObject;
			pObject->SetName(L"Default");

			Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurrentLevel();
			pLevel->AddObject(0, pObject);
		}


		if (ImGui::BeginMenu("Add Component"))
		{
			for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
			{
				COMPONENT_TYPE Type = (COMPONENT_TYPE)i;
				const char* ComponentName = nullptr;

				switch (Type)
				{
				case COMPONENT_TYPE::TRANSFORM:
					ComponentName = "CTransform";
					break;
				case COMPONENT_TYPE::CAMERA:
					ComponentName = "CCamera";
					break;
				case COMPONENT_TYPE::COLLIDER2D:
					ComponentName = "CCollider2D";
					break;
				case COMPONENT_TYPE::COLLIDER3D:
					ComponentName = "CCollider3D";
					break;
				case COMPONENT_TYPE::LIGHT2D:
					ComponentName = "CLight2D";
					break;
				case COMPONENT_TYPE::LIGHT3D:
					ComponentName = "CLight3D";
					break;
				case COMPONENT_TYPE::MESHRENDER:
					ComponentName = "CMeshRender";
					break;
				case COMPONENT_TYPE::BILLBOARD_RENDER:
					ComponentName = "CBillboardRender";
					break;
				case COMPONENT_TYPE::SPRITE_RENDER:
					ComponentName = "CSpriteRender";
					break;
				case COMPONENT_TYPE::FLIPBOOK_RENDER:
					ComponentName = "CFlipbookRender";
					break;
				case COMPONENT_TYPE::PARTICLE_RENDER:
					ComponentName = "CParticleRender";
					break;
				case COMPONENT_TYPE::TILE_RENDER:
					ComponentName = "CTileRender";
					break;
				}

				if (ComponentName != nullptr)
				{
					if (ImGui::MenuItem(ComponentName))
					{
						Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
						Ptr<GameObject> pObject = pInspector->GetTargetObejct();

						if (pObject != nullptr)
						{
							Component* pComponent = nullptr;

							switch (Type)
							{
							case COMPONENT_TYPE::TRANSFORM:
								pComponent = new CTransform;
								break;
							case COMPONENT_TYPE::CAMERA:
								pComponent = new CCamera;
								break;
							case COMPONENT_TYPE::COLLIDER2D:
								pComponent = new CCollider2D;
								break;
							case COMPONENT_TYPE::COLLIDER3D:
								//pComponent = new CCollider3D;
								break;
							case COMPONENT_TYPE::LIGHT2D:
								pComponent = new CLight2D;
								break;
							case COMPONENT_TYPE::LIGHT3D:
								//pComponent = new CLight3D;
								break;
							case COMPONENT_TYPE::MESHRENDER:
								pComponent = new CMeshRender;
								break;
							case COMPONENT_TYPE::BILLBOARD_RENDER:
								pComponent = new CBillboardRender;
								break;
							case COMPONENT_TYPE::SPRITE_RENDER:
								pComponent = new CSpriteRender;
								break;
							case COMPONENT_TYPE::FLIPBOOK_RENDER:
								pComponent = new CFlipbookRender;
								break;
							case COMPONENT_TYPE::PARTICLE_RENDER:
								//pComponent = new CParticleRender;
								break;
							case COMPONENT_TYPE::TILE_RENDER:
								pComponent = new CTileRender;
								break;
							}

							if (pComponent != nullptr)
							{
								pObject->AddComponent(pComponent);
							}
						}
					}
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Add Script"))
		{
			vector<wstring> vecScriptName;
			ScriptMgr::GetScriptInfo(vecScriptName);

			for (const auto& ScriptName : vecScriptName)
			{
				if (ImGui::MenuItem(string(ScriptName.begin(), ScriptName.end()).c_str()))
				{
					Ptr<Inspector> pInspector = (Inspector*)EditorMgr::GetInst()->FindUI("Inspector").Get();
					Ptr<GameObject> pObject = pInspector->GetTargetObejct();

					if (pObject != nullptr)
					{
						CScript* pNewScript = ScriptMgr::GetScript(ScriptName);
						pObject->AddComponent(pNewScript);
					}

				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void Menu::Asset()
{
	if (ImGui::BeginMenu("Asset"))
	{
		if (ImGui::BeginMenu("Create Asset"))
		{
			if (ImGui::MenuItem("Create Material"))
			{
				Ptr<AMaterial> pMtrl = new AMaterial;
				wstring Key = GetAssetName(ASSET_TYPE::MATERIAL, L"Material\\Default Material");
				AssetMgr::GetInst()->AddAsset(Key, pMtrl.Get());
			}

			if (ImGui::MenuItem("Create Sprite"))
			{
				Ptr<ASprite> pSprite = new ASprite;
				wstring Key = GetAssetName(ASSET_TYPE::SPRITE, L"Sprite\\Default Sprite");
				AssetMgr::GetInst()->AddAsset(Key, pSprite.Get());
			}

			if (ImGui::MenuItem("Create Level"))
			{
				Ptr<ALevel> pLevel = new ALevel;
				wstring Key = GetAssetName(ASSET_TYPE::SPRITE, L"Level\\Default Level");
				AssetMgr::GetInst()->AddAsset(Key, pLevel.Get());
			}

			if (ImGui::MenuItem("Create Flipbook"))
			{
				Ptr<AFlipbook> pFlipbook = new AFlipbook;
				wstring Key = GetAssetName(ASSET_TYPE::FLIPBOOK, L"Flipbook\\Default Flipbook");
				AssetMgr::GetInst()->AddAsset(Key, pFlipbook.Get());
			}

			if (ImGui::MenuItem("Create TileMap"))
			{

			}
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}


wstring Menu::GetAssetName(ASSET_TYPE _Type, const wstring& _Name)
{
	wstring Ext;

	switch (_Type)
	{
	case ASSET_TYPE::MESH:
		Ext = L".mesh";
		break;
	case ASSET_TYPE::MATERIAL:
		Ext = L".mtrl";
		break;
	case ASSET_TYPE::TEXTURE:
	case ASSET_TYPE::SOUND:
	case ASSET_TYPE::GRAPHICSHADER:
	case ASSET_TYPE::COMPUTESHADER:
		assert(nullptr);
		break;
	case ASSET_TYPE::SPRITE:
		Ext = L".sprite";
		break;
	case ASSET_TYPE::FLIPBOOK:
		Ext = L".flip";
		break;
	case ASSET_TYPE::PREFAB:
		Ext = L".pref";
		break;
	case ASSET_TYPE::LEVEL:
		Ext = L".lv";
		break;
	}

	int i = 0;
	while (true)
	{
		wchar_t Num[50] = {};
		swprintf_s(Num, 50, L"_%d", i);

		wstring AssetName = wstring(_Name + Num + Ext);
		if (nullptr == AssetMgr::GetInst()->FindAsset(AssetName, _Type))
		{
			return AssetName;
		}

		i++;
	}
}
