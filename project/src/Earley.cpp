#include <Earley.h>
#include <cassert>
#include <iostream>

void Earley::fit(const Grammar& g) {
    grammar = g;
    S = grammar.getS();
    grammar.changeStart();
    S1 = grammar.getS();
}

bool Earley::process(const std::string& word) {
    std::vector<ConfigurationSet> sets; 
    sets.emplace_back();
    sets[0].before_point = 0;
    sets[0].configs.insert({{{S1, std::string(1, S)}, 0}, 0});
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
    return sets.back().configs.find({{{S1, std::string(1, S)}, 1}, 0}) != sets.back().configs.end();
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

bool Earley::Scan(ConfigurationSet& D, const Configuration& conf, char letter) const {
    if (conf.situation.point == conf.situation.rule.result.length()) {
        return false;
    }
    if (letter != conf.situation.rule.result[conf.situation.point]) {
        return false;
    }
    size_t Dsize = D.configs.size();
    D.configs.insert({{conf.situation.rule, conf.situation.point + 1}, conf.before_situation});
    return Dsize != D.configs.size();
}

bool Earley::Complete(ConfigurationSet& D, const Configuration& conf, const std::vector<ConfigurationSet>& sets) const {
    if (conf.situation.point != conf.situation.rule.result.length()) {
        return false;
    }
    size_t Dsize = D.configs.size();
    for (const Configuration& other : sets[conf.before_situation].configs) {
        if (other.situation.point == other.situation.rule.result.length()) {
            continue;
        }
        if (conf.situation.rule.premise != other.situation.rule.result[other.situation.point]) {
            continue;
        }
        D.configs.insert({{other.situation.rule, other.situation.point + 1}, other.before_situation});
    }
    return Dsize != D.configs.size();
}

bool Earley::Predict(ConfigurationSet& D, const Configuration& conf) const {
    if (conf.situation.point == conf.situation.rule.result.length()) {
        return false;
    }
    std::vector<Rule> rules = grammar.getCertainRules(conf.situation.rule.result[conf.situation.point]);
    size_t Dsize = D.configs.size();
    for (const Rule& rule : rules) {
        D.configs.insert({{rule, 0}, D.before_point});
    }  
    return Dsize != D.configs.size();
}
