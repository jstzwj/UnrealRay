#pragma once
#ifndef UNREALRAY_SCENE
#define UNREAlrAY_SCENE
#include<vector>
#include<memory>
#include"Light.h"
#include"Sampler.h"
#include"Film.h"
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
        std::unique_ptr<Film> film;
		
	public:
		void render();
		int getProgress() { return progress; }
        void setFilm();
        const Film& getFilm();
	};
}


#endif // !UNREALRAY_SCENE


