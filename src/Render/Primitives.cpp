#include "RendererFactory.h"

namespace Render
{

	/*цветной куб из GL примитивов*/
	void createRawCube(std::shared_ptr<Renderer> render)
	{
		/*BACK (RED)*/
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->color4(255, 0, 0);
			  
		render->verex3(0.5f, -0.5f, -0.5f);
		render->color4(255, 0, 0);
			  
		render->verex3(0.5f, 0.5f, -0.5f);
		render->color4(255, 0, 0);
			  
			  
		render->verex3(0.5f, 0.5f, -0.5f);
		render->color4(255, 50, 0);
			  
		render->verex3(-0.5f, 0.5f, -0.5f);
		render->color4(255, 50, 0);
			  
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->color4(255, 50, 0);



		/*FORNT (ORANGE)*/
		render->verex3(-0.5f, -0.5f, 0.5f);
		render->color4(255, 165, 0);
			  
		render->verex3(0.5f, -0.5f, 0.5f);
		render->color4(255, 165, 0);
			  
		render->verex3(0.5f, 0.5f, 0.5f);
		render->color4(255, 165, 0);
			  
			  
		render->verex3(0.5f, 0.5f, 0.5f);
		render->color4(255, 165, 90);
			  
		render->verex3(-0.5f, 0.5f, 0.5f);
		render->color4(255, 165, 90);
			  
		render->verex3(-0.5f, -0.5f, 0.5f);
		render->color4(255, 165, 90);



		/*LEFT (YELLOW)*/
		render->verex3(-0.5f, 0.5f, 0.5f);
		render->color4(255, 255, 0);
			  
		render->verex3(-0.5f, 0.5f, -0.5f);
		render->color4(255, 255, 0);
			  
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->color4(255, 255, 0);
			  
			  
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->color4(255, 235, 0);
			  
		render->verex3(-0.5f, -0.5f, 0.5f);
		render->color4(255, 235, 0);
			  
		render->verex3(-0.5f, 0.5f, 0.5f);
		render->color4(255, 235, 0);



		/*RIGHT (GREEN)*/
		render->verex3(0.5f, 0.5f, 0.5f);
		render->color4(0, 255, 0);
			  
		render->verex3(0.5f, 0.5f, -0.5f);
		render->color4(0, 255, 0);
			  
		render->verex3(0.5f, -0.5f, -0.5f);
		render->color4(0, 255, 0);
			  
			  
		render->verex3(0.5f, -0.5f, -0.5f);
		render->color4(90, 255, 0);
			  
		render->verex3(0.5f, -0.5f, 0.5f);
		render->color4(90, 255, 0);
			  
		render->verex3(0.5f, 0.5f, 0.5f);
		render->color4(90, 255, 0);



		/*BOT (BLUE)*/
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->color4(0, 0, 255);

		render->verex3(0.5f, -0.5f, -0.5f);
		render->color4(0, 0, 255);

		render->verex3(0.5f, -0.5f, 0.5f);
		render->color4(0, 0, 255);


		render->verex3(0.5f, -0.5f, 0.5f);
		render->color4(0, 90, 255);

		render->verex3(-0.5f, -0.5f, 0.5f);
		render->color4(0, 90, 255);

		render->verex3(-0.5f, -0.5f, -0.5f);
		render->color4(0, 90, 255);



		/*TOP (CYAN)*/
		render->verex3(-0.5f, 0.5f, -0.5f);
		render->color4(0.f, 255.f, 255.f);

		render->verex3(0.5f, 0.5f, -0.5f);
		render->color4(0.f, 255.f, 255.f);

		render->verex3(0.5f, 0.5f, 0.5f);
		render->color4(0.f, 255.f, 255.f);


		render->verex3(0.5f, 0.5f, 0.5f);
		render->color4(0, 235, 255);

		render->verex3(-0.5f, 0.5f, 0.5f);
		render->color4(0, 235, 255);

		render->verex3(-0.5f, 0.5f, -0.5f);
		render->color4(0, 235, 255);


		render->upload();
	}

