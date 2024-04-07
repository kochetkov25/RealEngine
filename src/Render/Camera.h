#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render 
{


	class Camera
	{
	public:
		/*тип камеры*/
		enum class ProjectionMode
		{
			PERSPECTIVE,
			ORTHOGRAPHIC_CENTER,
			ORTHOGRAPHIC_LEFT_BOT
		};

		/*конструктор*/
		/*
		* ВНИМАНИЕ!!!
		* Для корректной работы камеры
		* необходимо задать правильные
		* параметры frustum.
		* windowHeight и windowHeight должны
		* совпадать с параметрами окна отрисовки
		*/
		Camera(
				const glm::vec3& position = { 0,0,0 },
				const glm::vec3& rotation = { 0,0,-90 },
				const float nearPlane = 0.1f,
				const float farplane = 100.f,
			    const float windowHeight = 768.f,
			    const float windowWidth = 1024.f,
				const ProjectionMode projMode = ProjectionMode::PERSPECTIVE,
				const float velocity = 0.0f
		      ) : _position(position),
			      _rotation(rotation), 
			      _projectionMode(projMode), 
			      _nearPlane(nearPlane), 
			      _farPlane(farplane),
			      _windowHeight(windowHeight),
			      _windowWidth(windowWidth),
			      _velocity(velocity)
		{ 
			updateViewMat();
			updateProjMat();
		}

		/*установка near и far plane*/
		void setPlane(const float near, const float far);
		/*установка размеров окна отрисовки*/
		void setWindowSize(const float height, const float width);

		/*позиция камеры в мировой СК*/
		void setPosition(const glm::vec3& position);
		/*поворот камеры*/
		void setRotation(const glm::vec3& rotation);

		/*установка одновременно и позиции и поворта камеры*/
		void setPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
		/*установка типа камеры*/
		void setProjectionMode(ProjectionMode mode);

		/*матрица вида*/
		glm::mat4 getViewMat();
		/*матрица проекции*/
		glm::mat4 getProjMat();

		void setVelocity(const float velocity) { _velocity = velocity; }

		void moveCamera(const float duration);

		void rotateCamera(const float duration);

		glm::vec3 getPosition() { return _position; }
	private:
		ProjectionMode _projectionMode;
		glm::vec3 _position;
		glm::vec3 _rotation = { 0.0f, 0.0f, -90.0f }; /*ROLL PITCH YAW*/
		glm::vec3 _front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 _up = { 0.0f, 1.0f, 0.0f };

		glm::mat4 _viewMat;
		glm::mat4 _projMat;


		float _nearPlane;
		float _farPlane;

		float _windowHeight;
		float _windowWidth;

		float _velocity;

		/*пересоздать матрицу вида*/
		void updateViewMat();
		/*пересоздать матрицу проекции*/
		void updateProjMat();
	};


}