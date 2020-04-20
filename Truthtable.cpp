#include <iostream>
#include <vector>
#include <cmath>
#include <regex>
#include "TruthTable.h"
using namespace std;

namespace Tt {
    Truthtable::Truthtable(VarNum vn) {
        int n = int(vn);
        int k = 0;
        bool flg = true;
        for (int i=0; i<256; i++, k++) {
            if (k == n + 1) {
                flg = !flg;
                k = 0;
            }
            bin_code.push_back(flg);
        }
    }

    TBinCode Truthtable::get_table() const {
        return bin_code;
    }

    TBinCode& Truthtable::set_table(const TBinCode& bc) {
        bin_code = bc;
        return bin_code;
    }

    string Truthtable::output_to_string(int n) {
        string ans;
        for (int i=0; i<pow(2, n); i++) {
            if (bin_code[i])
                ans.push_back('1');
            if (!bin_code[i])
                ans.push_back('0');
            if (i>0 && (i+1) % 8 == 0 && i != pow(2, n) - 1)
                ans.push_back(',');
        }
        return ans;
    }

    Truthtable operator~(const Truthtable& tt) {
        TBinCode ttable = tt.get_table();
        Truthtable ans;
        for (int i=0; i<256; i++)
            ttable[i] = !ttable[i];
        ans.set_table(ttable);
        return ans;
    }

    Truthtable operator&(const Truthtable& ta, const Truthtable& tb) {
        TBinCode atable = ta.get_table();
        TBinCode btable = tb.get_table();
        Truthtable ans;
        for (int i=0; i<256; i++)
            atable[i] = atable[i] & btable[i];
        ans.set_table(atable);
        return ans;
    }

    Truthtable operator^(const Truthtable& ta, const Truthtable& tb) {
        TBinCode atable = ta.get_table();
        TBinCode btable = tb.get_table();
        Truthtable ans;
        for (int i=0; i<256; i++)
            atable[i] = atable[i] ^ btable[i];
        ans.set_table(atable);
        return ans;
    }

    Truthtable operator|(const Truthtable& ta, const Truthtable& tb) {
        TBinCode atable = ta.get_table();
        TBinCode btable = tb.get_table();
        Truthtable ans;
        for (int i=0; i<256; i++)
            atable[i] = atable[i] | btable[i];
        ans.set_table(atable);
        return ans;
    }

    string pre_process(const string& str) {     // remove space
        string cp;
        cp = regex_replace(str, regex(" "), "");
        if (cp.empty()) throw NoInput{};
        return cp;
    }

    int find_char(const string& str, char ch) {
        for (int i=0; i!=str.size(); i++) {
            if (str[i] == ch) {
                int bracket_num=0;
                for(int j=i-1; j>=0; j--) {
                    switch (str[j])
                    {
                    case '(':
                        bracket_num--;
                        break;
                    case ')':
                        bracket_num++;
                        break;
                    }
                    if (bracket_num < 0) break;
                }
                if (bracket_num < 0) continue;
                return i;
            }
        }
        return -1;
    }

    Truthtable calc(const string& expr) {
        int pos = find_char(expr, '|');
        if (pos != -1)
            return calc(expr.substr(0, pos)) | calc(expr.substr(pos + 1));

        pos = find_char(expr, '^');
        if (pos != -1)
            return calc(expr.substr(0, pos)) ^ calc(expr.substr(pos + 1));

        pos = find_char(expr, '&');
        if (pos != -1)
            return calc(expr.substr(0, pos)) & calc(expr.substr(pos + 1));

        if (expr.front() == '~')
            return ~calc(expr.substr(1));

        if (expr.front() == '(' && expr.back() == ')')
            return calc(expr.substr(1, expr.size() - 2));

        if (expr.size() == 1)
            return Truthtable(VarNum(expr[0] - 'A'));
        else
            throw InvalidInput{};

        return Truthtable();
    }
}

string expr_to_truthtable(int n, const string& expr) {
    return Tt::calc(Tt::pre_process(expr)).output_to_string(n);
}
