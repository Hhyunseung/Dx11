#pragma once

// 추상클래스 ... 로 만들 계획
class Entity
{
private:
	static UINT g_NextID;	// 다음에 부여할 고유 ID 값

private:
	const UINT	m_InstID;	// 객체마다 가지는 고유 ID 값
	wstring		m_Name;		// 객체마다 이름을 지정할 수 있다
	int			m_RefCount; // 참조 카운팅

public:
	void AddRef() { ++m_RefCount; }
	void Release() { --m_RefCount; if (m_RefCount <= 0) delete this; }

public:
	UINT GetID() { return m_InstID; }
	void SetName(const wstring& _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }

public:
	Entity();
	Entity(const Entity& _Other); // ID 가 겹지기 않게 하기 위해서 직접 복사 생성자 구현
	// Entity(const Entity&& _Other) = delete; // 복사 생성자 삭제 해도됨
	// const 멤벼변수가 있으면 대입 연산자는 삭제되어 만들 필요 없다
	virtual ~Entity(); // 가상 소멸자 // 포인터가 가리키고 있는 곳으로 가서 거기에 등록되어있는 소멸자 호출

	template<typename T>
	friend class Ptr;
};

