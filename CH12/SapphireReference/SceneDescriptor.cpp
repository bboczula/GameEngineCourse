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
    return objectData.size();
}

const std::string& SceneDescriptor::GetMeshLibraryPath()
{
    return meshLibrary;
}

const std::string& SceneDescriptor::GetSceneName()
{
      return sceneName;
}

const ObjectData& SceneDescriptor::GetObjectData(unsigned int index)
{
    return objectData[index];
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
    else if (tokens[0].compare("sceneName") == 0)
    {
          sceneName = tokens[1];
    }
    else if(tokens[0].compare("staticObject") == 0)
    {
        ObjectData temp;
        temp.name.Set(tokens[1]);
        temp.colorTextureName.Set(tokens[2]);
        temp.bumpTextureName.Set(tokens[3]);
        objectData.push_back(temp);
    }
}
