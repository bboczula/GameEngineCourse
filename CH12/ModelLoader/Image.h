#pragma once

#ifdef MODELLOADER_EXPORTS
#define MODEL_LOADER_EXPORTS __declspec(dllexport)
#else
#define MODEL_LOADER_EXPORTS __declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <algorithm>

class MODEL_LOADER_EXPORTS Image
{
public:
	Image();
	~Image();
	void loadFromFile(std::string fileName);
	BYTE* getPixelData();
	DWORD getWidth();
	DWORD getHeight();
private:
	std::string fileName;
	WORD bitsPerPixel;
	DWORD pixelDataOffset;
	UINT numOfColorsInPalette;
	DWORD width;
	DWORD height;
	BYTE* pixelData;
private:
	void readHeaders();
	int calculatePadding(DWORD lineSize);
	void printInfo();
	void readPixelData();
};