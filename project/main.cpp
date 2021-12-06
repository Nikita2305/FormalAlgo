#include <iostream>
#include <Grammar.h>
#include <Earley.h>
#include <Rule.h>

int main() {
    Grammar g("SA", "ab", 'S');
    std::vector<Rule> rules;
    rules.push_back({'S', ""});
    rules.push_back({'S', "aA"});
    rules.push_back({'A', "aA"});
    rules.push_back({'A', "b"});
    // eps, ab, aab, aaab
    g.appendRules(rules);
    Earley algo;
    algo.fit(g);
    std::cout << algo.process("") << std::endl;
    std::cout << algo.process("a") << std::endl;
    std::cout << algo.process("b") << std::endl;
    std::cout << algo.process("aa") << std::endl;
    std::cout << algo.process("ab") << std::endl;
}
