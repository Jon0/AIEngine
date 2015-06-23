#pragma once

#include <string>
#include <unordered_map>

namespace graph {

class Resource;

class Location {
public:
	Location(std::string name);

private:
	std::string location_name;
	std::unordered_map<Resource *, double> res;

};

}
