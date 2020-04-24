#include "Implicant.h"
#include <set>
#include <vector>
#include <iostream>
using namespace std;
using uint = unsigned int;

namespace Ct {
    ImplicantTable::ImplicantTable(const vector<int>& comb, int n) {
        for (auto i : comb) {
            TBinCode t(n, false);
            t[n - i - 1] = true;
            m_bin_code.insert(t);
        }
    }

    ImplicantTable& ImplicantTable::operator*=(const ImplicantTable& it) {
        set<TBinCode> ans;
        for (auto i : this->m_bin_code) {
            for (auto k : it.m_bin_code) {
                ans.insert(i+k);
            }
        }
        this->m_bin_code = ans;
        return *this;
    }

    void ImplicantTable::simplify_tab() {
        vector<TBinCode> eraser;
        for (auto i : m_bin_code) {
            for (auto j : m_bin_code) {
                if ((i != j) && IsIncluded(i, j))
                    eraser.push_back(i);
            }
        }
        for (auto i : eraser)
            m_bin_code.erase(i);
    }

    TBinCode ImplicantTable::simplest_bin_code() {
        vector<TBinCode> t;
        for (auto i : m_bin_code)
            t.push_back(i);
        int counter = 100;
        int pos = 0;
        for (uint i=0; i<t.size(); i++) {
            int one_num = 0;
            for (uint k=0; k<t[i].size(); k++) {
                if (t[i][k]) one_num++;
            }
            if (one_num < counter) {
                counter = one_num;
                pos = i;
            }
        }
        return t[pos];
    }

    bool operator<(const TBinCode& a, const TBinCode& b) {
        for (uint i=0; i<a.size(); i++) {
            if (a[i] && !b[i]) return false;
            else if (!a[i] && b[i]) return true;
        }
        return false;
    }

    TBinCode operator+(const TBinCode& a, const TBinCode& b) {
        TBinCode ans = a;
        for (uint i=0; i<a.size(); i++)
            ans[i] = ans[i] | b[i];
        return ans;
    }

    bool IsIncluded(const TBinCode& a, const TBinCode& b) {
        for (uint i=0; i<a.size(); i++) {
            if (!a[i] && b[i]) return false;
        }
        return true;
    }

    set<Term> bin_code2ImplicantPool(TBinCode bin_code, const vector<Term>& Term_Row) {
        set<Term> ans;
        int n = bin_code.size();
        for (uint i=0; i<bin_code.size(); i++) {
            if (bin_code[i]) {
                ans.insert(Term_Row[n - i - 1]);
            }
        }
        return ans;
    }

    set<Term> solve_petrick(const set<int>& min_term_index, const set<Term>& ImplicantPool) {
        if (ImplicantPool.size() == 1)
            return ImplicantPool;
        vector<Term> ImplicantVector_Row;   // index terms
        for (auto i : ImplicantPool)
            ImplicantVector_Row.push_back(i);
        vector<set<int>> ImplicantVectors;
        for (auto i : min_term_index) {
            set<int> ImplicantSet_Column;  // re-allocate rows into columns
            for (uint j=0; j<ImplicantVector_Row.size(); j++) {
                for (auto k : ImplicantVector_Row[j].get_combination()) {
                    if (k == i) ImplicantSet_Column.insert(j);
                }
            }
            ImplicantVectors.push_back(ImplicantSet_Column);
        }
        vector<ImplicantTable> ImplicantTable_Vector;
        for (uint i=0; i<ImplicantVectors.size(); i++) {
            vector<int> Combination_Vector;
            for (auto j : ImplicantVectors[i]) {
                Combination_Vector.push_back(j);
            }
            ImplicantTable t(Combination_Vector, ImplicantVector_Row.size());
            ImplicantTable_Vector.push_back(t);
        }
        ImplicantTable& ansImplicant = ImplicantTable_Vector[0];
        for (uint i=1; i<ImplicantTable_Vector.size(); i++) {
            ansImplicant *= ImplicantTable_Vector[i];
            ansImplicant.simplify_tab();
        }
        TBinCode ansBinCode = ansImplicant.simplest_bin_code();
        set<Term> ansImplicantPool = bin_code2ImplicantPool(ansBinCode, ImplicantVector_Row);
        return ansImplicantPool;
    }

    string output_expr(const set<Term>& ansImplicantPool) {
        string ans;
        if (ansImplicantPool.size() > 0) {
            for (auto i : ansImplicantPool) {
                ans.append(i.output_string());
                ans.push_back('|');
            }
            ans.erase(ans.end() - 1);
        }
        if (ansImplicantPool.size() == 1 && ans.size() == 4) {
            ans.erase(ans.begin());
            ans.erase(ans.end() - 1);
        }
        return ans;
    }
}   // namespace Ct

