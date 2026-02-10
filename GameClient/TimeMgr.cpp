#include "pch.h"
#include "TimeMgr.h"
#include "Engine.h"

TimeMgr::TimeMgr()
	: m_Frequency{}
	, m_Prev{}
	, m_Current{}
	, m_DeltaTime(0.f)
	, m_Time(0.f)
{
}

TimeMgr::~TimeMgr()
{
}

void TimeMgr::Init()
{
	/// QueryPerformanceFrequency, QueryPerformanceCounter
	/// LARGE_INTEGER 를 인자로 받아서 멤버변수들을 LARGE_INTEGER 형으로 설정
	
	// 1 초동안 가능한 카운팅 횟수
	/// CPU 가 아니라 운영체제에서 제공하는 고해상도 타이머 
	QueryPerformanceFrequency(&m_Frequency);

	// 초기 카운트
	QueryPerformanceCounter(&m_Current);
	QueryPerformanceCounter(&m_Prev);
}

void TimeMgr::Tick()
{
	QueryPerformanceCounter(&m_Current);

	// 이전과 현재 카운팅 차이를 Frequency 로 나눠서 1 프레임동안 진행한 시간값을 구하기
	m_DeltaTime = (float)(m_Current.QuadPart - m_Prev.QuadPart) / (float)m_Frequency.QuadPart;

	// Prev 카운팅을 다시 현재카운팅으로 맞추기
	m_Prev = m_Current;

	// 누적 시간 계산
	m_Time += m_DeltaTime;

	// 1 초마다 if 수행
	if (1.f < m_Time)
	{
		wchar_t buff[255] = {};
		swprintf_s(buff, 255, L"DeltaTime : %f", m_DeltaTime);
		SetWindowText(Engine::GetInst()->GetMainWndHwnd(), buff);

		m_Time -= 1.f;
	}

	g_Global.DeltaTime = m_DeltaTime;
	g_Global.Time += m_DeltaTime;
	g_Global.EngineDT = m_DeltaTime;
	g_Global.EngineTime += m_DeltaTime;
}
