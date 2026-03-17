#include "pch.h"

TransformMatrix g_Trans = {};
GlobalData g_Global = {};

const char* ASSET_TYPE_STR[(UINT)ASSET_TYPE::END] =
{
	"MESH",
	"TEXTURE",
	"MATERIAL",
	"SOUND",
	"GRAPHICSHADER",
	"COMPUTESHADER",
	"LEVEL",
	"SPRITE",
	"FLIPBOOK",
	"TILEMAP",
	"PREFAB",
};

const char* ToString(ASSET_TYPE _Type)
{
	return ASSET_TYPE_STR[(UINT)_Type];
}

const char* PLAYER_STATE_STR[(UINT)PLAYER_STATE_ID::END] =
{
	"RUN",
	"JUMP",
	"DOUBLE_JUMP",
	"LAND",
	"SLIDE",
	"HIT",
	"DEAD",
};

const char* ToString(PLAYER_STATE_ID _Type)
{
	return PLAYER_STATE_STR[(UINT)_Type];
}
