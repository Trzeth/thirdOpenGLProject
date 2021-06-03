#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <thirdEngine/Renderer/UI/ImGui/imgui_internal.h>

namespace ImGui
{
	static bool ImageAnimButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec2& uv2 = ImVec2(0, 0), const ImVec2& uv3 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f), const ImVec4& tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		// Default to using texture ID as ID. User can still push string/integer prefixes.
		// We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
		PushID((void*)user_texture_id);
		ImGuiID id = window->GetID("#image");
		PopID();

		const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
		const ImRect bb(window->DC.CursorPos, (window->DC.CursorPos + size + padding * 2));
		const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
		ItemSize(bb);
		if (!ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held);

		// Render
		const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
		if (bg_col.w > 0.0f)
			window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
		if (hovered)
		{
			window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv2, uv3, GetColorU32(tint_col));
		}
		else
		{
			window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
		}

		return pressed;
	}
};