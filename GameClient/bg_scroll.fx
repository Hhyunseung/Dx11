/// _Test 가 정의되지 않았을 때만 컴파일
#ifndef _TEST
#define _TEST

#include "value.fx"

#define ScrollUV g_vec2_0

struct VS_IN
{
    float3 vPos : POSITION; // Semantic : Layout 에서 설명한 이름 // 이름 중첩된 경우.. POSITION0, POSITION1 ... 
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION; // 래스터라이저로 보낼때, NDC 좌표
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
    float3 vWorld : POSITION;
};


VS_OUT VS_BGScroll2D(VS_IN _input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);

    output.vPosition = vProj;
    output.vUV = _input.vUV;
    output.vColor = _input.vColor;
    output.vWorld = vWorld;
    
    return output;
}


// 입력된 텍스쳐를 사용해서 픽셀셰이더의 출력 색상으로 지정한다
float4 PS_BGScroll2D(VS_OUT _input) : SV_Target
{
    float2 vUV = _input.vUV + ScrollUV;
    
    vUV = frac(vUV);
    
    float4 vColor = g_tex_0.Sample(g_sam_1, vUV);
    
    if (vColor.a == 0.f)
    {
        discard;
    }

    // 물체가 받는 빛의 총량
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < Light2DCount; ++i)
    {
        LightColor += CalcLight2D(i, _input.vWorld);
    }
    
    // 물체의 색상에, 자신이 받는 최종빛 총량을 곱한다
    vColor.rgb *= LightColor;
        
    return vColor;
}


#endif