#include "ModelLoaderSapphire.h"

Sapphire::ModelLoaderSapphire::ModelLoaderSapphire()
{
	vertexHandler = new VertexHandler;
	normalHandler = new NormalHandler;
	faceHandler = new FaceHandler;
	defaultHandler = new DefaultHandler;

	// Define chain of responsibility
	vertexHandler->SetNext(normalHandler)->SetNext(faceHandler)->SetNext(defaultHandler);
}

Sapphire::ModelLoaderSapphire::~ModelLoaderSapphire()
{
	if (vertexHandler)
	{
		delete vertexHandler;
	}

	if (normalHandler)
	{
		delete normalHandler;
	}

	if (faceHandler)
	{
		delete faceHandler;
	}

	if (defaultHandler)
	{
		delete defaultHandler;
	}
}

Model* Sapphire::ModelLoaderSapphire::LoadFromFile(std::string filePath)
{
	// Reset
	// Chain of responsibility
	Handler* current = vertexHandler;
	do
	{
		current->Reset();
		current = current->next;
	} while (current);

	// Process file
	std::ifstream file(filePath);
	std::string line;
	while (std::getline(file, line))
	{
		ProcessLine((char*)line.c_str());
	}
	file.close();

	return nullptr;
}

void Sapphire::ModelLoaderSapphire::ProcessLine(char* lineBuffer)
{
	char* first;
	char* next = NULL;

	std::vector<std::string> tokens;

	// Tokenize
	first = strtok_s(lineBuffer, " ", &next);
	do
	{
		tokens.push_back(first);
	}
	while ((first = strtok_s(NULL, " ", &next)) != NULL);

	// Chain of responsibility
	Handler* current = vertexHandler;
	do
	{
		if (current->Condition(tokens))
		{
			current->Handle(tokens);
			break;
		}
		current = current->next;
	}
	while (current);
}

void Sapphire::ModelLoaderSapphire::FillData(GameObject* gameObject)
{
	VertexDefinition* output = new VertexDefinition[faceHandler->indices.size()];
	UINT* outputIndices = new UINT[faceHandler->indices.size()];
	for (int i = 0; i < faceHandler->indices.size(); i++)
	{
		output[i].position.x = vertexHandler->position[faceHandler->indices[i] * 3 + 0];
		output[i].position.y = vertexHandler->position[faceHandler->indices[i] * 3 + 1];
		output[i].position.z = vertexHandler->position[faceHandler->indices[i] * 3 + 2];
		output[i].position.w = 1.0f;
		output[i].normal.x = normalHandler->normal[faceHandler->normalIndices[i] * 3 + 0];
		output[i].normal.y = normalHandler->normal[faceHandler->normalIndices[i] * 3 + 1];
		output[i].normal.z = normalHandler->normal[faceHandler->normalIndices[i] * 3 + 2];
		output[i].normal.w = 0.0f;
		outputIndices[i] = i;
	}

	// Here you would have to somehow make unique vertices. I wonder how I did it before. Or I have just created a non-indexed vertex buffer? Probably
	gameObject->numOfVertices = faceHandler->indices.size();
	gameObject->indices = outputIndices;
	gameObject->numOfIndices = faceHandler->indices.size();
}
