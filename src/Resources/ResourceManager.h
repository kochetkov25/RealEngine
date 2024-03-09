/*
**  File        :	ResourceManager.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   09.03.2024
**  Modified on :   09.03.2024
**  Description :
*/

#pragma once
#include "../Render/ShaderProgram.h"
#include "../Render/Sprite.h"

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

namespace Render{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}


class ResourceManager{
public:
	/*�����������, �������� ���������� .exe ����� ����������*/
	ResourceManager(const std::string &executablePath);

	/*�������� ��������� ���������*/
	std::shared_ptr<Render::ShaderProgram> loadShederProgram(
																const std::string &shaderName, 
																const std::string &vertexShaderPath, 
																const std::string fragmentShaderPath
															);

	/*�������� shared_ptr �� ��������� ���������*/
	std::shared_ptr<Render::ShaderProgram> getShaderProgram(const std::string &shaderName);

	/*�������� ����� 2D ��������*/
	std::shared_ptr<Render::Texture2D> loadTexture2D(
														const std::string &textureName, 
														const std::string &texturePath
													);

	/*�������� shared_ptr �� 2D ����� ��������*/
	std::shared_ptr<Render::Texture2D> getTexture2D(const std::string &texture2DName);

	/*�������� �������*/
	std::shared_ptr<Render::Sprite> loadSprite(
												   const std::string &spriteName,
												   const std::string &textureName,
												   const std::string &shaderProgramName,
												   const unsigned int spriteWidth,
												   const unsigned int spriteHeight,
												   const std::string& subTextureName = "default"
	                                          );

	/*�������� shared_ptr �� ������*/
	std::shared_ptr<Render::Sprite> getSprite(const std::string &spriteName);

	/*�������� ����������� ������*/
	std::shared_ptr<Render::Texture2D> loadTextureAthlas2D(
															   const std::string &texture2DName,
															   const std::string &texturePath,
															   std::vector<std::string> subTextureNames,
															   const unsigned int subTextureWidth,
															   const unsigned int subTextureHeight
														  );

private:
	/*������ ����� �� �������������� ���������� (��� .exe ����)*/
	std::string getFileString(const std::string &relativeFilePath);

	/*map ��� ��������� ��������*/
	typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap _shaderPrograms;

	/*map ��� �������*/
	typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> Texture2DMap;
	Texture2DMap _texture2DMaps;

	/*map ��� ��������*/
	typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpriteMap;
	SpriteMap _spriteMaps;

	/*���� �� ���������� � .exe ������ ����������*/
	std::string _path;
};

