#ifndef _VALUE
#define _VALUE


SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);

//
cbuffer TRANSFORM : register(b0)
{
    // matrix 는 float4x4
    row_major matrix g_matWorld; // Local(Model) -> World Space
    row_major matrix g_matView; // World -> Camera(View) Space
    row_major matrix g_matProj; // Camera(View) -> Projection Space
}

/// 상수 레지스터는 자료형의 최소사이즈가 16byte (4 float) 여야 한다
/// 배열로 가져오면 

// HLSL 5.0 : 최대 4096byte (256 float4) 까지 가능
cbuffer MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0; // billboard scale 
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
}

/// t0 에 바인딩된 텍스쳐가 512 512 라면 UV 좌표계로 샘플링
/// 입력으로 들어오는 UV 좌표계를 이용해서 보간된 텍스쳐 색상을 출력

/// 텍스쳐 레지스터
/// 포인터를 사용하는 개념과 유사
/// 가변적으로 텍스쳐를 바인딩 할 수 있다
/// 가변적인 데이터가 필요한 경우... 구조화버퍼로 여기에 저장
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);


#endif