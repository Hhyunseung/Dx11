#pragma once
#include "Entity.h"

// Asset 위 모든 객체들이 상속받는 최상위 클래스 Entity
// Asset 들이 가지는 공통적인 속성 Asset class
// Asset 종류.. Mesh / Texture / Shader / Animation / Sound / .. 등등

class Asset
	: public Entity
{
private:
	wstring m_Key;			// 에셋을 찾을때 사용할 이름
	wstring m_RelativePath; // 상대 경로 (실제 Content 폴더에 저장되어있는 위치)

	// RTTI : RunTime Type Identification 
	// 런타임 도중에 객체의 실제 타입을 알아내는 기술
	/// 다형성을 이용하여 부모 클래스 타입으로 자식 클래스 객체를 가리킬 때
	/// 실제 자식 클래스 타입이 무엇인지 알아내는 기법
	const ASSET_TYPE m_Type; // 에셋 타입

public:
	ASSET_TYPE GetType() { return m_Type; }

public:
	const wstring& GetKey() { return m_Key; }
	const wstring& GetRelativePath() { return m_RelativePath; }

public:
	// Asset(); // 기본 생성자 제거, 자신이 어떤 타입의 Asset 인지 입력으로 받는 버전의 생성자만 남겨둠
	Asset(ASSET_TYPE _Type);
	virtual ~Asset();

};

