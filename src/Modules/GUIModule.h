#pragma once

struct GLFWwindow;

namespace Modules
{


	class GUIModule
	{
	public:
		/*инициализация модуля пользовательского интерфейса*/
		static void onWindowCreate(GLFWwindow* pWindow);

		/*освобождения ресурсов, занятх модулем*/
		static void onWindowClose();

		/*начало отрисовки*/
		static void GUIbegin();

		/*конец отрисовки*/
		static void GUIend();
	};


}