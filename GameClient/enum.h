#pragma once

enum class CB_TYPE
{
	TRANSFORM, // b0 // 행렬변환 
	MATERIAL,  // b1 // 재질의 상수데이터
	GLOBAL,    // b2

	END,
};


enum class RS_TYPE
{
	CULL_BACK,  // 뒷면 (정점 반시계방향으로 그려진 면) 컬링
	CULL_FRONT, // 앞면 (정점 시계방향으로 그려진 면) 컬링
	CULL_NONE,  // 컬링 X
	WIRE_FRAME, // 정점과 정점 사이를 잇는 부분을 제외하고 컬링

	END,
};

enum class DS_TYPE
{
	LESS,				// 깊이 판정 LESS 깊이 - 성공 시 덮어쓰기 // 기본 옵션 /// 더 가까워야 통과
	LESS_EQUL,			// 깊이 판정 LSEFF_EQUAL 
	NO_TEST,			// 깊이 판정 X(무조건 성공) 성공 시 - 덮어쓰기
	NO_TEST_NO_WRITE,	// 깊이 판정 X(무조건 성공) 성공 시 - 깊이 기록 X 

	END,
}
;

enum class BS_TYPE
{
	DEFAULT,		// (ScrRGB * 1) + (DestRGB * 0)
	ALPHABLEND,		// (ScrRGB * SrcAlpha) + (DestRGB * (1 - SrcAlpha))
	ONE_ONE,		// (ScrRGB * 1) + (DestRGB * 1)

	END,
};





enum class ASSET_TYPE
{
	MESH,
	TEXTURE,
	MATERIAL,
	SOUND,
	GRAPHICSHADER,
	COMPUTESHADER,
	LEVEL,
	SPRITE,
	FLIPBOOK,
	TILEMAP,
	PREFAB,
	STAGE_DATA,

	END,
};

enum class COMPONENT_TYPE
{
	// 게임엔진에서 기본적으로 제공하는 컴포넌트들
	TRANSFORM,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	LIGHT2D,
	LIGHT3D,

	MESHRENDER,
	BILLBOARD_RENDER,
	SPRITE_RENDER,
	FLIPBOOK_RENDER,
	PARTICLE_RENDER,
	TILE_RENDER,

	END,

	// 게임에서 직접 구현하는 컴포넌트 (사용자 지정 컴포넌트)
	SCRIPT,  // 게임오브젝트가 여러개의 스크립트를 가질 수 있도록 하기 위해 END 뒤에 배치 // 앞에 배치하면 하나밖에 못가지니까
};

enum class DIR
{
	RIGHT,
	UP,
	FRONT,

	END,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_END,
};

enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
};


enum class DBG_SHAPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
};

enum class TASK_TYPE
{
	CREATE_OBJECT,
	DESTROY_OBJECT,
	CHANGE_LEVEL,
	CHANGE_LEVEL_STATE,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성	멀리서부터 오는 광원에게 설정, 주로 전역광원(빛이 월드로 오는 방향이 모두 같다고 본다)
	POINT,			// 점광원	기본적인 광원, 한 점으로부터 주변으로 빛이 뻗어나감
	SPOT,			// 스포트	특수한 광원, 빛을 특정 방향으로 모아서 보내는 것으로 봄
};


// 재질이 렌더링 되는 시점
enum class RENDER_DOMAIN
{
	DOMAIN_OPAQUE,		// 불투명
	DOMAIN_MASKED,		// 붙투명 + 완전 투명 /// discard 가 있는 셰이더
	DOMAIN_TRANSPARENT, // 반투명 
	DOMAIN_POSTPROCESS, // 후처리 - 가장 마지막에 동작, 기존에 그려진 장면을 재가공


	DOMAIN_DEBUG,		// 디버그 렌더링

	DOMAIN_NONE,		// 미지정
};

// 플레이어의 상태
enum class PLAYER_STATE_ID
{
	RUN,
	JUMP,
	DOUBLE_JUMP,
	LAND,
	SLIDE,
	HIT,
	DEAD,

	END,
};


enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};


enum class EObjectID
{
    None			= 0,

	DefaultJelly	= 1001,
	Coin1			= 1002,
	Coin2			= 1003,

	BearYellow		= 1004,
	BearPink		= 1005,
	BearBlue		= 1006,

	BearBigYellow	= 1007,
	BearRainbow		= 1008,

	SpecialBonus_1	= 1009,
	SpecialBonus_2	= 1010,

	Item,

	// 장애물
	Obstacle_bl1	= 2001,
	Obstacle_cb2	= 2002,
    Obstacle_tm001	= 2003,
 Obstacle_jp1down	= 2004,
	Obstacle_tm001_sdswing	= 2005,

	END,
};

// EObjectID 이름 변환 헬퍼 함수
inline const char* EObjectIDToString(EObjectID _ID)
{
	switch (_ID)
	{
	case EObjectID::None:			return "None";
	case EObjectID::DefaultJelly:	return "DefaultJelly";
	case EObjectID::Coin1:			return "Coin1";
	case EObjectID::Coin2:			return "Coin2";
	case EObjectID::BearYellow:		return "BearYellow";
	case EObjectID::BearPink:		return "BearPink";
	case EObjectID::BearBlue:		return "BearBlue";
	case EObjectID::BearBigYellow:	return "BearBigYellow";
	case EObjectID::BearRainbow:	return "BearRainbow";
	case EObjectID::SpecialBonus_1:	return "SpecialBonus_1";
	case EObjectID::SpecialBonus_2:	return "SpecialBonus_2";
	case EObjectID::Item:			return "Item";
	case EObjectID::Obstacle_bl1:	return "Obstacle_bl1";
	case EObjectID::Obstacle_cb2:	return "Obstacle_cb2";
    case EObjectID::Obstacle_tm001:	return "Obstacle_tm001";
	case EObjectID::Obstacle_jp1down: return "Obstacle_jp1down";
	case EObjectID::Obstacle_tm001_sdswing: return "Obstacle_tm001_sdswing";
	default:						return "Unknown";
	}
}

// 콤보박스용 EObjectID 배열 및 개수
constexpr int g_ObjectIDCount = 17;

inline EObjectID GetObjectIDByIndex(int _Idx)
{
	static const EObjectID arr[] = 
	{
		EObjectID::None,
		EObjectID::DefaultJelly,
		EObjectID::Coin1,
		EObjectID::Coin2,
		EObjectID::BearYellow,
		EObjectID::BearPink,
		EObjectID::BearBlue,
		EObjectID::BearBigYellow,
		EObjectID::BearRainbow,
		EObjectID::SpecialBonus_1,
		EObjectID::SpecialBonus_2,
		EObjectID::Item,
		EObjectID::Obstacle_bl1,
		EObjectID::Obstacle_cb2,
        EObjectID::Obstacle_tm001,
		EObjectID::Obstacle_jp1down,
		EObjectID::Obstacle_tm001_sdswing,
	};

	if (_Idx < 0 || _Idx >= g_ObjectIDCount)
		return EObjectID::None;

	return arr[_Idx];
}
