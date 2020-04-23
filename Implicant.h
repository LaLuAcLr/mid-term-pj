#ifndef IMPLICANT_H
#define IMPLICANT_H
#include <set>
#include <vector>
#include "Term.h"
using namespace std;

namespace Ct {
    typedef vector<bool> TBinCode;

    class ImplicantTable {
    public:
        ImplicantTable                              (const vector<int>&, int);

        ImplicantTable&         operator*=          (const ImplicantTable&);
        void                    simplify_tab        ();
        TBinCode                simplest_bin_code   ();
    private:
        set<TBinCode>           m_bin_code;
    };

    bool                        operator<           (const TBinCode&, const TBinCode&);
    TBinCode                    operator+           (const TBinCode&, const TBinCode&);
    bool                        IsIncluded          (const TBinCode&, const TBinCode&);
    set<Term>                   bin_code2TermPool   (TBinCode, const vector<Term>&);
    set<Term>                   solve_petrick       (const set<int>&, const set<Term>&);
    string                      output_expr         (const set<Term>&);
}   //namespace Ct


#endif
