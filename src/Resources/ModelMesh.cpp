#include "ModelMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Render/Texture2D.h"
#include "../Render/ShaderProgram.h"

#include <iostream>

ModelMesh::ModelMesh(const aiScene* pScene, VecTexGL vecTexGL)
{
	_vecTexGL = vecTexGL;
	processNode(pScene->mRootNode, pScene);
}


void ModelMesh::processNode(const aiNode* pNode, const aiScene* pScene)
{
	for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
	{
		auto mesh = pScene->mMeshes[pNode->mMeshes[i]];
		_vecMeshes.push_back(std::make_shared<BaseMesh>(mesh, pScene));
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		processNode(pNode->mChildren[i], pScene);
	}
}

void ModelMesh::draw(std::shared_ptr<Render::ShaderProgram> shader)
{


	//auto mMesh = _vecMeshes[0];
	//std::cout << "MESH NAME: " << mMesh->getMeshName() << std::endl;
	//auto mTextures = mMesh->getTexture();
	//std::cout << "TEXTURE COUNT: " << mTextures.size() << std::endl;

	//auto mTex = mTextures[0];
	//auto mTexture2D_GL = _vecTexGL[mTex._id];
	//std::cout << "TEXTURE NAME: " << mTexture2D_GL.first << std::endl;

	//mTexture2D_GL.second->bindTexture2D(0);
	//shader->setTexUniform("material.texture", 0);

	//mMesh->drawMesh();




	for (const auto& mesh : _vecMeshes)
	{
		auto mTextures = mesh->getTexture();
		for (const auto& mTex : mTextures)
		{
			std::cout << mTex._type << std::endl;
			auto mTexture2D_GL = _vecTexGL[mTex._id];
			mTexture2D_GL.second->bindTexture2D(0);
			shader->setTexUniform("material.texture", 0);
			mesh->drawMesh();
		}
	}
}