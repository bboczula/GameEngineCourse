#include <fstream>

#include "SceneDescriptor.h"
#include "../SapphireEngine/Logger.h"

SceneDescriptor::SceneDescriptor()
{
}

SceneDescriptor::~SceneDescriptor()
{
}

void SceneDescriptor::Load(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::string meshLibrary;
    while (std::getline(file, line))
    {
        ProcessLine(line);
    }
    file.close();
}

unsigned int SceneDescriptor::Size()
{
    return gameObjectNames.size();
}

const std::string& SceneDescriptor::GetMeshLibraryPath()
{
    return meshLibrary;
}

const std::string& SceneDescriptor::GetObjectName(unsigned int index)
{
    return gameObjectNames[index];
}

const std::string& SceneDescriptor::GetColorTexturePath(unsigned int index)
{
    return gameObjectColor[index];
}

const std::string& SceneDescriptor::GetBumpTexturePath(unsigned int index)
{
    return gameObjectBump[index];
}

void SceneDescriptor::ProcessLine(const std::string& line)
{
    char* first;
    char* next = NULL;

    std::vector<std::string> tokens;

    // Tokenize
    first = strtok_s((char*)line.c_str(), " ", &next);

    if (!first)
    {
        return;
    }

    do
    {
        tokens.push_back(first);
    } while ((first = strtok_s(NULL, " ", &next)) != NULL);

    if (tokens[0].compare("meshLibrary") == 0)
    {
        meshLibrary = tokens[1];
    }
    else if(tokens[0].compare("staticObject") == 0)
    {
        gameObjectNames.push_back(tokens[1]);
        gameObjectColor.push_back(tokens[2]);
        gameObjectBump.push_back(tokens[3]);
    }
}
