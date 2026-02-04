/// _Test 가 정의되지 않았을 때만 컴파일
#ifndef _TEST
/// _TEST 정의
/// 이후 #ifndef _TEST 를 만나면 false 가 되어 블록이 컴파일되지 않음
#define _TEST

#include "value.fx"


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
};


VS_OUT VS_Std2D(VS_IN _input)
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
    output.vColor    = _input.vColor;
    
    return output;
}

//VS_OUT VS_Test1(VS_IN _input)
//{
//    VS_OUT output = (VS_OUT) 0.f;
    
//    // Local->World
//    //float4 vWorld = mul(float4(_input.vPos, 1.f), g_matWorld);
//    float4 vWorld = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWorld);
    
//    // World->View
//    float4 vView = mul(vWorld, g_matView);
    
//    // 빌보드 효과
//    // 월드에 배치된 오브젝트의 중심점을 뷰로 옮겨서 입력된 Pos 
//    vView.xyz += _input.vPos * 100.f; // 오브젝트의 100.f 는 배율정보 // 화면에 고정된 크기
    
//    // View -> Projection
//    float4 vProj = mul(vView, g_matProj);

//    output.vPosition = vProj;
//    output.vUV = _input.vUV;
//    output.vColor = _input.vColor;
    
//    return output;
//}


// 입력된 텍스쳐를 사용해서 픽셀셰이더의 출력 색상으로 지정한다
float4 PS_Std2D(VS_OUT _input) : SV_Target
{
    // 샘플링
    // 입력 UV 는 정점에서 반환한 값을 보간받아서 픽셀셰이더에 입력됨
    float4 vColor = g_tex_0.Sample(g_sam_1, _input.vUV);
    
    // 알파 채널이 있는 png 텍스쳐라서 alpha 값 추가
    /// 윈도우 비트맵(텍스쳐)에는 알파가 없어서 1.0f 로 들어온다
    if (vColor.a == 0.f || vColor.r < 0.01f && vColor.g < 0.01f && vColor.b < 0.01f)
    {
        /// 해당 픽셀은 출력하지 않음 (알파 테스트)
        /// 픽셀이 버려지고 깊이값도 기록되지 않음
        discard;
    }
    
    if (g_int_0 == 1)
    {
        vColor.r *= 2.f;
    }
    
        
    return vColor;

}


#endif