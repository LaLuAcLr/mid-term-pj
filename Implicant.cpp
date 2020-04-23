#include "Implicant.h"
#include <set>
#include <vector>
#include <iostream>
using namespace std;
using uint = unsigned int;

namespace Ct {
    ImplicantTable::ImplicantTable(const vector<int>& comb, int n) {
        cout << "new implicant\n";
        for (auto i : comb) {
            TBinCode t(n, false);
            t[n - i - 1] = true;
            for (uint k=0; k<t.size(); k++) {
                if (t[k]) cout << '1';
                else cout << '-';
            }
            cout << endl;
            m_bin_code.insert(t);
        }
        cout << "over\n";
    }

    ImplicantTable& ImplicantTable::operator*=(const ImplicantTable& it) {
        set<TBinCode> ans;
        for (auto i : this->m_bin_code) {
            for (auto k : it.m_bin_code) {
                ans.insert(i+k);
            }
        }
        this->m_bin_code = ans;
        cout << "test on mult:\n";
        for (auto i: this->m_bin_code) {
            for (uint k=0; k<i.size(); k++) {
                if (i[k]) cout << '1';
                else cout << '0';
            }
            cout << endl;
        }
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

    set<Term> bin_code2TermPool(TBinCode bin_code, const vector<Term>& Term_Row) {
        set<Term> ans;
        int n = bin_code.size();
        for (uint i=0; i<bin_code.size(); i++) {
            cout << "bin_code: " << bin_code[i];
            if (bin_code[i]) {
                ans.insert(Term_Row[n - i - 1]);
            }
            cout << endl;
        }
        return ans;
    }

    set<Term> solve_petrick(const set<int>& min_term_index, const set<Term>& TermPool) {
        if (TermPool.size() == 1)
            return TermPool;
        vector<Term> TermVector_Row;   // index terms
        for (auto i : TermPool)
            TermVector_Row.push_back(i);
        cout << "TermVector_Row.size: " << TermVector_Row.size() << endl;
        for (uint i=0; i<TermVector_Row.size(); i++) {
            for (uint k=0; k<TermVector_Row[i].bin_code.size(); k++) {
                cout << TermVector_Row[i].bin_code[k] << ' ';
            }
            cout << endl;
        }
        cout << "comb:\n";
        for (uint i=0; i<TermVector_Row.size(); i++) {
            for (auto k: TermVector_Row[i].get_combination()) {
                cout << k << ' ';
            }
            cout << endl;
        }
        vector<set<int>> ImplicantVectors;
        for (auto i : min_term_index) {
            set<int> ImplicantSet_Column;  // re-allocate rows into columns
            for (uint j=0; j<TermVector_Row.size(); j++) {
                for (auto k : TermVector_Row[j].get_combination()) {
                    if (k == i) ImplicantSet_Column.insert(j);
                }
            }
            ImplicantVectors.push_back(ImplicantSet_Column);
        }
        vector<ImplicantTable> ImplicantTable_Vector;
        for (uint i=0; i<ImplicantVectors.size(); i++) {
            vector<int> Combination_Vector;
            for (auto j : ImplicantVectors[i]) {
                cout << "j= " << j << '\t';
                Combination_Vector.push_back(j);
            }
            cout << endl;
            ImplicantTable t(Combination_Vector, TermVector_Row.size());
            ImplicantTable_Vector.push_back(t);
        }
        cout << "implicant:\n";
        for (uint i=0; i<ImplicantTable_Vector.size(); i++) {
            cout << "i= " << i << endl;
            for (auto k: ImplicantTable_Vector[i].m_bin_code) {
                for (uint j=0; j<k.size(); j++) {
                    if (k[j]) cout << "1";
                    else cout << '-';
                }
                cout << endl;
            }
        }
        ImplicantTable& ansImplicant = ImplicantTable_Vector[0];
        cout << "ansimplicant_pre\n";
        for (auto i: ansImplicant.m_bin_code) {
                for (uint k=0; k<i.size(); k++) {
                    if (i[k]) cout << '1';
                    else cout << '-';
                }
                cout << endl;
            }
        for (uint i=1; i<ImplicantTable_Vector.size(); i++) {
            ansImplicant *= ImplicantTable_Vector[i];
            cout << "mark\n";
            for (auto i: ansImplicant.m_bin_code) {
                for (uint k=0; k<i.size(); k++) {
                    if (i[k]) cout << '1';
                    else cout << '-';
                }
                cout << endl;
            }
            ansImplicant.simplify_tab();
        }
        cout << "ansImplicant.bin_code.size: " << ansImplicant.m_bin_code.size() << endl;
        for (auto i : ansImplicant.m_bin_code) {
            for (uint k = 0; k < i.size(); k++) {
                if (i[k]) cout << '1';
                else cout << '-';
            }
            cout << endl;
        }
        cout << "ansBinCode: \n";
        TBinCode ansBinCode = ansImplicant.simplest_bin_code();
        for (uint i = 0; i < ansBinCode.size(); i++) {
            if (ansBinCode[i]) cout << "1";
            else cout << "-";
        }
        cout << "\nansBinCode over\n";
        set<Term> ansTermPool = bin_code2TermPool(ansBinCode, TermVector_Row);
        return ansTermPool;
    }

    string output_expr(const set<Term>& ansTermPool) {
        string ans;
        if (ansTermPool.size() > 0) {
            for (auto i : ansTermPool) {
                ans.append(i.output_string());
                ans.push_back('|');
            }
            ans.erase(ans.end() - 1);
        }
        if (ansTermPool.size() == 1 && ans.size() == 4) {
            ans.erase(ans.begin());
            ans.erase(ans.end() - 1);
        }
        return ans;
    }
}   // namespace Ct

