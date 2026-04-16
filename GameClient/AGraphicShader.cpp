#include "pch.h"
#include "AGraphicShader.h"

#include "Device.h"
#include "PathMgr.h"


AGraphicShader::AGraphicShader()
	: Asset(ASSET_TYPE::GRAPHICSHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(RS_TYPE::CULL_BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
{

}

AGraphicShader::~AGraphicShader()
{
}

int AGraphicShader::CreateVertexShader(const wstring& _RelativeFilePath, const string& _FuncName)
{
	// vector<wchar_t> vecPath; 와 비슷한 개념..
	// 컨테이너
	wstring Path = PathMgr::GetInst()->GetContentPath();
	Path += _RelativeFilePath; // 상대경로

	ComPtr<ID3DBlob> Eff;

	if (FAILED(D3DCompileFromFile(Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "vs_5_0", 0, D3DCOMPILE_DEBUG
		, m_VSBlob.GetAddressOf(), Eff.GetAddressOf())))
	{
		const char* pErrmsg = (const char*)Eff->GetBufferPointer();
		MessageBoxA(nullptr, pErrmsg, "쉐이더 생성 실패", MB_OK);
		return E_FAIL;
	}

	// 컴파일된 어셈블리 코드로 VertexShader 객체 생성
	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 0	  12	 20
	// v	   v	 v
	// ---12--- --8-- ---16---
	// Position|  UV |  Color |
	// -----------------------
	// Input Layout 생성하기
	D3D11_INPUT_ELEMENT_DESC InputDesc[3] = {};


	InputDesc[0].SemanticName = "POSITION";
	InputDesc[0].SemanticIndex = 0;						   // 시멘틱 이름이 중복된 경우, 구별하기 위한 숫자
	InputDesc[0].AlignedByteOffset = 0;						   // 메모리 시작 위치(오프셋)
	InputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 오프셋으로부터 크기
	InputDesc[0].InputSlot = 0;						   // 설명하는 정점이 들어있는 버퍼의 위치
	InputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 설명하는 정점이 들어있는 버퍼의 위치
	InputDesc[0].InstanceDataStepRate = 0;						   // 인스턴스당 증가하는 오프셋(인스턴싱에 사용)

	InputDesc[1].SemanticName = "TEXCOORD";
	InputDesc[1].SemanticIndex = 0;						   // 시멘틱 이름이 중복된 경우, 구별하기 위한 숫자
	InputDesc[1].AlignedByteOffset = 12;						   // 메모리 시작 위치(오프셋)
	InputDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 오프셋으로부터 크기
	InputDesc[1].InputSlot = 0;						   // 설명하는 정점이 들어있는 버퍼의 위치
	InputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 설명하는 정점이 들어있는 버퍼의 위치
	InputDesc[1].InstanceDataStepRate = 0;						   // 인스턴스당 증가하는 오프셋(인스턴싱에 사용)

	InputDesc[2].SemanticName = "COLOR";
	InputDesc[2].SemanticIndex = 0;						   // 시멘틱 이름이 중복된 경우, 구별하기 위한 숫자
	InputDesc[2].AlignedByteOffset = 20;						   // 메모리 시작 위치(오프셋)
	InputDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 오프셋으로부터 크기
	InputDesc[2].InputSlot = 0;						   // 설명하는 정점이 들어있는 버퍼의 위치
	InputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 설명하는 정점이 들어있는 버퍼의 위치
	InputDesc[2].InstanceDataStepRate = 0;						   // 인스턴스당 증가하는 오프셋(인스턴싱에 사용)

	if (FAILED(DEVICE->CreateInputLayout(InputDesc, 3, m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize()
		, m_Layout.GetAddressOf())))
	{
		return E_FAIL;
	}

	return 0;
}

int AGraphicShader::CrateGeometryShader(const wstring& _RelativeFilePath, const string& _FuncName)
{
	wstring Path = PathMgr::GetInst()->GetContentPath() + _RelativeFilePath;

	// PixelShader
	ComPtr<ID3DBlob> Err;
	if (FAILED(D3DCompileFromFile(Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "gs_5_0", D3DCOMPILE_DEBUG, 0
		, m_GSBlob.GetAddressOf(), Err.GetAddressOf())))
	{
		const char* pErrMsg = (const char*)Err->GetBufferPointer();
		MessageBoxA(nullptr, pErrMsg, "쉐이더 생성 실패", MB_OK);
		return E_FAIL;
	}

	// 컴파일한 Shader 코드로 PixelShader 만들기
	if (FAILED(DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer()
		, m_GSBlob->GetBufferSize(), nullptr
		, m_GS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int AGraphicShader::CreatePixelShader(const wstring& _RelativeFilePath, const string& _FuncName)
{
	wstring Path = PathMgr::GetInst()->GetContentPath() + _RelativeFilePath;

	// PixelShader 
	ComPtr<ID3DBlob> Eff;

	if (FAILED(D3DCompileFromFile(Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "ps_5_0", 0, D3DCOMPILE_DEBUG
		, m_PSBlob.GetAddressOf(), Eff.GetAddressOf())))
	{
		const char* pErrmsg = (const char*)Eff->GetBufferPointer();
		MessageBoxA(nullptr, pErrmsg, "쉐이더 생성 실패", MB_OK);
		return E_FAIL;
	}

	// 컴파일된 어셈블리 코드로 PixelShader 객체 생성
	if (FAILED(DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer()
		, m_PSBlob->GetBufferSize()
		, nullptr, m_PS.GetAddressOf())))
	{
		return E_FAIL;
	}


	return 0;
}

void AGraphicShader::Binding()
{
	// Graphic Pipeline
	// IA (Input Assembler)
	// 렌더링에 필요한 리소스 입력

	// 정점으로 구성할 도형의 상태, 모양
	// TriangleList : 점 3개를 이어서 만든 삼각형(내부까지 색을 채움)
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	// 정점 하나 안에 들어있는 데이터를 구분해주는 정보
	/// 그걸 기반으로 정점 데이터 중 필요한 정보(일부분)만 뽑아올 수 있음 ex) Color 만 받아오기..
	CONTEXT->IASetInputLayout(m_Layout.Get());

	// Vertex Shader(함수) - 정점 당 연산 수행
	// HLSL (High Level Shader Language) 5.0
	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);

	// Geometry Shader(함수) - 정점 당 수행, 정점의 개수를 변경할 수 있음
	// HLSL(High Level Shader Language) 5.0
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);

	// Resterizer State
	// 정점 셰이더에서 반환한 정점의 위치를 NDC 좌표 기준으로 기록
	// 정점 내부에 들어오는 픽셀을 계산해서 픽셀 셰이더로 연계
	CONTEXT->RSSetState(Device::GetInst()->GetRSState(m_RSType).Get());

	// Pixel Shader
	// 레스터라이저를 거쳐서 호출될 픽셀마다 실행되는 함수
	// 픽셀 셰이더에서 리턴한 값이 렌더타겟 내에서 해당 픽셀 위치에 색상이 출력된다
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	// (OM) Output Merge State 
	// 픽셀 셰이더에서 리턴한 값을 OM 단계에 연결되어있는 RenderTarget, DepthStencilTarget 에 기록한다
	// DepthStencilState - 깊이 비교 /// 깊이를 비교해서 출력할지 말지 결정

	CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(m_DSType).Get(), 0);

	// 블랜드스테이트 바인딩
	ComPtr<ID3D11BlendState> pBSState = Device::GetInst()->GetBSState(m_BSType);
	CONTEXT->OMSetBlendState(pBSState.Get(), nullptr, 0xffffffff);
}
