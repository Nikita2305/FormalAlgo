#include <NodeLR.h>

bool compare_situations(const SituationLR& a, const SituationLR& b) {
    if (a.next != b.next) {
        return a.next < b.next;
    }
    if (a.point != b.point) {
        return a.point < b.point;
    }
    if (a.rule.premise != b.rule.premise) {
        return a.rule.premise < b.rule.premise;
    }
    return a.rule.result < b.rule.result;
}

NodeLR::NodeLR(): situations(compare_situations) {}
