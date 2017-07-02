#pragma once
#ifndef UNREALRAY_SCENE
#define UNREAlrAY_SCENE
#include<vector>
#include"Light.h"
#include"Sampler.h"
namespace unreal
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		int progress;
		std::vector<Light> lights;
		Sampler *sampler;
		
	public:
		void render();
		int getProgress() { return progress; }
	};
}


#endif // !UNREALRAY_SCENE


