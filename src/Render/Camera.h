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
		enum class ProjectionMode
		{
			PERSPECTIVE,
			ORTHOGRAPHIC_CENTER,
			ORTHOGRAPHIC_LEFT_BOT
		};

		Camera(
				const glm::vec3& position = { 0,0,0 },
				const glm::vec3& rotation = { 0,0,0 },
				const float nearPlane = 0.1f,
				const float farplane = 100.f,
			    const float windowHeight = 768.f,
			    const float windowWidth = 1024.f,
				const ProjectionMode projMode = ProjectionMode::PERSPECTIVE
		      ) : _position(position),
			      _rotation(rotation), 
			      _projectionMode(projMode), 
			      _nearPlane(nearPlane), 
			      _farPlane(farplane),
			      _windowHeight(windowHeight),
			      _windowWidth(windowWidth)
		{ 
			updateViewMat();
			updateProjMat();
		}

		void setPlane(const float near, const float far);
		void setWindowSize(const float height, const float width);

		void setPosition(const glm::vec3& position);

		void setRotation(const glm::vec3& rotation);


		void setPositionRotation(const glm::vec3& position, const glm::vec3& rotation);

		void setProjectionMode(ProjectionMode mode);

		glm::mat4 getViewMat();
		glm::mat4 getProjMat();
	private:
		ProjectionMode _projectionMode;
		glm::vec3 _position;
		glm::vec3 _rotation;

		glm::mat4 _viewMat;
		glm::mat4 _projMat;

		float _nearPlane;
		float _farPlane;

		float _windowHeight;
		float _windowWidth;

		void updateViewMat();
		void updateProjMat();
	};


}