#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "Truthtable.h"
#include "Term.h"
using namespace std;

int main() {
    string s;
    int n;
    cout << "Enter expr:\n";
    getline(cin, s);
    cout << "Enter num: ";
    cin >> n;
    string t = expr_to_truthtable(n, s);
    cout << t;
    cout << truthtable_to_expr(t);
    return 0;
}
