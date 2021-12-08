#pragma once
#include <Situation.h>

struct Configuration {
    Situation situation; 
    int before_situation;
    /*bool operator<(const Configuration& b) const {
        if (before_situation != b.before_situation) {
            return before_situation < b.before_situation;
        }
        if (situation.point != b.situation.point) {
            return situation.point < b.situation.point;
        }
        if (situation.rule.premise != b.situation.rule.premise) {
            return situation.rule.premise < b.situation.rule.premise;
        }
        return situation.rule.result < b.situation.rule.result;
    }*/
};
