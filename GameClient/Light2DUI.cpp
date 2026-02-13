#include "pch.h"
#include "Light2DUI.h"

Light2DUI::Light2DUI()
	: ComponentUI(COMPONENT_TYPE::LIGHT2D, "Light2DUI")
{
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::Tick_UI()
{
	OutputTitle("Light2D");
}
