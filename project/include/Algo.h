#pragma once
#include <Grammar.h>
#include <string>

class Algo {
public:
    virtual void fit(const Grammar& grammar) = 0;
    virtual bool process(const std::string& word) const = 0;
};
