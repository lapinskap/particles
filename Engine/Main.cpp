#include "System.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	try
	{
		System System;

		System.Run();

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		MessageBoxA(nullptr, e.what(), "Error", MB_OK);
		return 1;
	}
}