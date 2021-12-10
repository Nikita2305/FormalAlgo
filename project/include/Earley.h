#pragma once
#include <Algo.h>
#include <Configuration.h>
#include <ConfigurationSet.h>

class Earley: public Algo {
public:
    void fit(const Grammar& sourceGrammar) override;
    bool process(const std::string& word) const override;
    bool Scan(ConfigurationSet& destination, const Configuration& config, char letter) const;
    bool Complete(ConfigurationSet& destination, const Configuration& config, const std::vector<ConfigurationSet>& sets) const;
    bool Predict(ConfigurationSet& destination, const Configuration& config) const;
private:
    Grammar grammar;
    char new_start;
    char prev_start;
    void update(std::vector<ConfigurationSet>& sets, int index) const;
};
