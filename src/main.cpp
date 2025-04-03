#include "TestAPI.hpp"

// TODO: Span virtual functions, namely for setting and resetting local / default buffers, also
// create a generalized test framework for string output (void) methods
// create a generalized test framework for typed output (T) methods

// A Demo of what I've built so far, this is going to be especially helpful to me if i attemp university assignments or leetcode problems in general
void PrintMethod(std::string s)
{
    int N = s.length();

    std::stringstream ss;
    auto space = [&](int N) {
        for (int i = 0; i < N; i++)
            ss << ' ';
        return ss.str();
    };

    std::cout << s << space(N) << s;
}

int add(int a, int b) { return a + b; }

int main()
{
    {
        PrintingTest test(PrintMethod);
        test.add_test({ "Akiller" }, "Akiller       Akiller");
        test.add_test({ "Bibek" }, "Hiira       Samskar");
        test.add_test({ "Nitin" }, "Nitin     Nitin");

        test.execute_tests();
    }
    {
        AdditionTest<int> test(add);

        test.add_test({ 4, 5 }, 9);
        test.add_test({ 4, 3 }, 9);
        test.add_test({ 2, 7 }, 9);

        test.execute_tests();
    }

    return 0;
}