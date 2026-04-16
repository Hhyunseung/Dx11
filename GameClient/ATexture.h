#pragma once
#include "Asset.h"
class ATexture :
    public Asset
{
private:
	/// 라이브러리에서 제공하는 클래스
	ScratchImage						m_Image; // Content 폴더에 있는 이미지 파일을 메모리(System)로 불러드림

	/// 폴더에서 메모리(시스템)으로 올라온 이미지 데이터를 GPU 메모리로 올려야 한다
	/// 여기서 사용하는 버퍼는 ID3D11Texture2D
	ComPtr<ID3D11Texture2D>				m_Tex2D; // SystemMem 로 로딩한 픽셀 데이터를 GPU 메모리로 전송

	D3D11_TEXTURE2D_DESC				m_Desc;  // Texture2D의 속성 정보

	/// Tex2D 의 용도에 맞게 뷰를 생성
	// 텍스쳐의 용도에 맞는 View 들 
	ComPtr<ID3D11RenderTargetView>		m_RTV;
	ComPtr<ID3D11DepthStencilView>		m_DSV;
	ComPtr<ID3D11ShaderResourceView>	m_SRV;

	// u 레지스터 : ComputeShdaer 시점에만 사용 가능한 레지스터
	// UnorderedAcessView : u 레지스터에 바인딩 할때 사용하는 View
	ComPtr<ID3D11UnorderedAccessView>		m_UAV;

	/// 최근에 몇 번 바인딩 되었는지 체크
	int									m_RecentNum;
	int 								m_RecentSRV_CS;
	int 								m_RecentUAV_CS;	

public:
	void Binding(UINT _RegisterNum);
	void Clear();

	// ComputeShader 시점, t 레지스터 바인딩
	void Binding_CS_SRV(UINT _RegisterNum);

	// ComputeShader 시점, u 레지스터 바인딩
	void Binding_CS_UAV(UINT _RegisterNum);

	// ComputeShader 시점, t 레지스터 클리어
	void Clear_CS_SRV(int _RegisterNum = -1);

	// ComputeShader 시점, u 레지스터 클리어
	void Clear_CS_UAV();


	float GetWidth() { return m_Desc.Width; }
	float GetHeight() { return m_Desc.Height; }

	ComPtr< ID3D11Texture2D>		 GetTex2D() { return m_Tex2D; }
	ComPtr<ID3D11RenderTargetView>	 GetRTV() { return m_RTV; }
	ComPtr<ID3D11DepthStencilView>	 GetDSV() { return m_DSV; }
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }

public:
	int Create(UINT _Width, UINT _Height, DXGI_FORMAT _format, UINT _Flag, D3D11_USAGE _usage);
	virtual int Load(const wstring& _strFilePath);

public:
	ATexture();
	virtual ~ATexture();
};

