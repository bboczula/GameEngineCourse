#pragma once

#include "Model.h"
#include "InputFile.h"
#include "GameObject.h"
#include "VertexDefinition.h"

#include <string>
#include <fstream>
#include <vector>
#include <cassert>

namespace Sapphire
{
	class Handler
	{
	public:
		Handler() : next(nullptr)
		{
		}
		Handler* SetNext(Handler* handler)
		{
			return next = handler;
		}
		virtual bool Condition(const std::vector<std::string>& tokens) = 0;
		virtual void Handle(const std::vector<std::string>& tokens) = 0;
		virtual void Reset() = 0;
		Handler* next;
	};

	class VertexHandler : public Handler
	{
	public:
		bool Condition(const std::vector<std::string>& tokens)
		{
			return (tokens[0].compare("v") == 0);
		}
		void Handle(const std::vector<std::string>& tokens) override
		{
			// Logger::GetInstance().Log("I will handle this vertex.\n");

			// We don't know how many vertices each vertex has, maybe three or four
			// This is not true, this is always going to be 3
			for (int i = 1; i < tokens.size(); i++)
			{
				position.push_back(std::stof(tokens[i]));
			}
		}
		void Reset() override
		{
			position.clear();
		}
		std::vector<float> position;
	};

	class NormalHandler : public Handler
	{
	public:
		bool Condition(const std::vector<std::string>& tokens)
		{
			return (tokens[0].compare("vn") == 0);
		}
		void Handle(const std::vector<std::string>& tokens) override
		{
			// Logger::GetInstance().Log("I will handle this vertex.\n");

			// We don't know how many vertices each vertex has, maybe three or four
			// This is not true, this is always going to be 3
			for (int i = 1; i < tokens.size(); i++)
			{
				normal.push_back(std::stof(tokens[i]));
			}
		}
		void Reset() override
		{
			normal.clear();
		}
		std::vector<float> normal;
	};

	class FaceHandler : public Handler
	{
	public:
		bool Condition(const std::vector<std::string>& tokens)
		{
			return (tokens[0].compare("f") == 0);
		}
		void Handle(const std::vector<std::string>& tokens) override
		{
			// We have to tokenize again this time on \
			// This code should be extracted to a function probably, duplication
			// Tokenize
			

			// Logger::GetInstance().Log("I will handle this face.\n");

			// Here the situation can be complex, you can have vertex or normal or texture coordinates
			// On top of that, you can either have 3 or 4 elements

			// In the simple case, we should have 3 vertices per face and each is a UINT
			for (int i = 1; i < tokens.size(); i++)
			{
				std::vector<std::string> innerTokens;
				char* first;
				char* next = NULL;
				first = strtok_s((char*)tokens[i].c_str(), "/", &next);
				do
				{
					innerTokens.push_back(first);
					if (next[0] == '/')
					{
						// This means that we have empty element
						innerTokens.push_back(std::string());
					}
				} while ((first = strtok_s(NULL, "/", &next)) != NULL);

				// Start indexing from 0, not 1 like in files
				indices.push_back(std::stoi(innerTokens[0]) - 1);
				normalIndices.push_back(std::stoi(innerTokens[2]) - 1);
			}
		}
		void Reset() override
		{
			indices.clear();
			normalIndices.clear();
		}
		std::vector<UINT> indices;
		std::vector<UINT> normalIndices;
	};

	class DefaultHandler : public Handler
	{
	public:
		bool Condition(const std::vector<std::string>& tokens)
		{
			return true;
		}
		void Handle(const std::vector<std::string>& tokens) override
		{
			// Logger::GetInstance().Log("I couldn't handle this.\n");
		}
		void Reset() override
		{
		}
	};

	class ModelLoaderSapphire
	{
	public:
		ModelLoaderSapphire();
		~ModelLoaderSapphire();
		Model* LoadFromFile(std::string filePath);
		void ProcessLine(char* lineBuffer);
		void FillData(GameObject* gameObject);
	private:
		VertexHandler* vertexHandler;
		NormalHandler* normalHandler;
		FaceHandler* faceHandler;
		DefaultHandler* defaultHandler;
	};
}