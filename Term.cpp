#include <iostream>
#include <string>
#include <regex>
#include <cmath>
#include "Term.h"
using namespace std;

namespace Ct {
    Term::Term(int n, int value, bool dont_care) {
        dont_care = dont_care;
        bin_code = min_term2bin_code(n, value);
        combination.insert(value);
        for (int i=0; i<=bin_code.size(); i++) {
            if (bin_code[i] == 1)
                one_num++;
        }
    }

    bool& Term::get_dont_care() {
        return dont_care;
    }

    bool Term::get_dont_care() const {
        return dont_care;
    }

    int& Term::get_one_num() {
        return one_num;
    }

    int Term::get_one_num() const {
        return one_num;
    }

    set<int>& Term::get_combination() {
        return combination;
    }

    set<int> Term::get_combination() const {
        return combination;
    }

    string pre_process(const string& str) {
        string cp;
        cp = regex_replace(str, regex(","), "");
        if (cp.empty()) throw NoInput{};
        return cp;
    }

    int get_var_num(const string& str) {
        return log2(str.size());
    }

    set<Term> input_truthtable(int n, const string& str) {
        set<Term> ans;
        for (int i=0; i<=pow(2, n); i++) {
            if (str[i] == 1) {
                ans.insert(Term(n, i, false));
            }
            if (str[i] == 'd') {
                ans.insert(Term(n, i, true));
            }
        }
        return ans;
    }

    MBinCode min_term2bin_code(int n, int value) {
        MBinCode ans;
        for (int i=0; i<pow(2, n); i++) {
            if (value % 2) ans.push_back(0);
            else ans.push_back(1);
            value = value >> 1;
        }
        return ans;
    }

    bool operator<(const Term& a, const Term& b) {
        if (a.get_one_num() > b.get_one_num())
            return false;
        else if (a.get_one_num() < b.get_one_num())
            return true;
        else {
            for (int i=0; i<=a.bin_code.size(); i++) {
                if (a.bin_code[i] != b.bin_code[i]) {   // 0<1, 0<-1, -1<1
                    if (a.bin_code[i] == 0)
                        return true;
                    else if (b.bin_code[i] == 0)
                        return false;
                    else
                        return a.bin_code[i] < 0;
                }
            }
        }
        return false;
    }

    int combine_term_pos(const Term& a, const Term& b) {
        int pos = -1;
        for (int i=0; i<=a.bin_code.size(); i++) {
            if (a.bin_code[i] + b.bin_code[i] == 1)
                pos = i;
            if (pos != -1 && a.bin_code[i] != b.bin_code[i])
                return -1;
        }
        return pos;
    }

    Term combine_term(const Term& a, const Term& b, int pos) {
        Term ans = a;
        ans.dont_care &= b.dont_care;   // both dont care for a and b
        if (a.get_one_num() > b.get_one_num())
            ans.get_one_num() = b.get_one_num();
        else
            ans.get_one_num() = a.get_one_num();
        ans.bin_code[pos] = -1;     // '-'
        for (auto i : b.combination)
            ans.combination.insert(i);
        return ans;
    }
}

string truthtable_to_expr(const string& truth_table) {
    return " ";
}
