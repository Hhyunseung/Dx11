#include "pch.h"
#include "ATexture.h"

#include "Device.h"

ATexture::ATexture()
	: Asset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
	, m_RecentNum(-1) // 세팅된 적이 없는 상태
{
}

ATexture::~ATexture()
{
}

void ATexture::Binding(UINT _RegisterNum)
{
	m_RecentNum = _RegisterNum;

	// 어느 파이프라인 단계인지 상관없이 바인딩
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void ATexture::Clear()
{
	if (-1 == m_RecentNum)
		return;

	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(m_RecentNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(m_RecentNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(m_RecentNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(m_RecentNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(m_RecentNum, 1, &pSRV);
}

int ATexture::Load(const wstring& _strFilePath)
{
	// _strFilePath.c_str() // 문자열 시작 주소
	//wchar_t szDrive[10] = {}; // 드라이브 경로 받을 배열
	//wchar_t szFolder[255] = {}; // 폴더 경로 받을 배열
	//wchar_t szFileName[50] = {}; // 파일 이름 받을 배열
	//wchar_t szExt[10] = {}; // 확장자 받을 배열

	//_wsplitpath_s(_strFilePath.c_str(), szDrive, 10, szFolder, 255, szFileName, 50, szExt, 10);
	//
	// 1. system 메모리로 옮기기

	wchar_t szExt[10] = {}; // 확장자 받을 배열
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);
	wstring strExt = szExt;


	HRESULT hr = S_OK;
	if (L".dds" == strExt)
	{
		// .dds
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (L".tga" == strExt)
	{
		// .tga
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}

	else
	{
		// WIC(Window Image Component) .png, .jpg, .jpeg, .bmp
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"텍스쳐 시스템 메모리 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// 2. texture2D 생성하기
	// 렌더링 파이프라인으로 전달할 셰이더 리소스 뷰
	// 하나의 텍스쳐에는 여러개의 뷰가 나올 수 있다

	// Sysmem			-> GPU
	// ScratchImage		-> Texture2D
	// Texture2D 생성
	
	// Texture2D -> RTV
	//			 -> DSV
	//			 -> SRV
	// View 생성


	// ScratchImage 에 로딩된 이미지 데이터를 기반으로 Texture2D 를 생성하고,
	// 이걸로 ShaderResourceView 까지 만들어서 ShaderResourceView 주소를 알려줌
	if (FAILED(CreateShaderResourceView(DEVICE, m_Image.GetImages()
		, m_Image.GetImageCount(), m_Image.GetMetadata()
		, m_SRV.GetAddressOf())))
	{
		MessageBox(nullptr, L"텍스쳐 시스템 메모리 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// 생성된 SRV 를 이용해서, 먼저 만들어진 Texture2D 의 주소를 알아냄
	/// 역으로 주소를 받아서 Texture2D 주소가져옴
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// Texture2D 를 생성할 때 세팅한 Desc 옵션 정보를 알아냄
	m_Tex2D->GetDesc(&m_Desc);


	return S_OK;
}
