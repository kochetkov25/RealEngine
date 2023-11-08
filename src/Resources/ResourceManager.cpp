#include "ResourceManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite.h"
#include "../Render/AnimatedSprite.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"
// подключение библиотеки stbi. Запрет на использование любых форматов кроме png

// конструктор принимает на вход полный путь до exe файла
// и устанавливает в path директорию exe файла
ResourceManager::ResourceManager(const std::string &executablePath)
{
	// в функцию передается путь до exe файла.
	// далее ищем первый найденный / или '\\'
	size_t found = executablePath.find_last_of("/\\");
	// обрезаем строку до найденной позиции
	_path = executablePath.substr(0, found);
}

// функция возвращает все содержимое txt файла по пути относительно exe
std::string ResourceManager::getFileString(const std::string &relativeFilePath)
{
	// на вход подается относительное расположение файла
	std::fstream file;
	file.open(_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	// если файл не удалось открыть, возвращаем пустую строку
	if (!file.is_open()){
		std::cerr << "Failed open file (source: ResourceManager): " << relativeFilePath << std::endl;
		return std::string();
	}
	// если удалось, считываем все содержимое в буффер и затем возвращаем из буффера си строку
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

// функция загружает в поле класса шейдерную программу
std::shared_ptr<Render::ShaderProgram> ResourceManager::loadShederProgram(const std::string &shaderName, const std::string &vertexShaderPathRelative, const std::string fragmentShaderPathRelative)
{
	// получаем в строке код вершинного шейдера
	std::string vertexString = getFileString(vertexShaderPathRelative);
	if (vertexString.empty()){
		std::cerr << "Failed to load VERTEX SHADER. (source: ResourceManager)" << std::endl;
		return nullptr;
	}
	// получаем в строке код фрагментного шейдера
	std::string fragmentString = getFileString(fragmentShaderPathRelative);
	if (fragmentString.empty()){
		std::cerr << "Failed to load FRAGMENT SHADER. (source: ResourceManager)" << std::endl;
		return nullptr;
	}
	// создаем указатель на новую шейдерную программу
	std::shared_ptr<Render::ShaderProgram> pNewShaderProgram = std::make_shared<Render::ShaderProgram>(vertexString, fragmentString);
	// проверяем получилось ли создать шейдерную программу
	if (!pNewShaderProgram->isCompiled()){
		std::cerr << "Can not create new shader program. Path to shaders: \n"
			<< "Vertex shader: " << vertexShaderPathRelative << "\n"
			<< "Fragment shader: " << fragmentShaderPathRelative <<"\n"
			<<"(source: ResourceManager)"<<std::endl;
		return nullptr;
	}
	// добавляем шейдерную программу в map и возвращаем указатель на нее
	auto isShaderProgramAdd = _shaderPrograms.emplace(shaderName, pNewShaderProgram);
	return isShaderProgramAdd.first->second;
}
// функция возвращает указатель на шейдерную программу из map по имени программы
std::shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(const std::string &shaderName)
{
	ShaderProgramsMap::const_iterator it = _shaderPrograms.find(shaderName);
	if (it != _shaderPrograms.end()){
		return it->second;
	}
	std::cerr << "Can not find shader program (source: ResourceManager): " << shaderName << std::endl;
	return nullptr;
}

// функция побитово загружает текстуру и вычисляет ее размеры + количество каналов
std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D(const std::string &textureName, const std::string &texturePathRelative)
{
	int chanels = 0;
	int width = 0;
	int height = 0;
	// В OpenGl координаты текстур задаются от нижнего левого угла. В stbi от верхнего правого. Чтобы решить эту проблему, 
	// устанавливаем флаг для переворота текстуры
	stbi_set_flip_vertically_on_load(true);
	// загружаем текстуру
	unsigned char *pixelsArr=stbi_load(std::string(_path + "/" + texturePathRelative).c_str(), &width, &height, &chanels, 0);

	if (!pixelsArr){
		std::cerr << "Can not load texture image (source: ResourceManager): " << texturePathRelative << std::endl;
		return nullptr;
	}

	// создаем текстуру и указатель на нее
	std::shared_ptr<Render::Texture2D> pNewTexture2D = std::make_shared<Render::Texture2D>(width, height, pixelsArr, chanels, GL_NEAREST, GL_CLAMP_TO_EDGE);
	// добавляем текстуру в мап
	auto isNewTexture2DAdd = _texture2DMaps.emplace(textureName, pNewTexture2D);
	// освобождаем данные, которые заняла stbi для загрузки текстуры
	stbi_image_free(pixelsArr);
	
	// возвращаем указатель на созданную текстуру
	return isNewTexture2DAdd.first->second;
}

// функция возвращает указатель на 2д текстуру по имени текстуры
std::shared_ptr<Render::Texture2D> ResourceManager::getTexture2D(const std::string &texture2DName)
{
	Texture2DMap::const_iterator it = _texture2DMaps.find(texture2DName);
	if (it != _texture2DMaps.end()){
		return it->second;
	}
	std::cerr << "Can not find texture 2D (source: ResourceManager): " << texture2DName << std::endl;
	return nullptr;
}

// функция создает и загружает в ресурсный менеджер спрайт
std::shared_ptr<Render::Sprite> ResourceManager::loadSprite(const std::string &spriteName,
	const std::string &textureName,
	const std::string &shaderProgramName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight,
	const std::string& subTextureName)
{
	auto pTexture = getTexture2D(textureName);
	if (!pTexture){
		std::cerr << "Texture with this name can not be found (source: ResourceManager): " << textureName << std::endl;
		return nullptr;
	}

	auto pShaderProgram = getShaderProgram(shaderProgramName);
	if (!pShaderProgram){
		std::cerr << "Shader program with this name can not be found (source: ResourceManager): " << shaderProgramName << std::endl;
	}

	std::shared_ptr<Render::Sprite> pNewSprite = std::make_shared<Render::Sprite>(pTexture, subTextureName, pShaderProgram, glm::vec2(0.f, 0.f), glm::vec2(spriteWidth, spriteHeight), 0.f);
	auto isNewSpriteAdd = _spriteMaps.emplace(spriteName, pNewSprite);
	return isNewSpriteAdd.first->second;
}

// функция возвращает указатель на загруженный спрайт по имени спрайта
std::shared_ptr<Render::Sprite> ResourceManager::getSprite(const std::string &spriteName)
{
	SpriteMap::const_iterator it = _spriteMaps.find(spriteName);
	if (it != _spriteMaps.end()){
		return it->second;
	}
	std::cerr << "Can not find sprite (source: ResourceManager): " << spriteName << std::endl;
	return nullptr;
}

// данная функция загружает в мап текстуру, и в соответствии с вектором названий сабтекстур загружает 
// в мап объекта Texture2D координаты сабтекстур
std::shared_ptr<Render::Texture2D> ResourceManager::loadTextureAthlas2D(const std::string &texture2DName,
	const std::string &texturePath,
	std::vector<std::string> subTextureNames,
	const unsigned int subTextureWidth,
	const unsigned int subTextureHeight)
{
	// загружаем текстуру в объект класса
	auto pTexture = loadTexture2D(texture2DName, texturePath);
	if (pTexture)
	{
		// получаем длинну и высоту текстуры
		const unsigned int textureWidth = pTexture->getWidth();
		const unsigned int texturHeight = pTexture->getWidth();
		// устанавливаем начальное положение (левый верхний угол)
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = texturHeight;

		// проходимся по всему массиву имен саб текстур и вычисляем координаты  
		// нижнего левого угла и верхнего правого угла сабтекстуры
		for (auto &currentSubtextureName : subTextureNames)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
								   static_cast<float>(currentTextureOffsetY - subTextureHeight) / texturHeight);

			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
								 static_cast<float>(currentTextureOffsetY) / texturHeight);

			// после вычисления, добавляем в объект класса Texture2D имя сабтекстуры и координаты углов
			pTexture->addSubTexture2D(currentSubtextureName, leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}

	}
	return pTexture;
}





