#pragma once
#include <string>

class GameDataMgr
	: public singleton<GameDataMgr>
{
	SINGLE(GameDataMgr);

private:
	int m_TotalCoin;	// 총 코인 수 (게임 전체에서 누적되는 값)
	int m_BestScore;	// 최고 점수 (게임 전체에서 누적되는 값)

	wstring m_SelectedCharacter;	// 현재 선택된 캐릭터 이름
	wstring m_SelectedStage;		// 현재 선택된 스테이지 이름

public:
	void Init();
	void Clear();

	// Coin
	int GetTotalCoin() const { return m_TotalCoin; }
	void SetTotalCoin(int _TotalCoin) { m_TotalCoin = _TotalCoin; }

	void AddTotalCoin(int _Amount);
	bool UseCoin(int _Amount);	// 코인 사용 (사용 가능 여부 반환)

	// Score
	int GetBestScore() const { return m_BestScore; }
	void SetBestScore(int _BestScore) { m_BestScore = _BestScore; }
	void UpdateBestScore(int _Score);	// 최고 점수 업데이트 (새 점수가 더 높을 때만)

	// Selected Character
	void SetSelectedCharacter(const wstring& _Character) { m_SelectedCharacter = _Character; }
	const wstring& GetSelectedCharacter() const { return m_SelectedCharacter; }

	// Selected Stage
	void SetSelectedStage(const wstring& _Stage) { m_SelectedStage = _Stage; }
	const wstring& GetSelectedStage() const { return m_SelectedStage; }

public:
	bool Save();
	bool Load();
};

