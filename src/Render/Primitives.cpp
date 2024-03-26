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
		render.color4(0.f, 255.f, 255.f);

		render.verex3(0.5f, 0.5f, -0.5f);
		render.color4(0.f, 255.f, 255.f);

		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(0.f, 255.f, 255.f);


		render.verex3(0.5f, 0.5f, 0.5f);
		render.color4(0, 235, 255);

		render.verex3(-0.5f, 0.5f, 0.5f);
		render.color4(0, 235, 255);

		render.verex3(-0.5f, 0.5f, -0.5f);
		render.color4(0, 235, 255);


		render.end();
	}

	void createTexCube(Renderer& render)
	{
		render.Enable(Render::Renderer::State::TEXTURE_2D);
		render.begin(GL_TRIANGLES);

		/*1*/
		render.verex3(-0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 0.0f);

		render.verex3(0.5f, -0.5f, -0.5f);
		render.vertexUV(1.0f, 0.0f);

		render.verex3(0.5f, 0.5f, -0.5f);
		render.vertexUV(1.0f, 1.0f);


		render.verex3(0.5f, 0.5f, -0.5f);
		render.vertexUV(1.0f, 1.0f);

		render.verex3(-0.5f, 0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);

		render.verex3(-0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 0.0f);




		/*2*/
		render.verex3(-0.5f, -0.5f, 0.5f);
		render.vertexUV(0.0f, 0.0f);

		render.verex3(0.5f, -0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);

		render.verex3(0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 1.0f);


		render.verex3(0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 1.0f);

		render.verex3(-0.5f, 0.5f, 0.5f);
		render.vertexUV(0.0f, 1.0f);

		render.verex3(-0.5f, -0.5f, 0.5f);
		render.vertexUV(0.0f, 0.0f);




		/*3*/
		render.verex3(-0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);

		render.verex3(-0.5f, 0.5f, -0.5f);
		render.vertexUV(1.0f, 1.0f);

		render.verex3(-0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);


		render.verex3(-0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);

		render.verex3(-0.5f, -0.5f, 0.5f);
		render.vertexUV(0.0f, 0.0f);

		render.verex3(-0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);



		/*4*/
		render.verex3(0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);

		render.verex3(0.5f, 0.5f, -0.5f);
		render.vertexUV(1.0f, 1.0f);

		render.verex3(0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);


		render.verex3(0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);

		render.verex3(0.5f, -0.5f, 0.5f);
		render.vertexUV(0.0f, 0.0f);

		render.verex3(0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);



		/*5*/
		render.verex3(-0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);

		render.verex3(0.5f, -0.5f, -0.5f);
		render.vertexUV(1.0f, 1.0f);

		render.verex3(0.5f, -0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);


		render.verex3(0.5f, -0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);

		render.verex3(-0.5f, -0.5f, 0.5f);
		render.vertexUV(0.0f, 0.0f);

		render.verex3(-0.5f, -0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);



		/*6*/
		render.verex3(-0.5f, 0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);

		render.verex3(0.5f, 0.5f, -0.5f);
		render.vertexUV(1.0f, 1.0f);

		render.verex3(0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);


		render.verex3(0.5f, 0.5f, 0.5f);
		render.vertexUV(1.0f, 0.0f);

		render.verex3(-0.5f, 0.5f, 0.5f);
		render.vertexUV(0.0f, 0.0f);

		render.verex3(-0.5f, 0.5f, -0.5f);
		render.vertexUV(0.0f, 1.0f);

		render.end();
		render.Enable(Render::Renderer::State::TEXTURE_2D);
	}

}