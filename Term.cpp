#include <iostream>
#include <string>
#include <regex>
#include <cmath>
#include "Term.h"
#include "Implicant.h"
using namespace std;
using uint = unsigned int;

namespace Ct {
    Term::Term(int n, int value, bool dc) {
        dont_care = dc;
        bin_code = min_term2bin_code(n, value);
        combination.insert(value);
        one_num = 0;
        for (uint i=0; i<bin_code.size(); i++) {
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

    MBinCode& Term::get_bin_code() {
        return bin_code;
    }

    MBinCode Term::get_bin_code() const {
        return bin_code;
    }

    string Term::output_string() {
        string ans;
        size_t already_output_var = 0;
        size_t reduce_var = 0;
        for (uint i=0; i<bin_code.size(); i++) {
            if (bin_code[i] == -1)
                reduce_var++;
        }
        int n = bin_code.size();
        for (uint i=0; i<bin_code.size(); i++) {
            if (bin_code[i] != -1) {
                already_output_var++;
                if (bin_code[i] == 1) {
                    ans.push_back('A' + i);
                }
                else if (bin_code[i] == 0) {
                    ans.push_back('(');
                    ans.push_back('~');
                    ans.push_back('A' + i);
                    ans.push_back(')');
                }
                if (already_output_var < bin_code.size() - reduce_var)
                    ans.push_back('&');
            }
        }
        return ans;
    }

    string pre_process(const string& str) {
        string cp;
        cp = regex_replace(str, regex("[ ,]"), "");
        if (cp.empty()) throw NoInput{};
        if (regex_search(cp, regex("[^10d]")))
            throw InvalidInput{};
        vector<int> len;
        for (int i = 0; i <= 8; i++)
            len.push_back(static_cast<int>(pow(2, i)));
        bool flg = 0;
        for (uint i = 0; i < len.size(); i++) {
            if (cp.size() == len[i]) flg = 1;
        }
        if (flg == 0) throw StrLenError{};
        return cp;
    }

    int get_var_num(const string& str) {
        return (int)log2(str.size());
    }

    set<int> min_term_index(const string& str) {
        set<int> ans;
        for (uint i=0; i<=str.size(); i++) {
            if (str[i] == '1'){
                ans.insert(i);
            }
        }
        return ans;
    }

    set<Term> input_truthtable(int n, const string& str) {
        set<Term> ans;
        for (uint i=0; i<str.size(); i++) {
            if (str[i] == '1') {
                Term t(n, i, false);
                ans.insert(t);
            }
            if (str[i] == 'd') {
                Term t(n, i, true);
                ans.insert(t);
            }
        }
        return ans;
    }

    MBinCode min_term2bin_code(int n, int value) {
        MBinCode ans;
        for (int i=0; i<n; i++) {
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
            for (uint i=0; i<a.bin_code.size(); i++) {
                if (a.bin_code[i] != b.bin_code[i]) {   // 0<1, 0<-1, 1<-1
                    if (a.bin_code[i] == 0)
                        return true;
                    else if (b.bin_code[i] == 0)
                        return false;
                    else
                        return a.bin_code[i] > 0;
                }
            }
        }
        return false;
    }

    int combine_term_pos(const Term& a, const Term& b) {
        int pos = -1;
        for (uint i=0; i<a.bin_code.size(); i++) {
            if (a.bin_code[i] + b.bin_code[i] == 1)
                pos = i;
        }
        if (pos == -1) return -1;
        for (uint j=0; j<a.bin_code.size(); j++) {
            if (j != pos && a.bin_code[j] != b.bin_code[j])
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

    set<Term> solve_QM(int nVar, const set<Term>& TermPool) {
        vector<vector<Term>> pool;
        for (int i=0; i<nVar+1; i++) {
            vector<Term> t;
            t.reserve(nVar * (nVar + 1));
            pool.push_back(t);
        }
        for (auto j: TermPool) {    // classify based on one_num
            pool[j.get_one_num()].push_back(j);
        }
        set<Term> ans;
        bool already_combine = false;
        for (int i=0; i!=pool.size(); i++) {
            for (int j=0; j!=pool[i].size(); j++) {
                bool can_combine = false;
                if (i != pool.size() - 1) {
                    for (int k=0; k!=pool[i+1].size(); k++) {
                        int pos = combine_term_pos(pool[i][j], pool[i+1][k]);
                        if (pos != -1) {
                        can_combine = true;
                        if (!already_combine) already_combine = true;
                        Term combined_term = combine_term(pool[i][j], pool[i+1][k], pos);
                        ans.insert(combined_term);
                        }
                    }
                }
                if (i > 0) {
                    for (int l=0; l!=pool[i-1].size(); l++) {
                        int pos = combine_term_pos(pool[i][j], pool[i-1][l]);
                        if (pos != -1)
                            can_combine = true;
                    }
                }
                if (!can_combine)
                    ans.insert(pool[i][j]);
            }
        }
        if (!already_combine || ans.size() == 1)
            return ans;
        else
            return solve_QM(nVar, ans);
    }

    set<Term>& remove_dont_care(set<Term>& TermPool) {
        set<Term>::iterator it;
        for (it=TermPool.begin(); it!=TermPool.end();) {
            Term x = *it;
            if (x.get_dont_care()) {
                set<Term>::iterator cp = it;
                it++;
                TermPool.erase(cp);
            }
            else it++;
        }
        return TermPool;
    }


}   // namespace Ct

string truthtable_to_expr(const string& truth_table) {
    string tt_processed = Ct::pre_process(truth_table);
    int nVar = Ct::get_var_num(tt_processed);
    set<int> IndexPool = Ct::min_term_index(tt_processed);
    set<Ct::Term> TermPool = Ct::input_truthtable(nVar, tt_processed);

    bool all_dont_care = true;
    for (Ct::Term i : TermPool) {
        if (!i.get_dont_care()) {
            all_dont_care = false;
            break;
        }
    }
    if (all_dont_care || TermPool.size() == 0)
        return "0";
    else if (TermPool.size() == pow(2, nVar))
        return "1";

    set<Ct::Term> Pool1 = Ct::solve_QM(nVar, TermPool);
    set<Ct::Term> Pool2 = Ct::remove_dont_care(Pool1);
    set<Ct::Term> Pool3 = Ct::solve_petrick(IndexPool, Pool2);
    string ans = Ct::output_expr(Pool3);
    return ans;
}
