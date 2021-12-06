#include <Grammar.h>
#include <string>

Grammar::Grammar(const std::string& terminals, const std::string& letters, char S): terminals(terminals), letters(letters), S(S) {}

void Grammar::appendRules(const std::vector<Rule>& addition) {
    for (const Rule& rule : addition) {
        rules.push_back(rule);
    }
}

void Grammar::changeStart() {
    char new_start = S;
    for (char x : terminals) {
        new_start = std::min(new_start, x);
    }
    for (char x : letters) {
        new_start = std::min(new_start, x);
    }
    new_start--;
    terminals.push_back(new_start); 
    rules.push_back({new_start, std::string(1, S)});
    S = new_start;
}

const std::vector<Rule>& Grammar::getRules() const {
    return rules;
}

std::vector<Rule> Grammar::getCertainRules(char premise) const {
    std::vector<Rule> ret;
    for (auto& rule : rules) {
        if (rule.premise == premise) {
            ret.push_back(rule);
        }
    }
    return ret;
}

const std::string& Grammar::getTerminals() const {
    return terminals;
}

const std::string& Grammar::getAlphabet() const {
    return letters;
}

char Grammar::getS() const {
    return S;
}
