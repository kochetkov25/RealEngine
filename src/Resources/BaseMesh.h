#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <string>

#include "../Render/Renderer.h"

namespace Render
{
	class ShaderProgram;
}

class BaseMesh
{
public:
	/*ctor*/
	BaseMesh(const aiMesh* pMesh, const aiScene* pScene);
	/*copy ctor*/
	BaseMesh(const BaseMesh&) = default;
	/*move ctor*/
	BaseMesh(BaseMesh&&) noexcept = default;
	/*dtor*/
	~BaseMesh() noexcept = default;

	/*структура для сохранения текстур, принадлежащих мешу*/
	struct Texture
	{
		Texture(unsigned int id, aiTextureType type) : 
			_id(id), _type(type)
		{}
		unsigned int _id;
		aiTextureType _type;
	};

	/*отрисовка меша*/
	void drawMesh();

	/*получить вектор текстур*/
	std::vector<Texture> getTexture()
	{
		return _vecTextures;
	}

	/*получить название меша*/
	std::string getMeshName()
	{
		return _nameMesh;
	}
private:
	Render::Renderer _renderer;
	std::string _nameMesh;

	std::vector<Texture> _vecTextures;
};