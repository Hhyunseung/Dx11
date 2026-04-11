#ifndef _HITEFFECT_FX
#define _HITEFFECT_FX

#include "value.fx"

// g_float_0 : 현재 알파값 (0 = 완전 투명, 1 = 완전 표시)
#define Alpha g_float_0

struct VS_IN
{
    float3 vPos   : POSITION;
    float2 vUV    : TEXCOORD;
    float4 vColor : COLOR;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vUV       : TEXCOORD;
};

VS_OUT VS_HitEffect(VS_IN _input)
{
    VS_OUT output = (VS_OUT) 0.f;

    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vView  = mul(vWorld, g_matView);
    float4 vProj  = mul(vView,  g_matProj);

    output.vPosition = vProj;
    output.vUV       = _input.vUV;

    return output;
}

float4 PS_HitEffect(VS_OUT _input) : SV_Target
{
    if (Alpha <= 0.01f)
        discard;

    // UV (0,0)~(1,1) 기준으로 사각형 테두리 vignette 계산
    // 중앙 = 0, 가장자리 = 1
    float2 edge     = abs(_input.vUV - 0.5f) * 2.0f;
    float  vignette = max(edge.x, edge.y);
    vignette        = pow(vignette, 4.0f); // 커브: 중앙 투명, 테두리만 진하게

    float finalAlpha = vignette * Alpha;

    if (finalAlpha < 0.01f)
        discard;

    return float4(1.f, 0.f, 0.f, finalAlpha);
}

#endif
