#pragma once

#include <Rule.h>

class Instruction {
public:
    explicit Instruction(const Rule& rule);
    explicit Instruction(int node);
    bool isReduce() const;
    bool isShift() const;
    const Rule& getRule() const;
    int getNode() const;
private:
    bool is_reduce;
    Rule reduce_rule;
    int dest_node;
};
