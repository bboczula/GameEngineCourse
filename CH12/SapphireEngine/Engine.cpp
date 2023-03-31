#include "Engine.h"
#include "Renderer.h"
#include <filesystem>

Sapphire::Engine::Engine(UINT width, UINT height) : WindowApplication{ width, height }, renderer{ nullptr }
{
	Logger::GetInstance().Log("%s", "Sapphire::Engine::Engine()\n");

	newRenderer = new Renderer(hwnd, width, height);

	imGuiHandler = new ImGuiHandler();
	Attach(imGuiHandler);
	input = new RawInput();
	Attach(input);
	winMouse = new WinMouse();
	Attach(winMouse);

	modelLoader = new ModelLoader;
}

Sapphire::Engine::~Engine()
{
	delete modelLoader;

	Logger::GetInstance().Log("%s", "Sapphire::Engine::~Engine()\n");
	if (renderer)
	{
		delete renderer;
	}

#if _DEBUG
	ReportLiveObjects();
#endif
}

void Sapphire::Engine::Register(GameObject* gameObject)
{
	Logger::GetInstance().Log("%s", "Sapphire::Engine::Register()\n");

	gameObjects.push_back(gameObject);
}

void Sapphire::Engine::RegisterCamera(Camera* camera)
{
	//renderer->SetCamera(camera);
	newRenderer->SetCamera(camera);
}

void Sapphire::Engine::LoadModel(GameObject* gameObject, const std::string& filePath, const std::string& groupName)
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath.append("assets");
	currentPath.append(filePath);
	Logger::GetInstance().Log("Current Directory %s\n", currentPath.string().c_str());
	modelLoader->LoadFromFile(currentPath.string());

	// Fetch the group data
	auto groupOffset = modelLoader->GetGroupOffset(groupName);
	auto groupSize = modelLoader->GetGroupSize(groupName);

	// Here you have to manually adapt the results
	//VertexDefinition* output = new VertexDefinition[modelLoader->GenNumOfFaces() * 3];
	//UINT* outputIndices = new UINT[modelLoader->GenNumOfFaces() * 3];
	//VertexDefinition* output = new VertexDefinition[groupSize * 3];
	// for (int i = 0; i < modelLoader->GenNumOfFaces(); i++)

	UINT* outputIndices = new UINT[groupSize * 3];
	for (int i = 0; i < groupSize; i++)
	{
		auto face = modelLoader->GetFace(i + groupOffset);
		for (int j = 0; j < 3; j++)
		{
			outputIndices[i * 3 + j] = i * 3 + j;
		}
	}

	// START: Flexible Vertex Buffers (FVB) !--
	gameObject->position = new DirectX::SimpleMath::Vector4[groupSize * 3];
	for (int i = 0; i < groupSize; i++)
	{
		auto face = modelLoader->GetFace(i + groupOffset);
		for (int j = 0; j < 3; j++)
		{
			auto vertex = modelLoader->GetVertex(face.vertexIndex[j]);
			gameObject->position[i * 3 + j] = { vertex.x, vertex.y, vertex.z, 1.0f };
		}
	}
	// END: Flexible Vertex Buffers (FVB) !--

	// START: Flexible Vertex Buffers (FVB) !--
	gameObject->normal = new DirectX::SimpleMath::Vector4[groupSize * 3];
	for (int i = 0; i < groupSize; i++)
	{
		auto face = modelLoader->GetFace(i + groupOffset);
		for (int j = 0; j < 3; j++)
		{
			auto normal = modelLoader->GetNormal(face.normalIndex[j]);
			gameObject->normal[i * 3 + j] = { normal.x, normal.y, normal.z, 0.0f };
		}
	}
	// END: Flexible Vertex Buffers (FVB) !--

	// START: Flexible Vertex Buffers (FVB) !--
	gameObject->tangent = new DirectX::SimpleMath::Vector4[groupSize * 3];
	for (int i = 0; i < groupSize; i++)
	{
		auto face = modelLoader->GetFace(i + groupOffset);
		for (int j = 0; j < 3; j++)
		{
			auto tangent = modelLoader->GetTangent(face.tangentIndex[j]);
			gameObject->tangent[i * 3 + j] = { tangent.x, tangent.y, tangent.z, 0.0f };
		}
	}
	// END: Flexible Vertex Buffers (FVB) !--

	// START: Flexible Vertex Buffers (FVB) !--
	gameObject->albedoTexCoord = new DirectX::SimpleMath::Vector2[groupSize * 3];
	for (int i = 0; i < groupSize; i++)
	{
		auto face = modelLoader->GetFace(i + groupOffset);
		for (int j = 0; j < 3; j++)
		{
			auto texCoord = modelLoader->GetTextureCoord(face.textureIndex[j]);
			gameObject->albedoTexCoord[i * 3 + j] = { texCoord.x, texCoord.y };
		}
	}
	// END: Flexible Vertex Buffers (FVB) !--

	// Here you would have to somehow make unique vertices. I wonder how I did it before. Or I have just created a non-indexed vertex buffer? Probably
	gameObject->numOfVertices = groupSize * 3;
	gameObject->indices = outputIndices;
	gameObject->numOfIndices = groupSize * 3;
	gameObject->name = groupName;

	Logger::GetInstance().Log("Loaded object %s\n", groupName.c_str());
}

