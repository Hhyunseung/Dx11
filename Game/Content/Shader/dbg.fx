#ifndef _DEBUG_RENDER
#define _DEBUG_RENDER

#include "value.fx"

#define COLOR g_vec4_0

struct VS_IN
{
    float3 vPos : POSITION; // Semantic : Layout 에서 설명한 이름 // 이름 중첩된 경우.. POSITION0, POSITION1 ... 
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION; // 래스터라이저로 보낼때, NDC 좌표
    float2 vUV : TEXCOORD;
};


VS_OUT VS_Debug(VS_IN _input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Local->World
    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    
    // World->View
    float4 vView = mul(vWorld, g_matView);
    
    // View -> Projection
    float4 vProj = mul(vView, g_matProj);

    output.vPosition = vProj;
    output.vUV       = _input.vUV;
    
    return output;
}

// 입력된 텍스쳐를 사용해서 픽셀셰이더의 출력 색상으로 지정한다
float4 PS_Debug(VS_OUT _input) : SV_Target
{
    return COLOR;
}


#endif