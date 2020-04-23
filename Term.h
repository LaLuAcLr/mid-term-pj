#ifndef TERM_H
#define TERM_H
#include <string>
#include <set>
#include <vector>
using namespace std;

namespace Ct {
    class           NoInput {};
    class           InvalidInput {};
    class           StrLenError {};

    typedef vector<int> MBinCode;

    class Term {
    public:
        Term                            (int =1, int =0, bool =false);
        bool&       get_dont_care       ();
        bool        get_dont_care       () const;
        set<int>&   get_combination     ();
        set<int>    get_combination     () const;
        int&        get_one_num         ();
        int         get_one_num         () const;
        MBinCode&   get_bin_code        ();
        MBinCode    get_bin_code        () const;

        string      output_string();

        friend bool operator<           (const Term&, const Term&);
        friend int  combine_term_pos    (const Term&, const Term&);
        friend Term combine_term        (const Term&, const Term&, int);
    private:
        bool        dont_care;
        MBinCode    bin_code;
        set<int>    combination;
        int         one_num;
    };

    string          pre_process         (const string&);
    int             get_var_num         (const string&);
    set<Term>       input_truthtable    (int, const string&);
    MBinCode        min_term2bin_code   (int, int);
    set<Term>       solve_QM            (int, const set<Term>&);
    set<Term>&      remove_dont_care    (set<Term>&);
    set<int>        min_term_index      (const string&);

}

string              truthtable_to_expr  (const string&);

#endif
