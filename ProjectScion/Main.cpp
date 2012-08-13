#include "ScionEngine.h"
#include <Windows.h>

int main()
{
	srand(GetTickCount());
	std::unique_ptr<ScionEngine> engine(new ScionEngine());

	engine->Go();

	return EXIT_SUCCESS;
}