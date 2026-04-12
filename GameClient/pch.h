#pragma once

// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Directx11 객체들을 가리킬 수 있는 스마트포인터
#include "wrl.h"
using namespace Microsoft::WRL;


// Directx11 라이브러리 (정적, 동적)
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#include "SimpleMath.h"
typedef SimpleMath::Vector2 Vec2;
typedef SimpleMath::Vector3 Vec3;
typedef SimpleMath::Vector4 Vec4;
typedef SimpleMath::Matrix Matrix;

/// 라이브러리.. 오브젝트 파일로 어느정도 컴파일된 상태
// DLL (동적 라이브러리)
#pragma comment(lib, "d3d11") // 헤더파일들에 대응하는 라이브러리 링크
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")


/// 외부에서 직접 추가한 
// DirectTex Library
#include <DirectxTex\\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectxTex\\DirectxTex_debug.lib") // 디버그버전 라이브러리
#else
#pragma comment(lib, "DirectxTex\\DirectxTex.lib") // 릴리즈버전 라이브러리
#endif



// C++ 타입 정보기능
/// 가상 함수 테이블에 대한 정보를 가져올 수 있음
#include <typeinfo>

// 정적 라이브러리
// 컴파일때 .lib 파일을 링크(포함)후 실행파일(.exe) 생성
// 실행중인 exe 파일들에 중복된 코드가 존재하게 됨

// 동적 라이브러리
// 컴파일된 .dll 파일을 로딩해서 사용하는 방식
// 컴파일 때는 .lib 파일을 링크(.dll의 함수 위치 정보만 포함)  /// #pragma comment(lib, "d3d11")
// 런타임때 동적 라이브러리(.dll) 파일을 동적 바인딩(로딩)
// 여러개의 exe 파일들이 동일한 코드를 공유해서 사용
// -> 자기쪽 함수에서 객체를 사용하면 delete 하는 함수도 같이 적어두어야함
// new 와 delete 가 다른 DLL 에서 호출되면 문제가 발생할 수 있으니까 /// 버전이 다를 수도 있고..
// 
 
// STL 자료구조
#include <vector>
#include <list>
#include <map>
#include <string>
using namespace std;

// FormatWithComma 
#include <sstream>
#include <locale>

// FMOD
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

// 상대경로 " "
#include "Resource.h"
#include "global.h"
#include "Ptr.h"