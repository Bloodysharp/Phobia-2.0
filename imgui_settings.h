#include "imgui.h"

namespace font
{
	inline ImFont* poppins = nullptr;
	inline ImFont* poppinss = nullptr;
	inline ImFont* font_icon = nullptr;
}

namespace c
{

	inline ImVec4 accent = ImColor(112, 110, 215);

	namespace background
	{

		inline ImVec4 filling = ImColor(12, 12, 12);
		inline ImVec4 stroke = ImColor(24, 26, 36);
		inline ImVec2 size = ImVec2(838, 535);

		inline float rounding = 10;
		inline float rounding_o = 5;

	}

	namespace elements
	{
		inline ImVec4 mark = ImColor(255, 255, 255);

		inline ImVec4 stroke = ImColor(75, 75, 75);
		inline ImVec4 background = ImColor(75, 75, 75);
		inline ImVec4 background_widget = ImColor(75, 75, 75);

		inline ImVec4 text_active = ImColor(255, 255, 255);
		inline ImVec4 text_hov = ImColor(81, 92, 109);
		inline ImVec4 text = ImColor(43, 51, 63);

		inline float rounding = 4;
	}

	namespace child
	{

	}

	namespace tab
	{
		inline ImVec4 tab_active = ImColor(22, 22, 30);

		inline ImVec4 border = ImColor(14, 14, 15);
	}

}