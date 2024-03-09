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
	/*конструктор, передаем директорию .exe файла приложения*/
	ResourceManager(const std::string &executablePath);

	/*загрузка шейдерной программы*/
	std::shared_ptr<Render::ShaderProgram> loadShederProgram(
																const std::string &shaderName, 
																const std::string &vertexShaderPath, 
																const std::string fragmentShaderPath
															);

	/*получить shared_ptr на шейдерную программу*/
	std::shared_ptr<Render::ShaderProgram> getShaderProgram(const std::string &shaderName);

	/*загрузка сырой 2D текстуры*/
	std::shared_ptr<Render::Texture2D> loadTexture2D(
														const std::string &textureName, 
														const std::string &texturePath
													);

	/*получить shared_ptr на 2D сырую текстуру*/
	std::shared_ptr<Render::Texture2D> getTexture2D(const std::string &texture2DName);

	/*загрузка спрайта*/
	std::shared_ptr<Render::Sprite> loadSprite(
												   const std::string &spriteName,
												   const std::string &textureName,
												   const std::string &shaderProgramName,
												   const unsigned int spriteWidth,
												   const unsigned int spriteHeight,
												   const std::string& subTextureName = "default"
	                                          );

	/*получить shared_ptr на спрайт*/
	std::shared_ptr<Render::Sprite> getSprite(const std::string &spriteName);

	/*загрузка текстурного атласа*/
	std::shared_ptr<Render::Texture2D> loadTextureAthlas2D(
															   const std::string &texture2DName,
															   const std::string &texturePath,
															   std::vector<std::string> subTextureNames,
															   const unsigned int subTextureWidth,
															   const unsigned int subTextureHeight
														  );

private:
	/*чтение файла из исполнительной директории (где .exe файл)*/
	std::string getFileString(const std::string &relativeFilePath);

	/*map для шейдерных программ*/
	typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap _shaderPrograms;

	/*map для текстур*/
	typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> Texture2DMap;
	Texture2DMap _texture2DMaps;

	/*map для спрайтов*/
	typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpriteMap;
	SpriteMap _spriteMaps;

	/*путь до директории с .exe файлом приложения*/
	std::string _path;
};

