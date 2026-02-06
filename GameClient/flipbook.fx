#ifndef _FLIPBOOK
#define _FLIPBOOK

#include "value.fx"

#define AtlasTex    g_tex_0
#define LeftTopUV   g_vec2_0
#define SliceUV     g_vec2_1


struct VS_IN
{
    float3 vPos : POSITION; // Sementic : Layout 에서 설명한 이름       
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position; // 래스터라이져로 보낼때, NDC 좌표
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Flipbook(VS_IN _input)
{
    VS_OUT output = (VS_OUT) 0.f;
             
    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
     
    output.vPosition = vProj;
    output.vUV = _input.vUV;
    
    return output;
}

// 입력된 텍스쳐를 사용해서 픽셀쉐이더의 출력 색상으로 지정한다.
float4 PS_Flipbook(VS_OUT _input) : SV_Target
{
    float2 vSpriteUV = _input.vUV * SliceUV + LeftTopUV;
    float4 vColor = AtlasTex.Sample(g_sam_1, vSpriteUV);
    
    if (vColor.a == 0.f)
        discard;
    
    return vColor;
}


#endif