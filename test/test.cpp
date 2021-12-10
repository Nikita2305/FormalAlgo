#define BOOST_TEST_MODULE AlgoTests
#include <boost/test/included/unit_test.hpp> // if this doesn't work
// #include <boost/test/unit_test.hpp> // then try this

#include <Grammar.h>
#include <Earley.h>
#include <ParserLR.h>

BOOST_AUTO_TEST_CASE(GrammarTestGetters) {
    Grammar g("SABC", "abc", 'S');
    BOOST_CHECK_EQUAL("SABC", g.getNonTerminals());
    BOOST_CHECK_EQUAL("abc", g.getAlphabet());
    BOOST_CHECK_EQUAL('S', g.getStart());
}

bool operator!=(const Rule& a, const Rule& b) {
    return a.premise != b.premise || a.result != b.result;
}

bool check_rules_equal(const std::vector<Rule>& a, const std::vector<Rule>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

BOOST_AUTO_TEST_CASE(GrammarTestCertainRules) {
    Grammar g("SABC", "abc", 'S');
    g.appendRules({{'S', "AB"}});
    g.appendRules({{'A', "Ab"}, {'B', "bA"}});
    std::vector<Rule> certainRules = {{'A', "Ab"}};
    BOOST_CHECK_EQUAL(check_rules_equal(certainRules, g.getCertainRules('A')), true);
}

BOOST_AUTO_TEST_CASE(GrammarTestNewStart) {
    Grammar g("SX", "ab", 'S');
    g.changeStart();
    BOOST_CHECK_EQUAL(g.getStart(), 'R');
}

Earley getEarley() {
    Grammar g("SX", "ab", 'S');
    std::vector<Rule> rules;
    rules.push_back({'S', ""});
    rules.push_back({'S', "aX"});
    rules.push_back({'X', "aX"});
    rules.push_back({'X', "b"});
    g.appendRules(rules);
    Earley algo;
    algo.fit(g);
    return algo;
}

bool operator==(const Configuration& a, const Configuration& b) {
    return !compare_configs(a, b) && !compare_configs(b, a);
}

bool operator!=(const Configuration& a, const Configuration& b) {
    return !(a == b);
}

template <typename T>
bool operator==(const std::set<T>& a, const std::set<T>& b) {
    if (a.size() != b.size())
        return false;
    auto ai = a.begin();
    auto bi = b.begin();
    while (ai != a.end()) {
        if (*ai != *bi) {
            return false;
        }
        ++ai;
        ++bi;
    }
    return true;
}

bool operator==(const ConfigurationSet& a, const ConfigurationSet& b) {
    if (a.before_point != b.before_point)
        return false;
    return a.configs == b.configs;
}

BOOST_AUTO_TEST_CASE(EarleyTestUpdate) {
    Earley algo = getEarley();
    std::vector<ConfigurationSet> vect;
    vect.emplace_back();
    vect.back().configs.insert({{{'R', "S"}, 0}, 0});
    ConfigurationSet cs_true = vect[0];

    algo.Predict(vect[0], *vect[0].configs.begin());
    cs_true.configs.insert({{{'S', ""}, 0}, 0});
    cs_true.configs.insert({{{'S', "aX"}, 0}, 0});
    BOOST_REQUIRE(cs_true == vect[0]);

    for (const Configuration& conf : vect[0].configs) {
        algo.Complete(vect[0], conf, vect);
    }
    cs_true.configs.insert({{{'R', "S"}, 1}, 0});
    BOOST_REQUIRE(cs_true == vect[0]);
}

BOOST_AUTO_TEST_CASE(EarleyTestScan1) {
    Earley algo = getEarley();
    ConfigurationSet cs;
    Configuration conf = {{{'R', "aS"}, 0}, 0};
    algo.Scan(cs, conf, 'a');

    ConfigurationSet cs_true;
    Configuration new_conf = {{{'R', "aS"}, 1}, 0};
    cs_true.configs.insert(new_conf);
    BOOST_CHECK(cs_true == cs);
}

BOOST_AUTO_TEST_CASE(EarleyTestScan2) {
    Earley algo = getEarley();
    ConfigurationSet cs;
    Configuration conf = {{{'R', "aS"}, 0}, 0};
    algo.Scan(cs, conf, 'b');
    BOOST_CHECK_EQUAL(cs.configs.size(), 0);
}

BOOST_AUTO_TEST_CASE(EarleyTestProcess) {
    Earley algo = getEarley();
    BOOST_CHECK_EQUAL(algo.process(""), true);
    BOOST_CHECK_EQUAL(algo.process("a"), false);
    BOOST_CHECK_EQUAL(algo.process("b"), false);
    BOOST_CHECK_EQUAL(algo.process("ab"), true);
    BOOST_CHECK_EQUAL(algo.process("aab"), true);
    BOOST_CHECK_EQUAL(algo.process("c"), false);
    BOOST_CHECK_EQUAL(algo.process("ba"), false);
    BOOST_CHECK_EQUAL(algo.process("aaaaaab"), true);
}

ParserLR getLR_simple() {
    Grammar g("SC", "cd", 'S');
    std::vector<Rule> rules;
    rules.push_back({'S', "CC"});
    rules.push_back({'C', "cC"});
    rules.push_back({'C', "d"});
    g.appendRules(rules);
    ParserLR algo;
    algo.fit(g);
    return algo;
}

std::string str(const std::set<char>& s) {
    std::string ret = "";
    for (auto c : s) {
        ret += c;
    }
    return ret;
}

BOOST_AUTO_TEST_CASE(LRTestGetFirst1) {
    ParserLR algo = getLR_simple();
    BOOST_CHECK_EQUAL(str(algo.getEpsilonCreators()), str(std::set<char>()));
    BOOST_CHECK_EQUAL(str(algo.getFirst("C")), str(std::set<char>({'c', 'd'})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("S")), str(std::set<char>({'c', 'd'})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("B")), str(std::set<char>({'c', 'd'})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("")), str(std::set<char>({ParserLR::END})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("c")), str(std::set<char>({'c'})));
}

BOOST_AUTO_TEST_CASE(LRTestProcess1) {
    ParserLR algo = getLR_simple();
    BOOST_CHECK_EQUAL(algo.process(""), false);
    BOOST_CHECK_EQUAL(algo.process("a"), false);
    BOOST_CHECK_EQUAL(algo.process("c"), false);
    BOOST_CHECK_EQUAL(algo.process("cdcd"), true);
    BOOST_CHECK_EQUAL(algo.process("ccdd"), true);
    BOOST_CHECK_EQUAL(algo.process("cc"), false);
    BOOST_CHECK_EQUAL(algo.process("ddd"), false);
    BOOST_CHECK_EQUAL(algo.process("ccccdcccd"), true);
}

ParserLR getLR_epsilons() {
    Grammar g("SCD", "cd", 'S');
    std::vector<Rule> rules;
    rules.push_back({'S', "CD"});
    rules.push_back({'C', ""});
    rules.push_back({'C', "c"});
    rules.push_back({'D', ""});
    rules.push_back({'D', "d"});
    g.appendRules(rules);
    ParserLR algo;
    algo.fit(g);
    return algo;
}

BOOST_AUTO_TEST_CASE(LRTestGetFirst2) {
    ParserLR algo = getLR_epsilons();
    BOOST_CHECK_EQUAL(str(algo.getEpsilonCreators()), str(std::set<char>({'B', 'S', 'C', 'D'})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("S")), str(std::set<char>({'c', 'd', ParserLR::END})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("C")), str(std::set<char>({'c', ParserLR::END})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("D")), str(std::set<char>({'d', ParserLR::END})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("B")), str(std::set<char>({'c', 'd', ParserLR::END})));
}

BOOST_AUTO_TEST_CASE(LRTestProcess2) {
    ParserLR algo = getLR_epsilons();
    BOOST_CHECK_EQUAL(algo.process(""), true);
    BOOST_CHECK_EQUAL(algo.process("a"), false);
    BOOST_CHECK_EQUAL(algo.process("c"), true);
    BOOST_CHECK_EQUAL(algo.process("d"), true);
    BOOST_CHECK_EQUAL(algo.process("cd"), true);
    BOOST_CHECK_EQUAL(algo.process("cc"), false);
}

ParserLR getLR_cycled() {
    Grammar g("S", "ab", 'S');
    std::vector<Rule> rules;
    rules.push_back({'S', "SaSb"});
    rules.push_back({'S', ""});
    g.appendRules(rules);
    ParserLR algo;
    algo.fit(g);
    return algo;
}

BOOST_AUTO_TEST_CASE(LRTestGetFirst3) {
    ParserLR algo = getLR_cycled();
    BOOST_CHECK_EQUAL(str(algo.getEpsilonCreators()), str(std::set<char>({'R', 'S'})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("S")), str(std::set<char>({'a', ParserLR::END})));
    BOOST_CHECK_EQUAL(str(algo.getFirst("R")), str(std::set<char>({'a', ParserLR::END})));
}

BOOST_AUTO_TEST_CASE(LRTestProcess3) {
    ParserLR algo = getLR_cycled();
    BOOST_CHECK_EQUAL(algo.process(""), true);
    BOOST_CHECK_EQUAL(algo.process("a"), false);
    BOOST_CHECK_EQUAL(algo.process("c"), false);
    BOOST_CHECK_EQUAL(algo.process("ab"), true);
    BOOST_CHECK_EQUAL(algo.process("ba"), false);
    BOOST_CHECK_EQUAL(algo.process("abab"), true);
    BOOST_CHECK_EQUAL(algo.process("aababb"), true);
    BOOST_CHECK_EQUAL(algo.process("aabb"), true);
}