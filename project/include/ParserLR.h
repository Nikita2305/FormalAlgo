#pragma once

#include <Algo.h>
#include <Grammar.h>
#include <NodeLR.h>
#include <string>
#include <set>

class ParserLR: public Algo {
public:
    void fit(const Grammar& sourceGrammar) override;
    bool process(const std::string& word) const override;
    std::set<char> getFirst(const std::string& expression);
    const std::set<char>& getEpsilonCreators() const;
    static char END;
private: 
    Grammar grammar;
    char new_start;
    char prev_start;
    std::vector<NodeLR> nodes;
    std::unordered_map<char, std::set<char>> FIRST;
    std::set<char> epsilon_creators;
    void countEpsilonCreators();
    void countFirst(char current, const char target, std::set<char>& processed);
    void buildAutomaton();
    std::vector<int> processNode(int vertex);
    void makeClosure(int id);
};
