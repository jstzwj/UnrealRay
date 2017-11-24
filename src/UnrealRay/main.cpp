#include"driver/unreal.h"
int main(int argc, char *argv[])
{
	unreal::Unreal engine;
	engine.render();
	unreal::Film * film = &*engine.integrator->camera->film;
	film->writeImage();
    return 0                                           ;
}
