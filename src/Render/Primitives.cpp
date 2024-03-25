#include "Renderer.h"

namespace Render
{


	void createRawCube(Renderer& render)
	{
		render.begin(GL_TRIANGLES);


		/*BACK (RED)*/
		render.verex3(-0.5f, -0.5f, -0.5f);
		render.color4(255, 0, 0);

		render.verex3(0.5f, -0.5f, -0.5f);
		render.color4(255, 0, 0);

		render.verex3(0.5f, 0.5f, -0.5f);
		render.color4(255, 0, 0);


		render.verex3(0.5f, 0.5f, -0.5f);
		render.color4(255, 50, 0);

		render.verex3(-0.5f, 0.5f, -0.5f);
		render.color4(255, 50, 0);

		render.verex3(-0.5f, -0.5f, -0.5f);
		render.color4(255, 50, 0);



		/*FORNT (ORANGE)*/
		render.verex3(-0.5f, -0.5f, 0.5f);
		render.color4(255, 165, 0);

		render.verex3(0.5f, -0.5f, 0.5f);
		render.color4(255, 165, 0);

		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(255, 165, 0);


		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(255, 165, 90);

		render.verex3(-0.5f, 0.5f, 0.5f);
		render.color4(255, 165, 90);

		render.verex3(-0.5f, -0.5f, 0.5f);
		render.color4(255, 165, 90);



		/*LEFT (YELLOW)*/
		render.verex3(-0.5f, 0.5f, 0.5f);
		render.color4(255, 255, 0);

		render.verex3(-0.5f, 0.5f, -0.5f);
		render.color4(255, 255, 0);

		render.verex3(-0.5f, -0.5f, -0.5f);
		render.color4(255, 255, 0);


		render.verex3(-0.5f, -0.5f, -0.5f);
		render.color4(255, 235, 0);

		render.verex3(-0.5f, -0.5f, 0.5f);
		render.color4(255, 235, 0);

		render.verex3(-0.5f, 0.5f, 0.5f);
		render.color4(255, 235, 0);



		/*RIGHT (GREEN)*/
		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(0, 255, 0);

		render.verex3(0.5f, 0.5f, -0.5f);
		render.color4(0, 255, 0);

		render.verex3(0.5f, -0.5f, -0.5f);
		render.color4(0, 255, 0);


		render.verex3(0.5f, -0.5f, -0.5f);
		render.color4(90, 255, 0);

		render.verex3(0.5f, -0.5f, 0.5f);
		render.color4(90, 255, 0);

		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(90, 255, 0);



		/*BOT (BLUE)*/
		render.verex3(-0.5f, -0.5f, -0.5f);
		render.color4(0, 0, 255);

		render.verex3(0.5f, -0.5f, -0.5f);
		render.color4(0, 0, 255);

		render.verex3(0.5f, -0.5f, 0.5f);
		render.color4(0, 0, 255);


		render.verex3(0.5f, -0.5f, 0.5f);
		render.color4(0, 90, 255);

		render.verex3(-0.5f, -0.5f, 0.5f);
		render.color4(0, 90, 255);

		render.verex3(-0.5f, -0.5f, -0.5f);
		render.color4(0, 90, 255);



		/*TOP (CYAN)*/
		render.verex3(-0.5f, 0.5f, -0.5f);
		render.color4(0.f, 255.f, 255.f,0.7f);

		render.verex3(0.5f, 0.5f, -0.5f);
		render.color4(0.f, 255.f, 255.f, 0.7f);

		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(0.f, 255.f, 255.f, 0.7f);


		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(0, 235, 255);

		render.verex3(-0.5f, 0.5f, 0.5f);
		render.color4(0, 235, 255);

		render.verex3(-0.5f, 0.5f, -0.5f);
		render.color4(0, 235, 255);


		render.end();
	}


}