std::shared_ptr<Render::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string &AnimatedSpriteName,
	const std::string &textureName,
	const std::string &shaderProgramName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight,
	const std::string& subTextureName)
{
	auto pTexture = getTexture2D(textureName);
	if (!pTexture){
		std::cerr << "Texture with this name can not be found (source: ResourceManager): " << textureName << std::endl;
		return nullptr;
	}

	auto pShaderProgram = getShaderProgram(shaderProgramName);
	if (!pShaderProgram){
		std::cerr << "Shader program with this name can not be found (source: ResourceManager): " << shaderProgramName << std::endl;
	}

	std::shared_ptr<Render::AnimatedSprite> pNewAnimatedSprite = std::make_shared<Render::AnimatedSprite>(pTexture, subTextureName, pShaderProgram, glm::vec2(0.f, 0.f), glm::vec2(spriteWidth, spriteHeight), 0.f);
	auto isNewSpriteAdd = _AnimatedSpriteMaps.emplace(AnimatedSpriteName, pNewAnimatedSprite);
	return isNewSpriteAdd.first->second;
}

std::shared_ptr<Render::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string &AnimatedSpriteName)
{
	AnimatedSpriteMap::const_iterator it = _AnimatedSpriteMaps.find(AnimatedSpriteName);
	if (it != _AnimatedSpriteMaps.end()){
		return it->second;
	}
	std::cerr << "Can not find animated sprite (source: ResourceManager): " << AnimatedSpriteName << std::endl;
	return nullptr;
}