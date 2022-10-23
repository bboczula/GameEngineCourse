#pragma once

#include <string>
#include <vector>

#include "Optional.h"

struct ObjectData
{
    Optional<std::string> name;
    Optional<std::string> colorTextureName;
    Optional<std::string> bumpTextureName;
};

class SceneDescriptor
{
public:
	SceneDescriptor();
	~SceneDescriptor();
	void Load(std::string fileName);
	unsigned int Size();
	const std::string& GetMeshLibraryPath();
    const ObjectData& GetObjectData(unsigned int index);
private:
	void ProcessLine(const std::string& line);
	std::string meshLibrary;
    std::vector<ObjectData> objectData;
};