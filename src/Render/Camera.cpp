#include "Camera.h"


namespace Render
{
	/*============================================================*/
	/*установка near и far plane*/
	void Camera::setPlane(const float near, const float far)
	{
		_nearPlane = near;
		_farPlane = far;
		updateProjMat();
	}

	/*============================================================*/
	/*установка размеров окна отрисовки*/
	void Camera::setWindowSize(const float height, const float width)
	{
		_windowHeight = height;
		_windowWidth = width;
	}

	/*============================================================*/
	/*позиция камеры в мировой СК*/
	void Camera::setPosition(const glm::vec3& position)
	{
		_position = position;
		updateViewMat();
	}

	/*============================================================*/
	/*поворот камеры*/
	void Camera::setRotation(const glm::vec3& rotation)
	{
		_rotation = rotation;
		updateViewMat();
	}

	/*============================================================*/
	/*установка одновременно и позиции и поворта камеры*/
	void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		_position = position;
		_rotation = rotation;
		updateViewMat();
	}

	/*============================================================*/
	/*установка типа камеры*/
	void Camera::setProjectionMode(ProjectionMode mode)
	{
		_projectionMode = mode;
		updateProjMat();
	}

	/*============================================================*/
	/*матрица вида*/
	glm::mat4 Camera::getViewMat()
	{
		return _viewMat;
	}

	/*============================================================*/
	/*матрица проекции*/
	glm::mat4 Camera::getProjMat()
	{
		return _projMat;
	}

	/*============================================================*/
	/*пересоздать матрицу вида*/
	void Camera::updateViewMat()
	{
		_viewMat = glm::lookAt(_position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	}

	/*============================================================*/
	/*пересоздать матрицу проекции*/
	void Camera::updateProjMat()
	{
		const float aspect = _windowWidth / _windowHeight;
		switch (_projectionMode)
		{
		case Render::Camera::ProjectionMode::PERSPECTIVE:
			_projMat = glm::perspective(
											glm::radians(45.f),
											aspect,
											_nearPlane,
											_farPlane
									   );
			break;
		case Render::Camera::ProjectionMode::ORTHOGRAPHIC_CENTER:
			_projMat = glm::ortho(
									  -_windowWidth / 2.f, _windowHeight / 2.f,
									  -_windowHeight / 2.f, _windowWidth / 2.f,
									  _nearPlane, _farPlane
								 );
			break;
		case Render::Camera::ProjectionMode::ORTHOGRAPHIC_LEFT_BOT:
			_projMat = glm::ortho(
									  0.f,
									  _windowWidth, 
									  0.f, 
									  _windowHeight, 
									  _nearPlane, 
									  _farPlane
								);
			break;
		default:
			const float aspect = _windowWidth / _windowHeight;
			_projMat = glm::perspective(
											glm::radians(45.f),
											aspect,
											_nearPlane,
											_farPlane
									   );
			break;
		}
	}


}
