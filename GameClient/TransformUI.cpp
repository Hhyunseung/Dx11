#include "pch.h"
#include "TransformUI.h"

#include "GameObject.h"

TransformUI::TransformUI()
	: ComponentUI(COMPONENT_TYPE::TRANSFORM, "TransformUI")
{

}

TransformUI::~TransformUI()
{
}

void TransformUI::Tick_UI()
{
	OutputTitle("Transform");

	Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRot = GetTarget()->Transform()->GetRelativeRot();

	/// Vec3 의 오퍼레이터 발생... float 3개짜리 배열을 요구하므로 이에 맞춰 넘어가줌
	/// UI 의 값을 수정하면 true 를 리턴한다
	ImGui::Text("Position");
	ImGui::SameLine(150);
	if (ImGui::DragFloat3("##POSITION", vPos))
		GetTarget()->Transform()->SetRelativePos(vPos);

	ImGui::Text("Scale");
	ImGui::SameLine(150);
	if (ImGui::DragFloat3("##SCALE", vScale))
		GetTarget()->Transform()->SetRelativeScale(vScale);

	/// 라디안 표기법 변경
	ImGui::Text("Rotation");
	ImGui::SameLine(150);
	Vec3 vDegree = vRot * 180 / XM_PI;
	if (ImGui::DragFloat3("##ROTATION", vDegree))
	{
		vRot = vDegree * (XM_PI / 180.f);
		GetTarget()->Transform()->SetRelativeRot(vRot);
	}

	bool Independent = GetTarget()->Transform()->IsIndependentScale();

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Independent Scale");
	ImGui::SameLine();
	if (ImGui::Checkbox("##Independent", &Independent))
	{
		GetTarget()->Transform()->SetIndependentScale(Independent);
	}
}