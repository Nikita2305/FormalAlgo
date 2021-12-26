#include <ConfigurationSet.h>

bool compare_configs(const Configuration& a, const Configuration& b) {
    if (a.before_situation != b.before_situation) {
        return a.before_situation < b.before_situation;
    }
    if (a.situation.point != b.situation.point) {
        return a.situation.point < b.situation.point;
    }
    if (a.situation.rule.premise != b.situation.rule.premise) {
        return a.situation.rule.premise < b.situation.rule.premise;
    }
    return a.situation.rule.result < b.situation.rule.result;
}

ConfigurationSet::ConfigurationSet(): configs(compare_configs), before_point(0) {}
