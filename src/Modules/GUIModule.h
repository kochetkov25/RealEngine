#pragma once

struct GLFWwindow;

namespace Modules
{


	class GUIModule
	{
	public:
		static void onWindowCreate(GLFWwindow* pWindow);

		static void onWindowClose();

		static void GUIupdate();

		static void GUIdraw();
	};


}