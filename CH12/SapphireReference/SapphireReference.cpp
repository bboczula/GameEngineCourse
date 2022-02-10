// SapphireReference.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "SierpinskiTriangle.h"
#include "Triangle.h"
#include "../SapphireEngine/Engine.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
    Sapphire::Engine engine;

    SierpinskiTriangle sierpinskiTriangle;
    engine.Register(&sierpinskiTriangle);

    //Triangle triangle;
    //engine.Register(&triangle);
    engine.Run();
}
