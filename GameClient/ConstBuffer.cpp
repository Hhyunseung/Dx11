#include "pch.h"
#include "ConstBuffer.h"

#include "Device.h"


ConstBuffer::ConstBuffer()
	: m_Desc{}
	, m_Type(CB_TYPE::END)
{
}

ConstBuffer::~ConstBuffer()
{
}

int ConstBuffer::Create(CB_TYPE _Type, UINT _Size)
{
	m_Type = _Type;

	// 버퍼의 크기 /// 16 의 배수로 크기를 잡아야 함
	m_Desc.ByteWidth = _Size;

	// CPU 를 통해서 버퍼의 내용을 쓰거나, 읽을 수 있는지
	// D3D11_USAGE_DEFAULT + 0
	// 버퍼를 생성한 이후에 수정할 수 없다
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 버퍼 용도
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// ?
	// 처음 버퍼 생성할 때 전달시킬 데이터의 시작주소를 Sub 구조체에 담아서 CreateBuffer 함수에 넣어준다
	//D3D11_SUBRESOURCE_DATA tSub = {};
	//tSub.pSysMem = arrVtx;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, /*&tSub*/nullptr, m_CB.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

void ConstBuffer::SetData(void* _SysMem, UINT _Size)
{
	if (_Size == 0)
		_Size = m_Desc.ByteWidth;

	D3D11_MAPPED_SUBRESOURCE tMapSub = {};
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
	memcpy(tMapSub.pData, _SysMem, _Size);
	CONTEXT->Unmap(m_CB.Get(), 0);
}

void ConstBuffer::Binding()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_Type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers((UINT)m_Type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers((UINT)m_Type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers((UINT)m_Type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers((UINT)m_Type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_CB.GetAddressOf());
}

void ConstBuffer::Binding_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}

