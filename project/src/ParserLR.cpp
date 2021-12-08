#include <ParserLR.h>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <algorithm>

char ParserLR::END = '$';

void ParserLR::fit(const Grammar& g) {
    grammar = g;
    S = grammar.getS();
    grammar.changeStart();
    S1 = grammar.getS();
    countEpsilonCreators();
    for (char NonTerm : grammar.getNonTerminals()) {
        FIRST[NonTerm] = std::set<char>();
        std::set<char> processed;
        countFirst(NonTerm, NonTerm, processed); 
    } 
    buildAutomaton();
}

bool operator==(const SituationLR& a, const SituationLR& b) {
    return !compare_situations(a, b) && !compare_situations(b, a);
}

bool operator!=(const SituationLR& a, const SituationLR& b) {
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

void ParserLR::buildAutomaton() {
    nodes.emplace_back();
    nodes[0].situations.insert(SituationLR({Rule({S1, std::string(1, S)}), 0, END}));
    std::queue<int> process_queue;
    process_queue.push(0);
    makeClosure(0);
    while (!process_queue.empty()) {
        for (int id : processNode(process_queue.front())) {
            process_queue.push(id);
        }
        process_queue.pop();
    }
}

std::vector<int> ParserLR::processNode(int v) {
    std::vector<int> new_nodes;
    std::unordered_map<char,  Rule> reduces;
    std::unordered_map<char, std::vector<SituationLR>> moves;
    for (const SituationLR& situation : nodes[v].situations) {
        if (situation.point == situation.rule.result.length()) {
            if (reduces.find(situation.next) != reduces.end()) {
                throw std::runtime_error("Reduce-reduce conflict");
            }
            reduces[situation.next] = situation.rule;
        } else {
            char c = situation.rule.result[situation.point]; 
            if (moves.find(c) == moves.end()) {
                moves[c] = std::vector<SituationLR>();
            }
            moves[c].push_back(situation);
            moves[c].back().point++;
        }
    }
    std::unordered_map<char, int> shifts;
    for (auto& pair : moves) { 
        nodes.emplace_back(); 
        for (const SituationLR& situation : pair.second) {
            nodes.back().situations.insert(situation);
        }
        makeClosure(nodes.size() - 1);
        int id = nodes.size() - 1;
        for (int i = 0; i + 1 < nodes.size(); i++) {
            if (nodes.back().situations == nodes[i].situations) {
                id = i;
                break;
            }
        }
        if (id == nodes.size() - 1) {
            new_nodes.push_back(nodes.size() - 1);    
        } else {
            nodes.pop_back();
        }
        shifts[pair.first] = id;
    }
    for (char symbol : grammar.getAlphabet() + grammar.getNonTerminals() + END) {
        if (reduces.find(symbol) != reduces.end() && shifts.find(symbol) != shifts.end()) {
            throw std::runtime_error("Shift-reduce conflict");
        }
        if (reduces.find(symbol) != reduces.end()) {
            nodes[v].instructions.insert({symbol, Instruction(reduces[symbol])});
        }
        if (shifts.find(symbol) != shifts.end()) {
            nodes[v].instructions.insert({symbol, Instruction(shifts[symbol])});
        }   
    }
    return new_nodes;
}

void ParserLR::makeClosure(int id) {
    bool changed = true;
    while (changed) {
        size_t size = nodes[id].situations.size();
        for (const SituationLR& situation : nodes[id].situations) {
            int length = situation.rule.result.length();
            int point = situation.point;
            if (point == length) {
                continue;
            }
            char c = situation.rule.result[situation.point];
            std::set<char> nexts = getFirst(situation.rule.result.substr(point + 1, length - point - 1) + (situation.next == END ? "" : std::string(1, situation.next)));
            for (const Rule& rule : grammar.getCertainRules(c)) {
                for (char next : nexts) {
                    nodes[id].situations.insert(SituationLR({rule, 0, next}));
                }
            } 
        }
        changed = (size != nodes[id].situations.size());
    }
}

void ParserLR::countEpsilonCreators() {
    bool changed = true;
    while (changed) {
        size_t size = epsilon_creators.size();
        for (const Rule& rule  : grammar.getRules()) {
            bool epsilon_maker = true;
            for (char c : rule.result) {
                if (epsilon_creators.find(c) == epsilon_creators.end()) {
                    epsilon_maker = false;
                    break; 
                }
            }
            if (epsilon_maker) {
                epsilon_creators.insert(rule.premise);
            }
        }
        changed = (size != epsilon_creators.size());
    }
}

void ParserLR::countFirst(char current, const char target, std::set<char>& processed) {
    processed.insert(current);
    for (Rule rule : grammar.getCertainRules(current)) {
        if (rule.result.length() == 0) {
            continue;
        }
        for (char c : rule.result) {
            if (grammar.getAlphabet().find(c) != std::string::npos) {
                FIRST[target].insert(c);
                break;
            }
            if (processed.find(c) == processed.end()) {
                countFirst(c, target, processed);
            }
            if (epsilon_creators.find(c) == epsilon_creators.end()) {
                break;
            }
        }  
    } 
}

bool ParserLR::process(const std::string& word) const {
    int node = 0;
    std::vector<int> node_stack = {node};
    std::string process_word = word + END;
    std::reverse(process_word.begin(), process_word.end());
    while (process_word.length() > 0) {
        if (nodes[node].instructions.find(process_word.back()) == nodes[node].instructions.end()) {
            if (process_word.back() == S1) {
                break;
            }
            return false;
        }
        Instruction instruction = nodes[node].instructions.find(process_word.back())->second;
        if (instruction.isShift()) {
            node = instruction.getNode();
            node_stack.push_back(node);
            process_word.pop_back();
        } else {
            if (node_stack.size() + 1 < instruction.getRule().result.size()) {
                return false;
            }
            for (int j = 0; j < instruction.getRule().result.size(); j++) {
                node_stack.pop_back();
            }
            process_word.push_back(instruction.getRule().premise);
            node = node_stack.back();
        }
    }
    return node_stack.size() == 1 && node_stack.back() == 0 && process_word.size() == 2;
}

const std::set<char>& ParserLR::getEpsilonCreators() const {
    return epsilon_creators; 
}

std::set<char> ParserLR::getFirst(const std::string& w) {
    std::set<char> ret = {END};
    for (char c : w) {
        if (epsilon_creators.find(c) == epsilon_creators.end()) {
            ret.erase(END);
            break;
        }
    }
    if (w.length() == 0) {
        return ret;
    }
    if (grammar.getAlphabet().find(w[0]) != std::string::npos) {
        ret.insert(w[0]);
        return ret;
    }
    if (FIRST.find(w[0]) == FIRST.end()) {
        return ret;
    }
    std::set<char> rret = FIRST[w[0]];
    rret.merge(ret);
    return rret;
}
