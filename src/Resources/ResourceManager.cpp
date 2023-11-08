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
// ����������� ���������� stbi. ������ �� ������������� ����� �������� ����� png

// ����������� ��������� �� ���� ������ ���� �� exe �����
// � ������������� � path ���������� exe �����
ResourceManager::ResourceManager(const std::string &executablePath)
{
	// � ������� ���������� ���� �� exe �����.
	// ����� ���� ������ ��������� / ��� '\\'
	size_t found = executablePath.find_last_of("/\\");
	// �������� ������ �� ��������� �������
	_path = executablePath.substr(0, found);
}

// ������� ���������� ��� ���������� txt ����� �� ���� ������������ exe
std::string ResourceManager::getFileString(const std::string &relativeFilePath)
{
	// �� ���� �������� ������������� ������������ �����
	std::fstream file;
	file.open(_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	// ���� ���� �� ������� �������, ���������� ������ ������
	if (!file.is_open()){
		std::cerr << "Failed open file (source: ResourceManager): " << relativeFilePath << std::endl;
		return std::string();
	}
	// ���� �������, ��������� ��� ���������� � ������ � ����� ���������� �� ������� �� ������
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

// ������� ��������� � ���� ������ ��������� ���������
std::shared_ptr<Render::ShaderProgram> ResourceManager::loadShederProgram(const std::string &shaderName, const std::string &vertexShaderPathRelative, const std::string fragmentShaderPathRelative)
{
	// �������� � ������ ��� ���������� �������
	std::string vertexString = getFileString(vertexShaderPathRelative);
	if (vertexString.empty()){
		std::cerr << "Failed to load VERTEX SHADER. (source: ResourceManager)" << std::endl;
		return nullptr;
	}
	// �������� � ������ ��� ������������ �������
	std::string fragmentString = getFileString(fragmentShaderPathRelative);
	if (fragmentString.empty()){
		std::cerr << "Failed to load FRAGMENT SHADER. (source: ResourceManager)" << std::endl;
		return nullptr;
	}
	// ������� ��������� �� ����� ��������� ���������
	std::shared_ptr<Render::ShaderProgram> pNewShaderProgram = std::make_shared<Render::ShaderProgram>(vertexString, fragmentString);
	// ��������� ���������� �� ������� ��������� ���������
	if (!pNewShaderProgram->isCompiled()){
		std::cerr << "Can not create new shader program. Path to shaders: \n"
			<< "Vertex shader: " << vertexShaderPathRelative << "\n"
			<< "Fragment shader: " << fragmentShaderPathRelative <<"\n"
			<<"(source: ResourceManager)"<<std::endl;
		return nullptr;
	}
	// ��������� ��������� ��������� � map � ���������� ��������� �� ���
	auto isShaderProgramAdd = _shaderPrograms.emplace(shaderName, pNewShaderProgram);
	return isShaderProgramAdd.first->second;
}
// ������� ���������� ��������� �� ��������� ��������� �� map �� ����� ���������
std::shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(const std::string &shaderName)
{
	ShaderProgramsMap::const_iterator it = _shaderPrograms.find(shaderName);
	if (it != _shaderPrograms.end()){
		return it->second;
	}
	std::cerr << "Can not find shader program (source: ResourceManager): " << shaderName << std::endl;
	return nullptr;
}

// ������� �������� ��������� �������� � ��������� �� ������� + ���������� �������
std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture2D(const std::string &textureName, const std::string &texturePathRelative)
{
	int chanels = 0;
	int width = 0;
	int height = 0;
	// � OpenGl ���������� ������� �������� �� ������� ������ ����. � stbi �� �������� �������. ����� ������ ��� ��������, 
	// ������������� ���� ��� ���������� ��������
	stbi_set_flip_vertically_on_load(true);
	// ��������� ��������
	unsigned char *pixelsArr=stbi_load(std::string(_path + "/" + texturePathRelative).c_str(), &width, &height, &chanels, 0);

	if (!pixelsArr){
		std::cerr << "Can not load texture image (source: ResourceManager): " << texturePathRelative << std::endl;
		return nullptr;
	}

	// ������� �������� � ��������� �� ���
	std::shared_ptr<Render::Texture2D> pNewTexture2D = std::make_shared<Render::Texture2D>(width, height, pixelsArr, chanels, GL_NEAREST, GL_CLAMP_TO_EDGE);
	// ��������� �������� � ���
	auto isNewTexture2DAdd = _texture2DMaps.emplace(textureName, pNewTexture2D);
	// ����������� ������, ������� ������ stbi ��� �������� ��������
	stbi_image_free(pixelsArr);
	
	// ���������� ��������� �� ��������� ��������
	return isNewTexture2DAdd.first->second;
}

// ������� ���������� ��������� �� 2� �������� �� ����� ��������
std::shared_ptr<Render::Texture2D> ResourceManager::getTexture2D(const std::string &texture2DName)
{
	Texture2DMap::const_iterator it = _texture2DMaps.find(texture2DName);
	if (it != _texture2DMaps.end()){
		return it->second;
	}
	std::cerr << "Can not find texture 2D (source: ResourceManager): " << texture2DName << std::endl;
	return nullptr;
}

// ������� ������� � ��������� � ��������� �������� ������
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

// ������� ���������� ��������� �� ����������� ������ �� ����� �������
std::shared_ptr<Render::Sprite> ResourceManager::getSprite(const std::string &spriteName)
{
	SpriteMap::const_iterator it = _spriteMaps.find(spriteName);
	if (it != _spriteMaps.end()){
		return it->second;
	}
	std::cerr << "Can not find sprite (source: ResourceManager): " << spriteName << std::endl;
	return nullptr;
}

// ������ ������� ��������� � ��� ��������, � � ������������ � �������� �������� ���������� ��������� 
// � ��� ������� Texture2D ���������� ����������
std::shared_ptr<Render::Texture2D> ResourceManager::loadTextureAthlas2D(const std::string &texture2DName,
	const std::string &texturePath,
	std::vector<std::string> subTextureNames,
	const unsigned int subTextureWidth,
	const unsigned int subTextureHeight)
{
	// ��������� �������� � ������ ������
	auto pTexture = loadTexture2D(texture2DName, texturePath);
	if (pTexture)
	{
		// �������� ������ � ������ ��������
		const unsigned int textureWidth = pTexture->getWidth();
		const unsigned int texturHeight = pTexture->getWidth();
		// ������������� ��������� ��������� (����� ������� ����)
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = texturHeight;

		// ���������� �� ����� ������� ���� ��� ������� � ��������� ����������  
		// ������� ������ ���� � �������� ������� ���� �����������
		for (auto &currentSubtextureName : subTextureNames)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
								   static_cast<float>(currentTextureOffsetY - subTextureHeight) / texturHeight);

			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
								 static_cast<float>(currentTextureOffsetY) / texturHeight);

			// ����� ����������, ��������� � ������ ������ Texture2D ��� ����������� � ���������� �����
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