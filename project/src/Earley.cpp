#include <Earley.h>
#include <cassert>
#include <iostream>

void Earley::fit(const Grammar& sourceGrammar) {
    grammar = sourceGrammar;
    prev_start = grammar.getStart();
    grammar.changeStart();
    new_start = grammar.getStart();
}

bool Earley::process(const std::string& word) const {
    std::vector<ConfigurationSet> sets; 
    sets.emplace_back();
    sets[0].before_point = 0;
    sets[0].configs.insert({{{new_start, std::string(1, prev_start)}, 0}, 0});
    for (int i = 1; i <= word.length(); i++) {
        sets.emplace_back();
        sets[i].before_point = i;
    }
    update(sets, 0);
    for (int j = 1; j <= word.length(); j++) {
        for (const Configuration& conf : sets[j - 1].configs) {
            Scan(sets[j], conf, word[j - 1]);
        }
        update(sets, j);
        
    }
    return sets.back().configs.find({{{new_start, std::string(1, prev_start)}, 1}, 0}) != sets.back().configs.end();
}

void Earley::update(std::vector<ConfigurationSet>& sets, int index) const {
    bool changed = true;
    while (changed) {
        changed = false;
        for (const Configuration& conf : sets[index].configs) {
            changed |= Complete(sets[index], conf, sets);
        }
        for (const Configuration& conf : sets[index].configs) {
            changed |= Predict(sets[index], conf);
        }
    }
}

bool Earley::Scan(ConfigurationSet& destination, const Configuration& config, char letter) const {
    if (config.situation.point == config.situation.rule.result.length()) {
        return false;
    }
    if (letter != config.situation.rule.result[config.situation.point]) {
        return false;
    }
    size_t destinationSize = destination.configs.size();
    destination.configs.insert({{config.situation.rule, config.situation.point + 1}, config.before_situation});
    return destinationSize != destination.configs.size();
}

bool Earley::Complete(ConfigurationSet& destination, const Configuration& config, const std::vector<ConfigurationSet>& sets) const {
    if (config.situation.point != config.situation.rule.result.length()) {
        return false;
    }
    size_t destinationSize = destination.configs.size();
    for (const Configuration& other : sets[config.before_situation].configs) {
        if (other.situation.point == other.situation.rule.result.length()) {
            continue;
        }
        if (config.situation.rule.premise != other.situation.rule.result[other.situation.point]) {
            continue;
        }
        destination.configs.insert({{other.situation.rule, other.situation.point + 1}, other.before_situation});
    }
    return destinationSize != destination.configs.size();
}

bool Earley::Predict(ConfigurationSet& destination, const Configuration& config) const {
    if (config.situation.point == config.situation.rule.result.length()) {
        return false;
    }
    std::vector<Rule> rules = grammar.getCertainRules(config.situation.rule.result[config.situation.point]);
    size_t destinationSize = destination.configs.size();
    for (const Rule& rule : rules) {
        destination.configs.insert({{rule, 0}, destination.before_point});
    }  
    return destinationSize != destination.configs.size();
}