void Sapphire::Engine::LoadDefaultTexture(GameObject* gameObject)
{
	gameObject->textureWidth = gameObject->textureHeight = 256;
	auto bufferSize = gameObject->textureWidth * gameObject->textureHeight;
	gameObject->pixels = new PixelDefinition[bufferSize];

	// Here fill it with sometching, a single color for now should be fine
	for (int i = 0; i < bufferSize; i++)
	{
		if (i < 256 || (i > 256 * 255) || ((i % 8) == 0))
		{
			//gameObject->pixels[i] = { 1.0f, 1.0f, 1.0f, 1.0f };
			gameObject->pixels[i] = { 0.0f, 0.0f, 0.0f, 1.0f };
		}
		else
		{
			int division = i / 16;
			gameObject->pixels[i] = { 1.0f, division % 2 ? 1.0f : 0.0f, 0.0f, 1.0f };
		}
	}
}

void Sapphire::Engine::LoadTextureFromFile(GameObject* gameObject, std::string filePath)
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath.append("assets");
	currentPath.append(filePath);

	std::string extension = currentPath.extension().string();
	Image image;
	// This condition has to be able to 
	if (extension.compare(".bmp") == 0)
	{
		image.loadFromFileBmp(currentPath.string());
	}
	else if (extension.compare(".png") == 0)
	{
		image.loadFromFilePng(currentPath.string());
	}

	gameObject->textureWidth = image.getWidth();
	gameObject->textureHeight = image.getHeight();
	auto bufferSize = gameObject->textureWidth * gameObject->textureHeight;
	gameObject->pixels = new PixelDefinition[bufferSize];

	unsigned int index = 0;
	auto sourceImage = image.getPixelData();
	for (int i = 0; i < bufferSize; i++)
	{
		if (image.getFormat() == Image::B8G8R8)
		{
			gameObject->pixels[i] = { (float)(sourceImage[index + 2] / 255.0f), (float)(sourceImage[index + 1] / 255.0f), (float)(sourceImage[index + 0] / 255.0f), 1.0f };
			index += 3;
		}
		else if (image.getFormat() == Image::R8G8B8A8)
		{
			gameObject->pixels[i] = { (float)(sourceImage[index + 0] / 255.0f), (float)(sourceImage[index + 1] / 255.0f), (float)(sourceImage[index + 2] / 255.0f), (float)(sourceImage[index + 3] / 255.0f) };
			index += 4;
		}
	}
}

void Sapphire::Engine::LoadBumpMapFromFile(GameObject* gameObject, std::string filePath)
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath.append("assets");
	currentPath.append(filePath);

	std::string extension = currentPath.extension().string();
	Image image;
	// This condition has to be able to 
	if (extension.compare(".bmp") == 0)
	{
		image.loadFromFileBmp(currentPath.string());
	}
	else if (extension.compare(".png") == 0)
	{
		image.loadFromFilePng(currentPath.string());
	}

	gameObject->bumpMapWidth = image.getWidth();
	gameObject->bumpMapHeight = image.getHeight();
	auto bufferSize = gameObject->bumpMapWidth * gameObject->bumpMapHeight;
	gameObject->bumpMapPixels = new PixelDefinition[bufferSize];

	unsigned int index = 0;
	auto sourceImage = image.getPixelData();
	for (int i = 0; i < bufferSize; i++)
	{
		if (image.getFormat() == Image::B8G8R8)
		{
			gameObject->bumpMapPixels[i] = { (float)(sourceImage[index + 2] / 255.0f), (float)(sourceImage[index + 1] / 255.0f), (float)(sourceImage[index + 0] / 255.0f), 1.0f };
			index += 3;
		}
		else if (image.getFormat() == Image::R8G8B8A8)
		{
			gameObject->bumpMapPixels[i] = { (float)(sourceImage[index + 0] / 255.0f), (float)(sourceImage[index + 1] / 255.0f), (float)(sourceImage[index + 2] / 255.0f), (float)(sourceImage[index + 3] / 255.0f) };
			index += 4;
		}
	}
}

RawInput* Sapphire::Engine::GetInput()
{
	return input;
}

void Sapphire::Engine::Initialize()
{
	// Initialize base class stuff too
	WindowApplication::Initialize();

	Logger::GetInstance().Log("%s", "Sapphire::Engine::Initialize()\n");

	//renderer->CreateResources(gameObjects);
	newRenderer->CreateResources(gameObjects);

	// Initialize all the resources
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Initialize();
	}
}

void Sapphire::Engine::Tick()
{
	// Check for Esc key
	if (input->isKeyDown(VK_ESCAPE))
	{
		exit(0);
	}

	// Update all game objects
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update(0.0f);
	}

	// Render from Render Context
	// renderer->Render(gameObjects);
	newRenderer->Render(gameObjects);

	// Execute the command list
	//renderer->Execute();
	newRenderer->Execute();
}

void Sapphire::Engine::ReportLiveObjects()
{
	IDXGIDebug* dxgiDebugInterface;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebugInterface))))
	{
		dxgiDebugInterface->ReportLiveObjects(DXGI_DEBUG_ALL,
			DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}
	SafeRelease(&dxgiDebugInterface);
}