	/*куб с натянутой текстурой*/
	void createTexCube(std::shared_ptr<Renderer> render)
	{
		/*1*/
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 0.0f);

		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(1.0f, 0.0f);

		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);


		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);

		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);

		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 0.0f);




		/*2*/
		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);

		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);

		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 1.0f);


		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 1.0f);

		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(0.0f, 1.0f);

		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);




		/*3*/
		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);

		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);


		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);

		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);



		/*4*/
		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);

		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);

		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);


		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);

		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);

		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);



		/*5*/
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);

		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);

		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);


		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);

		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);

		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);



		/*6*/
		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);

		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);

		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);


		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);

		render->upload();
	}

	/*куб с натянутой текстурой и нормалями к каждой вершине*/
	void createTexCubeLight(std::shared_ptr<Renderer> render)
	{
		/*1*/
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(0.0f, 0.0f, -1.0f);

		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(0.0f, 0.0f, -1.0f);

		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(0.0f, 0.0f, -1.0f);


		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(0.0f, 0.0f, -1.0f);

		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(0.0f, 0.0f, -1.0f);

		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(0.0f, 0.0f, -1.0f);




		/*2*/
		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(0.0f, 0.0f, 1.0f);

		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(0.0f, 0.0f, 1.0f);

		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(0.0f, 0.0f, 1.0f);


		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(0.0f, 0.0f, 1.0f);

		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(0.0f, 0.0f, 1.0f);

		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(0.0f, 0.0f, 1.0f);




		/*3*/
		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(-1.0f, 0.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(-1.0f, 0.0f, 0.0f);

		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(-1.0f, 0.0f, 0.0f);


		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(-1.0f, 0.0f, 0.0f);

		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(-1.0f, 0.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(-1.0f, 0.0f, 0.0f);



		/*4*/
		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(1.0f, 0.0f, 0.0f);

		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(1.0f, 0.0f, 0.0f);

		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(1.0f, 0.0f, 0.0f);


		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(1.0f, 0.0f, 0.0f);

		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(1.0f, 0.0f, 0.0f);

		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(1.0f, 0.0f, 0.0f);



		/*5*/
		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(0.0f, -1.0f, 0.0f);

		render->verex3(0.5f, -0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(0.0f, -1.0f, 0.0f);

		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(0.0f, -1.0f, 0.0f);


		render->verex3(0.5f, -0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(0.0f, -1.0f, 0.0f);

		render->verex3(-0.5f, -0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(0.0f, -1.0f, 0.0f);

		render->verex3(-0.5f, -0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(0.0f, -1.0f, 0.0f);



		/*6*/
		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(0.0f, 1.0f, 0.0f);

		render->verex3(0.5f, 0.5f, -0.5f);
		render->vertexUV(1.0f, 1.0f);
		render->verex3(0.0f, 1.0f, 0.0f);

		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(0.0f, 1.0f, 0.0f);


		render->verex3(0.5f, 0.5f, 0.5f);
		render->vertexUV(1.0f, 0.0f);
		render->verex3(0.0f, 1.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, 0.5f);
		render->vertexUV(0.0f, 0.0f);
		render->verex3(0.0f, 1.0f, 0.0f);

		render->verex3(-0.5f, 0.5f, -0.5f);
		render->vertexUV(0.0f, 1.0f);
		render->verex3(0.0f, 1.0f, 0.0f);

		render->upload();
	}

	/*куб с натянутой текстурой и нормалями к каждой вершине (задается перечислением восьми вершин куба)*/
	void createTexCubeLight_Indices(std::shared_ptr<Renderer> render)
	{
		// FRONT
		render->verex3(-0.5f, -0.5f, -0.5f);		render->vertexUV(0.f, 0.f);		render->verex3(-1.f, 0.f, 0.f);         // 0
		render->verex3(-0.5f,  0.5f, -0.5f);		render->vertexUV(1.f, 0.f);		render->verex3(-1.f, 0.f, 0.f);         // 1
		render->verex3(-0.5f,  0.5f,  0.5f);		render->vertexUV(1.f, 1.f);		render->verex3(-1.f, 0.f, 0.f);         // 2
		render->verex3(-0.5f, -0.5f,  0.5f);		render->vertexUV(0.f, 1.f);		render->verex3(-1.f, 0.f, 0.f);         // 3

        // BACK                                  
		render->verex3(0.5f, -0.5f, -0.5f);		render->vertexUV(1.f, 0.f);		render->verex3(1.f, 0.f, 0.f);          // 4
		render->verex3(0.5f,  0.5f, -0.5f);		render->vertexUV(0.f, 0.f);		render->verex3(1.f, 0.f, 0.f);          // 5
		render->verex3(0.5f,  0.5f,  0.5f);		render->vertexUV(0.f, 1.f);		render->verex3(1.f, 0.f, 0.f);          // 6
		render->verex3(0.5f, -0.5f,  0.5f);		render->vertexUV(1.f, 1.f);		render->verex3(1.f, 0.f, 0.f);          // 7

        // RIGHT
		render->verex3(-0.5f, 0.5f, -0.5f);		render->vertexUV(0.f, 0.f);		render->verex3(0.f, 1.f, 0.f);          // 8
		render->verex3(0.5f,  0.5f, -0.5f);		render->vertexUV(1.f, 0.f);		render->verex3(0.f, 1.f, 0.f);          // 9
		render->verex3(0.5f,  0.5f,  0.5f);		render->vertexUV(1.f, 1.f);		render->verex3(0.f, 1.f, 0.f);          // 10
		render->verex3(-0.5f, 0.5f,  0.5f);		render->vertexUV(0.f, 1.f);		render->verex3(0.f, 1.f, 0.f);          // 11

        // LEFT
		render->verex3(-0.5f, -0.5f, -0.5f);		render->vertexUV(1.f, 0.f);		render->verex3(0.f, -1.f, 0.f);         // 12
		render->verex3( 0.5f, -0.5f, -0.5f);		render->vertexUV(0.f, 0.f);		render->verex3(0.f, -1.f, 0.f);         // 13
		render->verex3( 0.5f, -0.5f,  0.5f);		render->vertexUV(0.f, 1.f);		render->verex3(0.f, -1.f, 0.f);         // 14
		render->verex3(-0.5f, -0.5f,  0.5f);	    render->vertexUV(1.f, 1.f);		render->verex3(0.f, -1.f, 0.f);         // 15

        // TOP
		render->verex3(-0.5f, -0.5f, 0.5f);		render->vertexUV(0.f, 0.f);		render->verex3(0.f, 0.f, 1.f);          // 16
		render->verex3(-0.5f,  0.5f, 0.5f);		render->vertexUV(1.f, 0.f);		render->verex3(0.f, 0.f, 1.f);          // 17
		render->verex3( 0.5f,  0.5f, 0.5f);		render->vertexUV(1.f, 1.f);		render->verex3(0.f, 0.f, 1.f);          // 18
		render->verex3( 0.5f, -0.5f, 0.5f);		render->vertexUV(0.f, 1.f);		render->verex3(0.f, 0.f, 1.f);          // 19

        // BOTTOM
		render->verex3(-0.5f, -0.5f, -0.5f);		render->vertexUV(0.f, 1.f);		render->verex3(0.f, 0.f, -1.f);         // 20
		render->verex3(-0.5f,  0.5f, -0.5f);		render->vertexUV(1.f, 1.f);	    render->verex3(0.f, 0.f, -1.f);         // 21
		render->verex3( 0.5f,  0.5f, -0.5f);		render->vertexUV(1.f, 0.f);		render->verex3(0.f, 0.f, -1.f);         // 22
		render->verex3( 0.5f, -0.5f, -0.5f);		render->vertexUV(0.f, 0.f);		render->verex3(0.f, 0.f, -1.f);         // 23

		render->upload();

		/*индексы используемых вершин*/
		std::vector<GLuint> indices =
		{
			0,   1,  2,  2,  3,  0, // front
			4,   5,  6,  6,  7,  4, // back
			8,   9, 10, 10, 11,  8, // right
			12, 13, 14, 14, 15, 12, // left
			16, 17, 18, 18, 19, 16, // top
			20, 21, 22, 22, 23, 20  // bottom
		};
		/*передача индексов в рендер*/
		render->setIndices(indices);
	}

	void createXYplane(std::shared_ptr<Renderer> render)
	{
		render->verex3(0.5f, 0.0f,  -0.5f);
		//render->color4(255.f, 0.f, 0.f);

		render->verex3(0.5f, 0.0f,  0.5f);
		//render->color4(255.f, 0.f, 0.f);

		render->verex3(-0.5f, 0.0f,  0.5f);
		//render->color4(255.f, 0.f, 0.f);


		render->verex3(0.5f, 0.0f,  -0.5f);
		//render->color4(255.f, 0.f, 0.f);

		render->verex3(-0.5f, 0.0f,  -0.5f);
		//render->color4(255.f, 0.f, 0.f);

		render->verex3(-0.5f, 0.0f,  0.5f);
		//render->color4(255.f, 0.f, 0.f);

		render->upload();
	}
}