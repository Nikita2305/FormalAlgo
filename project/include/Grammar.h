#pragma once
#include <string>
#include <vector>
#include <Rule.h>

class Grammar {
public:
    Grammar() = default;
    Grammar(const std::string& terminals, const std::string& letters, char S);
    void appendRules(const std::vector<Rule>& addition);
    void changeStart();
    const std::vector<Rule>& getRules() const;
    std::vector<Rule> getCertainRules(char premise) const;
    const std::string& getNonTerminals() const;
    const std::string& getAlphabet() const;
    char getS() const;
private:
    std::string non_terminals;
    std::string letters;
    char S;
    std::vector<Rule> rules;
};
