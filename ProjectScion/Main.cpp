#include "ScionEngine.h"

int main()
{
	std::unique_ptr<ScionEngine> engine(new ScionEngine());

	engine->Go();

	return EXIT_SUCCESS;
}