#include <Grammar.h>
#include <string>

Grammar::Grammar(const std::string& non_terminals, const std::string& letters, char StartNonTerminal): non_terminals(non_terminals), letters(letters), Start(StartNonTerminal) {}

void Grammar::appendRules(const std::vector<Rule>& addition) {
    for (const Rule& rule : addition) {
        rules.push_back(rule);
    }
}

void Grammar::changeStart() {
    char new_start = Start;
    for (char x : non_terminals) {
        new_start = std::min(new_start, x);
    }
    for (char x : letters) {
        new_start = std::min(new_start, x);
    }
    new_start--;
    non_terminals.push_back(new_start); 
    rules.push_back({new_start, std::string(1, Start)});
    Start = new_start;
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

const std::string& Grammar::getNonTerminals() const {
    return non_terminals;
}

const std::string& Grammar::getAlphabet() const {
    return letters;
}

char Grammar::getStart() const {
    return Start;
}
