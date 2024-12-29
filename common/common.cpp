#include "common.h"

std::vector<std::string> readFile(std::ifstream& file)
{
	std::vector<std::string> out;

	for (std::string line; std::getline(file, line); )
	{
		out.emplace_back(line);
	}
	return out;
}

std::vector<int> readNums(const std::string& str)
{
	std::vector<int> nums{};
	std::regex numRegex("\\d+");

	auto matches = std::sregex_iterator(str.begin(), str.end(), numRegex);
	for (auto it = matches; it != std::sregex_iterator(); ++it)
	{
		nums.push_back(std::stoi(it->str()));
	}

	return nums;
}

std::vector<int64_t> readNumsl(const std::string& str)
{
	std::vector<int64_t> nums{};
	std::regex numRegex("-?\\d+");

	auto matches = std::sregex_iterator(str.begin(), str.end(), numRegex);
	for (auto it = matches; it != std::sregex_iterator(); ++it)
	{
		nums.push_back(std::stoll(it->str()));
	}

	return nums;
}

