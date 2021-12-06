#pragma once
#include <set>
#include <Configuration.h>

struct ConfigurationSet {
    std::set<Configuration> configs;
    int before_point;
};
