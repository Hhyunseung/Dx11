#pragma once

template <typename T>
class singleton
{
private:
	static T* m_This;

public:
	static T* GetInst()
	{
		if (m_This == nullptr)
		{
			m_This = new T;
		}

		return m_This;
	}

	static void Destroy()
	{
		if (m_This != nullptr)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

	// 생성자는 자식에서만 호출 가능하게
protected:
	singleton()
	{
		/// 함수포인터를 전달시켜서 프로그램이 종료될때 해당 함수가 호출되도록 등록
		atexit(Destroy);
	}
};

// 초기화
template <typename T>
T* singleton<T>::m_This = nullptr;