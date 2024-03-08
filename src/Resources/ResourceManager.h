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
	// ����������� ��������� �� ���� ������ ���� �� exe �����
	// � ������������� � path ���������� exe �����
	ResourceManager(const std::string &executablePath);
	// ������� ��������� � ���� ������ ��������� ���������
	std::shared_ptr<Render::ShaderProgram> loadShederProgram(const std::string &shaderName, const std::string &vertexShaderPath, const std::string fragmentShaderPath);
	// ������� ���������� ��������� �� ��������� ��������� �� map �� ����� ���������
	std::shared_ptr<Render::ShaderProgram> getShaderProgram(const std::string &shaderName);
	// ������� �������� ��������� �������� � ��������� �� ������� + ���������� �������
	std::shared_ptr<Render::Texture2D> loadTexture2D(const std::string &textureName, const std::string &texturePath);
	// ������� ���������� ��������� �� 2� �������� �� ����� ��������
	std::shared_ptr<Render::Texture2D> getTexture2D(const std::string &texture2DName);
	// ������� ������� � ��������� � ��������� �������� ������
	std::shared_ptr<Render::Sprite> loadSprite(const std::string &spriteName,
											   const std::string &textureName,
											   const std::string &shaderProgramName,
											   const unsigned int spriteWidth,
											   const unsigned int spriteHeight,
											   const std::string& subTextureName = "default");
	// ������� ���������� ��������� �� ����������� ������ �� ����� �������
	std::shared_ptr<Render::Sprite> getSprite(const std::string &spriteName);
	// ������ ������� ��������� � ��� ��������, � � ������������ � �������� �������� ���������� ��������� 
	// � ��� ������� Texture2D ���������� ����������
	std::shared_ptr<Render::Texture2D> loadTextureAthlas2D(const std::string &texture2DName,
														   const std::string &texturePath,
														   std::vector<std::string> subTextureNames,
														   const unsigned int subTextureWidth,
														   const unsigned int subTextureHeight);



	//// �������� �������������� ������� (���������� �������� �������� �������)
	//std::shared_ptr<Render::AnimatedSprite> loadAnimatedSprite(const std::string &AnimatedSpriteName,
	//	const std::string &textureName,
	//	const std::string &shaderProgramName,
	//	const unsigned int spriteWidth,
	//	const unsigned int spriteHeight,
	//	const std::string& subTextureName = "default");
	//// �������� ��������� �� ������ ������������� ������.
	//std::shared_ptr<Render::AnimatedSprite> getAnimatedSprite(const std::string &AnimatedSpriteName);

private:
	// ������� ���������� ��� ���������� txt ����� �� ���� ������������ exe
	std::string getFileString(const std::string &relativeFilePath);
	// ������� ��������� ��� ���� ��������� ��������
	typedef std::map<const std::string, std::shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
	// ������� map ��� �������� ��������� ��������
	ShaderProgramsMap _shaderPrograms;

	// ������� ��������� ��� ���� �������
	typedef std::map<const std::string, std::shared_ptr<Render::Texture2D>> Texture2DMap;
	// ������� ��� ��� �������� �������
	Texture2DMap _texture2DMaps;

	// ������� ��������� ��� ���� ��������
	typedef std::map<const std::string, std::shared_ptr<Render::Sprite>> SpriteMap;
	SpriteMap _spriteMaps;

	//// ������� ��������� ��� ���� ������������� ��������
	//typedef std::map<const std::string, std::shared_ptr<Render::AnimatedSprite>> AnimatedSpriteMap;
	//AnimatedSpriteMap _AnimatedSpriteMaps;


	// ���� �� ����� � ���������
	std::string _path;
};

