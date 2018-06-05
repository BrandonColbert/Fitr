#include "FitrManager.h"

int main(int argc, char *argv[]) {
	FitrManager fitr(argc, argv);
	fitr.start();
	while(true);

	return 0;
}