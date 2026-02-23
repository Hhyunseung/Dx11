#pragma once

struct Vertex
{
	Vec3		vPos;	// 3D 공간상에서 정점의 위치
	Vec2		vUV;	// 정점 위에 이미지를 띄울 경우, 이미지의 어느 지점을 참고하는지 좌표
	Vec4		vColor; // 정점 자체의 색상

	/*
	UV 좌표계
	(0,0)---------> (1,0)
	 |
	 |
	 |
	 |
	 v
	(0,1)			(1,1)
	

	*/

};
typedef Vertex Vtx;


struct TransformMatrix
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
};

// extern, 전방 선언, 유일한 전역변수, 모든 파일(cpp) 에서 접근 가능
// extern 들의 구현만 따로 하는 extern.cpp 생성
extern TransformMatrix g_Trans;

// Light2D 정보
struct Light2DInfo
{
	LIGHT_TYPE	Type;
	Vec3		Color;		// 빛의 색상
	Vec3		Ambient;	// 환경광, 광원이 존재하면서 최소한으로 발생하는 빛의 세기
	Vec3 		LightDir;   // 광원의 빛이 향하는 방향
	Vec3        WorldPos;   // 광원의 위치 (포인트, 스포트)
	float       Radius;     // 빛의 영향 반경 (포인트, 스포트)
	float		Angle;		// SpotLight 범위 각
};


struct MtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	mat[2];

	int		IsTex[(UINT)TEX_PARAM::TEX_END]; /// bool 자료형을 쓸 수 없어서 int 배열로 텍스처가 들어가는지 여부를 표현
	int		Padding[2]; /// 16 바이트 정렬을 위한 패딩
};

// DebugInfo : 디버그 렌더링 요청사항 정보
struct DbgInfo
{
	DBG_SHAPE	Shape;
	Vec3 		Pos;
	Vec3		Scale;
	Vec3		Rotation;

	Matrix 		matWorld;

	Vec4		Color;
	float		Age;		// 현재 수명
	float		Life;		// 최대 수명

	bool		DepthTest;
};

// TaskMgr 가 처리할 작업 단위
struct TaskInfo
{
	TASK_TYPE Type;
	DWORD_PTR Param_0; // DWORD_PTR 8 바이트 정수 /// 플랫폼 비트에 따라서 정수 크기 달라짐 // 주소를 담기 위해
	DWORD_PTR Param_1;
	DWORD_PTR Param_2;
};

struct GlobalData
{
	Vec2	Resolution;		// 화면 해상도
	int     Light2DCount;	// 2D 광원 개수
	int		Light3DCount;	// 3D 광원 개수
	float	DeltaTime;		// DeltaTime
	float	Time;			// 누적 시간 값
	float	EngineDT;		// DeltaTime (Engine 기준)
	float	EngineTime;		// 누적 시간 값 (Engine 기준)
};

extern GlobalData g_Global;