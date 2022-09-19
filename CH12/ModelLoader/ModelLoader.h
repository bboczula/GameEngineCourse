#pragma once

#ifdef MODELLOADER_EXPORTS
#define MODEL_LOADER_EXPORTS __declspec(dllexport)
#else
#define MODEL_LOADER_EXPORTS __declspec(dllimport)
#endif

#include <string>
#include <fstream>
#include <vector>

#include "Handler.h"
#include "VertexHandler.h"
#include "NormalHandler.h"
#include "FaceHandler.h"
#include "GroupHandler.h"
#include "TextureHandler.h"
#include "DefaultHandler.h"
#include "MiniMath.h"

class MODEL_LOADER_EXPORTS ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();
	void LoadFromFile(std::string filename);
	void GenerateNormals();
	void GenerateTangents();
	void DumpToFile(std::string filename);
	void ProcessLine(char* lineBuffer);
	Face GetFace(unsigned int index);
	MiniMath::Point3D GetVertex(unsigned int index);
	MiniMath::Vector3D GetNormal(unsigned int index);
	MiniMath::Vector3D GetTangent(unsigned int index);
	MiniMath::Point2D GetTextureCoord(unsigned int index);
	unsigned int GenNumOfFaces();
	unsigned int GetGroupOffset(const std::string& key);
	unsigned int GetGroupSize(const std::string& key);
	unsigned int GetNumOfGroups();
	bool HasGroup(const std::string& key);
private:
	VertexHandler* vertexHandler;
	NormalHandler* normalHandler;
	FaceHandler* faceHandler;
	GroupHandler* groupHandler;
	TextureHandler* textureHandler;
	DefaultHandler* defaultHandler;
	std::string currentFile;
};