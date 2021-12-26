#include <Instruction.h>

Instruction::Instruction(const Rule& rule): is_reduce(true), reduce_rule(rule) {}

Instruction::Instruction(int node): is_reduce(false), dest_node(node) {}

bool Instruction::isReduce() const {
    return is_reduce;
}

bool Instruction::isShift() const {
    return !is_reduce;
}

const Rule& Instruction::getRule() const {
    return reduce_rule;
}

int Instruction::getNode() const {
    return dest_node;
}
