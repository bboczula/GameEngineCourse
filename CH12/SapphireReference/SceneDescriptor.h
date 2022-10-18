#pragma once

#include <string>
#include <vector>

class SceneDescriptor
{
public:
	SceneDescriptor();
	~SceneDescriptor();
	void Load(std::string fileName);
	unsigned int Size();
	const std::string& GetMeshLibraryPath();
	const std::string& GetObjectName(unsigned int index);
	const std::string& GetColorTexturePath(unsigned int index);
	const std::string& GetBumpTexturePath(unsigned int index);
private:
	void ProcessLine(const std::string& line);
	std::string meshLibrary;
	std::vector<std::string> gameObjectNames;
	std::vector<std::string> gameObjectColor;
	std::vector<std::string> gameObjectBump;
};