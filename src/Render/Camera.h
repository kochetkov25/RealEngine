#pragma once

#include <glm/vec3.hpp>

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
				const ProjectionMode projMode = ProjectionMode::PERSPECTIVE
		      );

	private:
		ProjectionMode _projectionMode;
	};


}