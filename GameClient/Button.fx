#ifndef _BUTTON_FX
#define _BUTTON_FX

#include "value.fx"

#define TintColor g_vec4_0;

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


VS_OUT VS_Button(VS_IN _input)
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

float4 SampleButtonTex(float2 _uv)
{
    if (g_int_0 == 1)
    {
        if (g_btex_1)
            return g_tex_1.Sample(g_sam_1, _uv);
    }
    
    if (g_btex_0)
        return g_tex_0.Sample(g_sam_1, _uv);
    
    return float4(1.f, 0.f, 1.f, 1.f);
}

float4 PS_Button(VS_OUT _input) : SV_Target
{
    float4 vColor = SampleButtonTex(_input.vUV);
    
    vColor *= TintColor;
    vColor *= _input.vColor;
    
    if (vColor.a == 0.f)
    {
        discard;
    }

    return vColor;
}


#endif