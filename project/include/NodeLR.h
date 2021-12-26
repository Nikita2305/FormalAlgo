#pragma once

#include <SituationLR.h>
#include <Instruction.h>
#include <unordered_map>
#include <set>

bool compare_situations(const SituationLR& a, const SituationLR& b);

struct NodeLR {
    NodeLR();
    std::set<SituationLR, decltype(compare_situations)*> situations;
    std::unordered_map<char, Instruction> instructions;
};
