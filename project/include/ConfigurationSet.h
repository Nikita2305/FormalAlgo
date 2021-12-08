#pragma once
#include <set>
#include <Configuration.h>

bool compare_configs(const Configuration& a, const Configuration& b);

struct ConfigurationSet {
    ConfigurationSet();
    std::set<Configuration, decltype(compare_configs)*> configs;
    int before_point;
};
