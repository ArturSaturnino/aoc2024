#pragma once

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <concepts>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>



std::vector<std::string> readFile(std::ifstream& file);
std::vector<int> readNums(const std::string& str);
std::vector<int64_t> readNumsl(const std::string& str);

constexpr bool isDigit(const char c)
{
	return static_cast<int>(c) >= static_cast<int>('0') && static_cast<int>(c) <= static_cast<int>('9');
}
constexpr int digitToInt(const char c)
{

	assert(isDigit(c));

	return static_cast<int>(c) - static_cast<int>('0');
}

constexpr uint32_t countDigits(std::integral auto num)
{
	if (num == 0)
		return 1;
	uint32_t count = 0;
	while (num /= 10 > 0)
		++count;
	return count;
}