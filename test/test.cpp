#define BOOST_TEST_MODULE AlgoTests
#include <boost/test/included/unit_test.hpp> // if this doesn't work
// #include <boost/test/unit_test.hpp> // then try this

#include <Grammar.h>
#include <Earley.h>

BOOST_AUTO_TEST_CASE(GrammarTestGetters) {
    Grammar g("SABC", "abc", 'S');
    BOOST_CHECK_EQUAL("SABC", g.getTerminals());
    BOOST_CHECK_EQUAL("abc", g.getAlphabet());
    BOOST_CHECK_EQUAL('S', g.getS());
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
    BOOST_CHECK_EQUAL(g.getS(), 'R');
}

Earley getEarley() {
    Grammar g("SX", "ab", 'S');
    std::vector<Rule> rules;
    rules.push_back({'S', ""});
    rules.push_back({'S', "aX"});
    rules.push_back({'X', "aX"});
    rules.push_back({'X', "b"});
    // eps, ab, aab, aaab
    g.appendRules(rules);
    Earley algo;
    algo.fit(g);
    return algo;
}

bool operator==(const Configuration& a, const Configuration& b) {
    return !(a < b) && !(b < a);
}

bool operator!=(const Configuration& a, const Configuration& b) {
    return !(a == b);
}

bool operator==(const ConfigurationSet& a, const ConfigurationSet& b) {
    if (a.before_point != b.before_point)
        return false;
    auto ai = a.configs.begin();    
    auto bi = b.configs.begin();
    while (ai != a.configs.end()) {
        if (*ai != *bi) {
            return false;
        }
        ++ai;
        ++bi;
    }
    return true;
}

BOOST_AUTO_TEST_CASE(EarleyTestUpdate) {
    Earley algo = getEarley();
    std::vector<ConfigurationSet> vect;
    vect.push_back({std::set<Configuration>(), 0}); 
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
    ConfigurationSet cs{std::set<Configuration>(), 0};
    Configuration conf = {{{'R', "aS"}, 0}, 0}; 
    algo.Scan(cs, conf, 'a');
    
    ConfigurationSet cs_true{std::set<Configuration>(), 0};
    Configuration new_conf = {{{'R', "aS"}, 1}, 0}; 
    cs_true.configs.insert(new_conf);
    BOOST_CHECK(cs_true == cs);
}

BOOST_AUTO_TEST_CASE(EarleyTestScan2) { 
    Earley algo = getEarley();
    ConfigurationSet cs{std::set<Configuration>(), 0};
    Configuration conf = {{{'R', "aS"}, 0}, 0}; 
    algo.Scan(cs, conf, 'b');
    BOOST_CHECK_EQUAL(cs.configs.size(), 0);
}

BOOST_AUTO_TEST_CASE(EarleyTestProcess1) {
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
