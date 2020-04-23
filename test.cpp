#include "TruthTable.h"
#include "Term.h"
#include "Implicant.h"
#include "simple_test.h"
using namespace std;

int test_main() {
    CHECK_THROW(expr_to_truthtable(1, ""),              Tt::NoInput);
    CHECK_THROW(expr_to_truthtable(1, "   "),           Tt::NoInput);

    CHECK_THROW(expr_to_truthtable(2, "a&b"),           Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(4, "A+B*C+D"),       Tt::InvalidInput);

    CHECK_THROW(expr_to_truthtable(3, "AB&C"),          Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(3, "A&^C"),          Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "A~&B"),          Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "&A|B"),          Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "A&B|"),          Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "(A&B|)"),        Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(1, "A|()"),          Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "(~A)(~B)"),      Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "(~A)~(~B)"),     Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "A(A&B)"),        Tt::InvalidInput);
    CHECK_THROW(expr_to_truthtable(2, "(A&B)B"),        Tt::InvalidInput);

    CHECK_THROW(expr_to_truthtable(1, "D"),             Tt::ExtraVar);
    CHECK_THROW(expr_to_truthtable(3, "A|B&D"),         Tt::ExtraVar);

    CHECK_THROW(expr_to_truthtable(3, "A|((B&(~C))"),   Tt::BracketError);

    CHECK_EQUAL(expr_to_truthtable(1, "A"),             "10");
    CHECK_EQUAL(expr_to_truthtable(2, "A"),             "1010");
    CHECK_EQUAL(expr_to_truthtable(3, "A"),             "10101010");
    CHECK_EQUAL(expr_to_truthtable(2, "A|B"),           "1110");
    CHECK_EQUAL(expr_to_truthtable(2, "A^B"),           "0110");
    CHECK_EQUAL(expr_to_truthtable(2, "A&B"),           "1000")
    CHECK_EQUAL(expr_to_truthtable(2, "~A"),            "0101");
    CHECK_EQUAL(expr_to_truthtable(3, "~A|B&C"),        "11010101");
    CHECK_EQUAL(expr_to_truthtable(4, "C|A&B|(~B)&D"),  "11111011,11111000");

    CHECK_THROW(truthtable_to_expr(""),                 Ct::NoInput);
    CHECK_THROW(truthtable_to_expr(" , , "),            Ct::NoInput);

    CHECK_THROW(truthtable_to_expr("12010101"),         Ct::InvalidInput);
    CHECK_THROW(truthtable_to_expr("1D010101"),         Ct::InvalidInput);

    CHECK_THROW(truthtable_to_expr("10101"),            Ct::StrLenError);

    CHECK_EQUAL(truthtable_to_expr("1010"),             "A");
    CHECK_EQUAL(truthtable_to_expr("10101010"),         "A");
    CHECK_EQUAL(truthtable_to_expr("01"),               "~A");
    CHECK_EQUAL(truthtable_to_expr("1"),                "1");
    CHECK_EQUAL(truthtable_to_expr("0"),                "0");
    CHECK_EQUAL(truthtable_to_expr("1111"),             "1");
    CHECK_EQUAL(truthtable_to_expr("1ddd"),             "1");
    CHECK_EQUAL(truthtable_to_expr("00000000"),         "0");
    CHECK_EQUAL(truthtable_to_expr("000ddd00"),         "0");
    CHECK_EQUAL(truthtable_to_expr("dddddddd"),         "0");
    CHECK_EQUAL(truthtable_to_expr("1011dd01"),         "(~A)&(~B)|A&C")
    CHECK_EQUAL(truthtable_to_expr("01000000,01000001"), "(~A)&(~B)&(~C)&(~D)|(~A)&B&C")
    CHECK_EQUAL(truthtable_to_expr("11000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000"), "B&C&D&E&F&G");
    CHECK_EQUAL(truthtable_to_expr("00011111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111"), "(~A)&(~B)|(~C)|(~D)|(~E)|(~F)|(~G)|(~H)");

    string test = "00011111,11011111";
    CHECK_EQUAL(expr_to_truthtable(4, truthtable_to_expr(test)), test);
    test = "00011111,11011111,11101111,11110111";
    CHECK_EQUAL(expr_to_truthtable(5, truthtable_to_expr(test)), test);
    test = "00011111,11111111,11101111,11111111,10111111,11111111,11111011,11111111";
    CHECK_EQUAL(expr_to_truthtable(6, truthtable_to_expr(test)), test);
    test = "A|B";
    CHECK_EQUAL(truthtable_to_expr(expr_to_truthtable(3, test)), test);
    test = "A&D";
    CHECK_EQUAL(truthtable_to_expr(expr_to_truthtable(6, test)), test);
    return 0;
}
