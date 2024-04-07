#include "Camera.h"

#include "../Input/Input.h"

#include <algorithm>


namespace Render
{
	/*============================================================*/
	/*��������� near � far plane*/
	void Camera::setPlane(const float near, const float far)
	{
		_nearPlane = near;
		_farPlane = far;
		updateProjMat();
	}

	/*============================================================*/
	/*��������� �������� ���� ���������*/
	void Camera::setWindowSize(const float height, const float width)
	{
		_windowHeight = height;
		_windowWidth = width;
	}

	/*============================================================*/
	/*������� ������ � ������� ��*/
	void Camera::setPosition(const glm::vec3& position)
	{
		_position = position;
		updateViewMat();
	}

	/*============================================================*/
	/*������� ������*/
	void Camera::setRotation(const glm::vec3& rotation)
	{
		_rotation = rotation;
		updateViewMat();
	}

	/*============================================================*/
	/*��������� ������������ � ������� � ������� ������*/
	void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		_position = position;
		_rotation = rotation;
		updateViewMat();
	}

	/*============================================================*/
	/*��������� ���� ������*/
	void Camera::setProjectionMode(ProjectionMode mode)
	{
		_projectionMode = mode;
		updateProjMat();
	}

	/*============================================================*/
	/*������� ����*/
	glm::mat4 Camera::getViewMat()
	{
		return _viewMat;
	}

	/*============================================================*/
	/*������� ��������*/
	glm::mat4 Camera::getProjMat()
	{
		return _projMat;
	}

	/*============================================================*/
	/*����������� ������� ����*/
	void Camera::updateViewMat()
	{
		_front.x = cos(glm::radians(_rotation[1])) * cos(glm::radians(_rotation[2]));
		_front.y = sin(glm::radians(_rotation[1]));
		_front.z = cos(glm::radians(_rotation[1])) * sin(glm::radians(_rotation[2]));
		_viewMat = glm::lookAt(_position, _position + _front, glm::vec3(0.f, 1.f, 0.f));
	}

	/*============================================================*/
	/*����������� ������� ��������*/
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


	void Camera::moveCamera(const float duration)
	{
		bool isUpdateViewMat = false;
		if (Core::Input::isKeyPressed(GLFW_KEY_W))
		{
			_position += (duration * _velocity) * _front;
			isUpdateViewMat = true;
		}

		if(Core::Input::isKeyPressed(GLFW_KEY_S))
		{
			_position -= (duration * _velocity) * _front;
			isUpdateViewMat = true;
		}

		if (Core::Input::isKeyPressed(GLFW_KEY_A))
		{
			_position -= glm::normalize(glm::cross(_front, _up)) * (duration * _velocity);
			isUpdateViewMat = true;
		}

		if (Core::Input::isKeyPressed(GLFW_KEY_D))
		{
			_position += glm::normalize(glm::cross(_front, _up)) * (duration * _velocity);
			isUpdateViewMat = true;
		}

		if (isUpdateViewMat)
			updateViewMat();
	}


	void Camera::rotateCamera(const float duration)
	{

	}

}