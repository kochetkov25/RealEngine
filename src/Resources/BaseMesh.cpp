#include "BaseMesh.h"

#include <iostream>

/*ctor*/
BaseMesh::BaseMesh(const aiMesh* pMesh, const aiScene* pScene)
{
	/*обработка возможных ошибок*/
	if (!pMesh)
	{
		std::cerr << "Mesh is nullptr! (source: " << __FUNCTION__ << ")" << std::endl;
		return;
	}
	if (!pScene)
	{
		std::cerr << "Scene is nullptr! (source: " << __FUNCTION__ << ")" << std::endl;
		return;
	}
	if (!pMesh->HasNormals())
	{
		std::cerr << "No normals! (source: " << __FUNCTION__ << ")" << std::endl;
		return;
	}
	if (!pMesh->HasPositions())
	{
		std::cerr << "No positions! (source: " << __FUNCTION__ << ")" << std::endl;
		return;
	}

	/*название меша*/
	_nameMesh = pMesh->mName.C_Str();

	/*передача параметров меша в рендер*/
	_renderer = Render::RendererFactory::CreateDefault3DModelRenderer();

	/*позиции, текстурные координаты, нормали*/
	for (int i = 0; i < pMesh->mNumVertices; i++)
	{
		/*позиция вершин*/
		_renderer->verex3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
		/*текстурные координаты*/
		_renderer->vertexUV(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y);
		/*нормали*/
		_renderer->verex3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z);
	}

	/*indices для данного меша*/
	std::vector<GLuint> indices;
	for (int i = 0; i < pMesh->mNumFaces; i++)
	{
		auto face = pMesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	_renderer->setIndices(indices);

	/*сохраняю тип и номер текстур, принадлежащих данному мешу*/
	auto pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
	for (int i = aiTextureType_NONE; i <= AI_TEXTURE_TYPE_MAX; i++)
	{
		auto cnt = pMaterial->GetTextureCount(static_cast<aiTextureType>(i));
		std::cout << "NAME: " << _nameMesh << " CNT: " << cnt << std::endl;
		if (cnt > 0)
		{
			aiString path;
			pMaterial->GetTexture(static_cast<aiTextureType>(i), 0, &path);
			unsigned int index = path.C_Str()[1] - '0';

			_vecTextures.push_back(Texture(index, static_cast<aiTextureType>(i)));
		}
	}
	std::cout << std::endl;

	_renderer->upload();
}

/*отрисовка меша*/
void BaseMesh::drawMesh()
{
	_renderer->drawElements();
}