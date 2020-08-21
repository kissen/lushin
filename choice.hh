#pragma once

#include <cstdlib>
#include <optional>
#include <vector>

namespace choice
{
	void ensure_initialized();

	template <typename T>
	std::optional<T> make(const std::vector<T> &vec)
	{
		if (vec.empty()) {
			return std::nullopt;
		}

		ensure_initialized();

		// this is problematic, but I am too lazy to use
		// the proper C++ way right now
		const int idx = random() % vec.size();
		return std::make_optional(vec.at(idx));
	}
};
