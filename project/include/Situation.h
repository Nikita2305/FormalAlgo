#pragma once
#include <Rule.h>

struct Situation {
    Rule rule; 
    int point; // 0..len(rule)
};
