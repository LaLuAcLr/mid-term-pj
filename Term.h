#ifndef TERM_H
#define TERM_H
#include <string>
#include <set>
#include <vector>
using namespace std;

namespace Ct {
    class NoInput {};

    typedef vector<int> MBinCode;

    class Term {
    public:
        Term(int =1, int =0, bool =false);
        bool& get_dont_care();
        bool get_dont_care() const;
        set<int>& get_combination();
        set<int> get_combination() const;
        int& get_one_num();
        int get_one_num() const;


        friend bool operator<(const Term&, const Term&);
        friend int combine_term_pos(const Term&, const Term&);
        friend Term combine_term(const Term&, const Term&, int);
    private:
        bool dont_care;
        MBinCode bin_code;
        set<int> combination;
        int one_num;
    };

    string pre_process(const string&);
    int get_var_num(const string&);
    set<Term> input_truthtable(int, const string&);
    MBinCode min_term2bin_code(int, int);

}

string truthtable_to_expr(const string&);





#endif
