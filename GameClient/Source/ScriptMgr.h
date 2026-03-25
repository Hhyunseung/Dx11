#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BGSCROLLSCRIPT,
	CAMMOVESCRIPT,
	DOUBLEJUMPSTATE,
	EFFECTSCRIPT,
	ENEMY,
	GAMEPLAYSPAWNSCRIPT,
	JELLYSCRIPT,
	JUMPSTATE,
	LANDSTATE,
	LYCHEESCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	MOVINGPLATFORMSCIRPT,
	PLAYERSCRIPT,
	RUNSTATE,
	SCROLLSCRIPT,
	SLIDESTATE,
	STATEMACHINE,
	WORLDSCROLLSCRIPT,
};

using namespace std;

class CScript;

class ScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
