#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <vector>

namespace Render{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}


class ResourceManager{
public:
	// конструктор принимает на вход полный путь до exe файла
	// и устанавливает в path директорию exe файла
	ResourceManager(const std::string &executablePath);
	// функция загружает в поле класса шейдерную программу
	std::shared_ptr<Render::ShaderProgram> loadShederProgram(const std::string &shaderName, const std::string &vertexShaderPath, const std::string fragmentShaderPath);
	// функция возвращает указатель на шейдерную программу из map по имени программы
	std::shared_ptr<Render::ShaderProgram> getShaderProgram(const std::string &shaderName);
	// функция побитово загружает текстуру и вычисляет ее размеры + количество каналов
	std::shared_ptr<Render::Texture2D> loadTexture2D(const std::string &textureName, const std::string &texturePath);
	// функция возвращает указатель на 2д текстуру по имени текстуры
	std::shared_ptr<Render::Texture2D> getTexture2D(const std::string &texture2DName);
	// функция создает и загружает в ресурсный менеджер спрайт
	std::shared_ptr<Render::Sprite> loadSprite(const std::string &spriteName,
											   const std::string &textureName,
											   const std::string &shaderProgramName,
											   const unsigned int spriteWidth,
											   const unsigned int spriteHeight,
											   const std::string& subTextureName = "default");
	// функция возвращает указатель на загруженный спрайт по имени спрайта
	std::shared_ptr<Render::Sprite> getSprite(const std::string &spriteName);
	// данная функция загружает в мап текстуру, и в соответствии с вектором названий сабтекстур загружает 
	// в мап объекта Texture2D координаты сабтекстур
	std::shared_ptr<Render::Texture2D> loadTextureAthlas2D(const std::string &texture2DName,
														   const std::string &texturePath,
														   std::vector<std::string> subTextureNames,
														   const unsigned int subTextureWidth,
														   const unsigned int subTextureHeight);



	//// загрузка анимированного спрайта (аналогична загрузке обычного спрайта)
	//std::shared_ptr<Render::AnimatedSprite> loadAnimatedSprite(const std::string &AnimatedSpriteName,
	//	const std::string &textureName,
	//	const std::string &shaderProgramName,
	//	const unsigned int spriteWidth,
	//	const unsigned int spriteHeight,
	//	const std::string& subTextureName = "default");
	//// получить указатель на нужный анимированный спрайт.
	//std::shared_ptr<Render::AnimatedSprite> getAnimatedSprite(const std::string &AnimatedSpriteName);

private:
	// функция возвращает все содержимое txt файла по пути относительно exe
	std::string getFileString(const std::string &relativeFilePath);
	// создаем псевдоним для мапа шейдерных программ
	typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
	// создаем map для хранения шейдерных программ
	ShaderProgramsMap _shaderPrograms;

	// создаем псевдоним для мапа текстур
	typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> Texture2DMap;
	// создаем мап для хранения текстур
	Texture2DMap _texture2DMaps;

	// создаем псевдоним для мапа спрайтов
	typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpriteMap;
	SpriteMap _spriteMaps;

	//// создаем псевдоним для мапа анимированных спрайтов
	//typedef std::map<const std::string, std::shared_ptr<Render::AnimatedSprite>> AnimatedSpriteMap;
	//AnimatedSpriteMap _AnimatedSpriteMaps;


	// путь до папки с ресурсами
	std::string _path;
};

