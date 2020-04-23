#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H

#include <iostream>
#include <vector>
#include "Implicant.h"
using namespace std;

namespace Tt{

    class       InvalidInput    {};
    class       NoInput         {};
    class       ExtraVar        {};
    class       BracketError    {};

    enum class  VarNum          {A=0, B, C, D, E, F, G, H};

    typedef     vector<bool>    TBinCode;

    class Truthtable {
    public:
        Truthtable                      (VarNum =VarNum::A);

        TBinCode    get_table           ()  const;
        TBinCode&   set_table           (const TBinCode&);
        string      output_to_string    (int);

    private:
        TBinCode    bin_code;
    };

    Truthtable      operator~           (const Truthtable&);
    Truthtable      operator&           (const Truthtable&, const Truthtable&);
    Truthtable      operator^           (const Truthtable&, const Truthtable&);
    Truthtable      operator|           (const Truthtable&, const Truthtable&);

    string          pre_process         (int,               const string&);
    int             find_char           (const string&,     char);
    Truthtable      calc                (const string&);
}

string              expr_to_truthtable  (int,               const string&);


#endif
