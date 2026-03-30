#pragma once
#include "CRenderComponent.h"

#include "AFlipbook.h"

class CFlipbookRender :
	public CRenderComponent
{
private:
	vector<Ptr<AFlipbook>>    m_vecFlipbook;

	int						  m_CurFlipbook;
	int						  m_CurSprite;

	int						  m_RepeatCount; // -1 : 반복재생, 1 이상이면 재생 횟수
	bool 					  m_Finish;
	float 					  m_FPS;
	float 					  m_AccTime;


public:
	void SetFlipbook(int _Idx, Ptr<AFlipbook> _Flipbook)
	{
		if (m_vecFlipbook.size() <= _Idx)
			m_vecFlipbook.resize(_Idx + 1);
		m_vecFlipbook[_Idx] = _Flipbook;
	}
	vector<Ptr<AFlipbook>>& GetFlipbooks() { return m_vecFlipbook; }

	void AddFlipbook(Ptr<AFlipbook> _Flipbook)
	{
		for (size_t i = 0; i < m_vecFlipbook.size(); ++i)
		{
			if (m_vecFlipbook[i] == nullptr)
			{
				m_vecFlipbook[i] = _Flipbook;
				return;
			}
		}

		m_vecFlipbook.push_back(_Flipbook);
	}

	void Play(int _FlipbookIdx, float _FPS, int _RepeatCount)
	{
		m_CurSprite = 0; /// 배열 초기화
		m_CurFlipbook = _FlipbookIdx;
		m_RepeatCount = _RepeatCount;
		m_FPS = _FPS;
		m_AccTime = 0.f;
	}

	int GetCurFlipbook() const { return m_CurFlipbook; }
	int GetCurSprite() const { return m_CurSprite; }

	bool IsAnimationComplete() const { return m_Finish && m_RepeatCount == 0; }


///  UV 값의 정수부분을 버림 frac

private:
	bool CheckFinish();

public:
	virtual void FinalTick() override;
	virtual void Render() override;
	virtual void CreateMaterial() override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CFlipbookRender);

public:
	CFlipbookRender();
	virtual ~CFlipbookRender();

};

