#pragma once
#ifndef UNREALRAY_UNREALRENDER
#define UNREALRAY_UNREALRENDER


#include<string>
#include"Scene.h"
namespace unreal
{
	class UnrealRender
	{
	public:
		UnrealRender();
		~UnrealRender();
	public:
		void readFile();
		Scene parseFile(char * path);
	};
}




#endif // !UNREALRAY_UNREALRENDER



