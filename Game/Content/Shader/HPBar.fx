#ifndef _HPBAR_FX
#define _HPBAR_FX

#include "value.fx"

#define HPRatio g_float_0

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
};

VS_OUT VS_HPBar(VS_IN _input)
{
    VS_OUT output = (VS_OUT) 0.f;

    float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);

    output.vPosition = vProj;
    output.vUV = _input.vUV;
    output.vColor = _input.vColor;

    return output;
}

float4 PS_HPBar(VS_OUT _input) : SV_Target
{
    if (!g_btex_0)
        return float4(1.f, 1.f, 1.f, 1.f);

    // 오른쪽 잘라내기
    if (_input.vUV.x > HPRatio)
        discard;

    float4 vColor = g_tex_0.Sample(g_sam_0, _input.vUV);

    if (vColor.a == 0.f)
        discard;

    return vColor;
}

#endif