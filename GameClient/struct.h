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


struct MtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	mat[2];
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