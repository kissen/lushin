#include "choice.hh"

void choice::ensure_initialized()
{
	static bool initialized = false;

	if (!initialized) {
		srand(time(NULL));
	}
}
