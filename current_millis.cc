#include <cstdio>
#include <cstdlib>
#include <sys/time.h>

#include "timer.hh"

uint64_t timer::current_millis()
{
	struct timeval tv;

	if (gettimeofday(&tv, nullptr) == -1) {
		perror("gettimeofday");
		exit(EXIT_FAILURE);
	}

	const uint64_t seconds = static_cast<uint64_t>(tv.tv_sec);
	const uint64_t usecs = static_cast<uint64_t>(tv.tv_usec);

	return seconds * 1000ULL + usecs / 1000ULL;
}
