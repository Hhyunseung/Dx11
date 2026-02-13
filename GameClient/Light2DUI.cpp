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

    static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
    static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

    static bool ref_color = false;
    static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
    static int picker_mode = 0;
    static int display_mode = 0;
    static ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_AlphaBar;

    ImGui::CheckboxFlags("ImGuiColorEditFlags_NoAlpha", &color_picker_flags, ImGuiColorEditFlags_NoAlpha);
    ImGui::CheckboxFlags("ImGuiColorEditFlags_AlphaBar", &color_picker_flags, ImGuiColorEditFlags_AlphaBar);
    ImGui::CheckboxFlags("ImGuiColorEditFlags_NoSidePreview", &color_picker_flags, ImGuiColorEditFlags_NoSidePreview);
    if (color_picker_flags & ImGuiColorEditFlags_NoSidePreview)
    {
        ImGui::SameLine();
        ImGui::Checkbox("With Ref Color", &ref_color);
        if (ref_color)
        {
            ImGui::SameLine();
            ImGui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | base_flags);
        }
    }

    ImGuiColorEditFlags flags = base_flags | color_picker_flags;
    if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
    if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
    if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
    if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
    if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
    if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;

    if (ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL))
    {
        GetTarget()->Light2D()->SetLightColor(Vec3(color.x, color.y, color.z));
    }


}
