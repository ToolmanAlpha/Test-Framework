#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

template <typename Parent, typename Signature, typename ReturnType>
struct Test
{
    std::string name;
    std::function<Signature> func;
    Parent* parent_framework;

    Test(const std::string& name, std::function<Signature> func, Parent* parent = nullptr)
        : name(name), func(func), parent_framework(nullptr) {
            if (parent) parent_framework = parent;
        }

    template <typename... Args>
    void execute(ReturnType expected_output, Args... args) {
        auto output = func(args...);
        if (output == expected_output) {
            // we could also keep track of a count of tests performed
            std::cout << "Test " << name << " Successful!" << std::endl;
        } else { 
            std::cout << "Test " << name << " Unsuccessful!" << std::endl;
            std::cout << "Output of last execution was: " << output << std::endl;
            std::cout << "Expected output was         : " << expected_output << std::endl;
        }
    }
};

template <typename Parent, typename Signature>
struct Test<Parent, Signature, void>
{
    std::string name;
    std::function<Signature> func;
    Parent* parent_framework;

    Test(const std::string& name, std::function<Signature> func, Parent* parent = nullptr)
        : name(name), func(func), parent_framework(nullptr) {
            if (parent) parent_framework = parent;
        }

    template <typename... Args>
    void execute(std::string expected_output, Args... args) {
        if (!parent_framework)  {
            std::cout << "No Parent handle for console i/o provided!" << std::endl;
            return;
        }
        parent_framework->enable_local_print_buffer();

        func(args...);

        std::string last_output = parent_framework->retrieve_from_local_print_buffer();
        parent_framework->clear_local_print_buffer();
        parent_framework->disable_local_print_buffer();

        if (last_output == expected_output) {
            // we could also keep track of a count of tests performed
            std::cout << "Test " << name << " Successful!" << std::endl;
        } else {
            std::cout << "Test " << name << " Unsuccessful!" << std::endl;
            std::cout << "Output of last execution was: " << last_output << std::endl;
            std::cout << "Expected output was         : " << expected_output << std::endl;
        }

    }
};

// mandates a particular kind of a signature
template <typename T>
class AdditionTest
{
public:
    using Signature = T(T, T);
    std::function<Signature> add_method;

    AdditionTest(std::function<Signature> add_method) : add_method(add_method) {
    }

    struct ArgumentList {
        int n0;
        int n1;
    };

    using OutputType = int;

    void add_test(ArgumentList arg_list, OutputType expected)
    {
        std::pair<ArgumentList, OutputType> p;
        p.first = arg_list;
        p.second = expected;
        tests.push_back(p);
    }

    void execute_tests()
    {
        int counter = 0;
        for (const auto& test : tests)
        {
            counter++;

            std::stringstream ss;
            ss << "Addition Test " << counter << "...";

            auto arg_list = test.first;
            auto expected_output = test.second;
            Test<AdditionTest, Signature, T>(ss.str(), add_method)
            .execute(expected_output, arg_list.n0, arg_list.n1);
        }
    }


private:
    std::vector<std::pair<ArgumentList, OutputType>> tests;
};

// mandates a particular kind of a signature
class PrintingTest
{
public:

    struct ArgumentList {
        std::string str;
    };

    using OutputType = std::string;

    using Signature = void(std::string);
    std::function<Signature> print_method;

    PrintingTest(std::function<Signature> print_method) : print_method(print_method) {
        // capture the buffer pointer of the std::cout
        std_print_buffer = std::cout.rdbuf();
    }
    void add_test(ArgumentList arg_list, OutputType expected)
    {
        std::pair<ArgumentList, OutputType> p;
        p.first = arg_list;
        p.second = expected;
        tests.push_back(p);
    }

    void execute_tests()
    {
        int counter = 0;
        for (const auto& test : tests)
        {
            counter++;

            std::stringstream ss;
            ss << "Printing Test " << counter << "...";

            auto arg_list = test.first;
            auto expected_output = test.second;
            Test<PrintingTest, Signature, void>(ss.str(), print_method, this)
            .execute(expected_output, arg_list.str);
        }
    }
private:
    void clear_local_print_buffer()
    {
        local_print_buffer.str("");
        local_print_buffer.clear();
    }
    
    std::string retrieve_from_local_print_buffer()
    {
        return local_print_buffer.str();
    }

    void enable_local_print_buffer()
    {
        std::cout.rdbuf(local_print_buffer.rdbuf());
    }

    void disable_local_print_buffer()
    {
        std::cout.rdbuf(std_print_buffer);
    }
private:
    std::vector<std::pair<ArgumentList, OutputType>> tests;
    
    std::ostringstream local_print_buffer;
    std::streambuf* std_print_buffer;

    friend class Test<PrintingTest, Signature, void>;
};
