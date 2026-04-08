#pragma once


// Task
void CreateObject(class GameObject* _Object, int LayerIdx);
void ChangeLevel(const wstring& _NextLevelName);
void ChangeLevelState(LEVEL_STATE _NextState);


// DebugRebder
void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Duration, bool _DeapthTest = false);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DeapthTest = false);

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Duration, bool _DeapthTest = false);

void SaveWString(FILE* _FIle, const wstring& _String);
wstring LoadWString(FILE* _File);

void SaveAssetRef(FILE* _File, class Asset* _Asset);

void CreateTestLevel();

// 0 ~ 1 사이의 값으로 만듦
float Saturate(float _Data);

// 
wstring FormatWithComma(long long value);

