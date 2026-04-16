#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"


// 읽기 쓰기가 동시에 가능한 레지스터
RWTexture2D<float4> g_Target : register(u0);


// 스레드 개수 지정, 스레드 그룹, HLSL 5.0 기준 최대 1024개
[numthreads(1024, 1, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)
{
    
}


#endif