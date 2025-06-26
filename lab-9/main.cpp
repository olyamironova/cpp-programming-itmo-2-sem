#include <iostream>
#include <vector>
#include <map>
#include <adapters.h>

int main() {
	std::vector<int> test = { 1, 4, 6, 2, 7, 4, 1, 4 };
	auto removed = v1 | values({ 3, 2, 4 });
	for (auto i : removed) {
		std::cout << i.first << ' ' << i.second << std::endl;
	}
	return 0;
}