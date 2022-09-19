#include "pch.h"

#include "Image.h"

Image::Image() : fileName("")
{
}

Image::~Image()
{
	delete[] pixelData;
}

void Image::loadFromFile(std::string fileName)
{
	this->fileName = fileName;
	readHeaders();
	readPixelData();
	printInfo();
}

void Image::readHeaders()
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	std::ifstream fileHandle(fileName, std::ios::binary);
	if (fileHandle.is_open())
	{
		fileHandle.read((char*)(&fileHeader), sizeof(BITMAPFILEHEADER));
		fileHandle.read((char*)(&infoHeader), sizeof(BITMAPINFOHEADER));
		fileHandle.close();
	}
	else
	{
		std::cout << "An error occured when opening a file." << std::endl;
	}

	pixelDataOffset = fileHeader.bfOffBits;
	bitsPerPixel = infoHeader.biBitCount;
	height = infoHeader.biHeight;
	width = infoHeader.biWidth;
}

void Image::printInfo()
{
	std::cout << "Read Image from file " << fileName << ", size " << width << " x " << height << ", " << bitsPerPixel << "-bits" << std::endl;
}

void Image::readPixelData()
{
	// Calculate proper byte line width
	// ERROR: this is magic number, works only for 24-bit
	const int MAGIC = bitsPerPixel / 8;
	const DWORD byteLineSize = width * MAGIC;

	// Calculate padding
	auto padding = calculatePadding(byteLineSize);

	// Allocate memory for pixel data
	pixelData = new BYTE[height * byteLineSize];

	// Go to the beginning of PixelData
	std::ifstream fileHandle(fileName, std::ios::binary);
	if (fileHandle.is_open())
	{
		// Skip to the pixel data
		fileHandle.seekg(pixelDataOffset);
		for (int i = height - 1; i >= 0; i--)
		{
			// Read "line"
			fileHandle.read((char*)&pixelData[i * byteLineSize], byteLineSize);

			// Skip padding
			fileHandle.seekg(padding, std::ios::cur);
		}
		fileHandle.close();
	}
	else
	{
		std::cout << "ERROR: File not opened!" << std::endl;
	}
}

int Image::calculatePadding(DWORD lineSize)
{
	auto padding = (lineSize % 4);
	return(padding ? 4 - padding : 0);
}

BYTE* Image::getPixelData()
{
	return pixelData;
}

DWORD Image::getWidth()
{
	return width;
}

DWORD Image::getHeight()
{
	return height;
}