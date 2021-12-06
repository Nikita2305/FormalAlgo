#pragma once
#include <Algo.h>
#include <Configuration.h>
#include <ConfigurationSet.h>

class Earley: public Algo {
public:
    void fit(const Grammar& g) override;
    bool process(const std::string& word) override;
    bool Scan(ConfigurationSet& D, const Configuration& conf, char letter) const;
    bool Complete(ConfigurationSet& D, const Configuration& conf, const std::vector<ConfigurationSet>& sets) const;
    bool Predict(ConfigurationSet& D, const Configuration& conf) const;
private:
    Grammar grammar;
    char S1;
    char S;
    void update(std::vector<ConfigurationSet>& sets, int index) const;
};
