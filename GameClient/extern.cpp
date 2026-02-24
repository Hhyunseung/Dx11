#include "pch.h"
#include "extern.h"

TransformMatrix g_Trans = {};
GlobalData g_Global = {};

const char* ASSET_TYPE_STR[(UINT)ASSET_TYPE::END] =
{
	"MESH",
	"TEXTURE",
	"MATERIAL",
	"SOUND",
	"GRAPHICSHADER",
	"LEVEL",
	"SPRITE",
	"FLIPBOOK",
	"TILEMAP",
};

const char* ToString(ASSET_TYPE _Type)
{
	return ASSET_TYPE_STR[(UINT)_Type];
}
