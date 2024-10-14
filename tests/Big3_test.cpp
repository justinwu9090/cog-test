#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE xxx
#include <boost/test/unit_test.hpp>

#include <Big3.hpp>

// helpers
template <typename T>
void AssertVectorsEqual(vector<T> want, vector<T> got);
template <typename T>
string vectorToString(const vector<T> v);
void assertFunctionsEqual(Function want, Function got);
// tests

BOOST_AUTO_TEST_CASE(method_test)
{
    vector<Function> want({
        Function("MyClass","void", "MyClass", {}),
        Function("MyClass","void", "~MyClass", {}),
        Function("MyClass","void", "method", {{"int", "x"}, {"float", "y"}}),
        Function("MyClass","int", "static_method", {}),
    });
    Big3 b3("testcases/Class.hpp");
    vector<Function> got = b3.GetFunctions();

    volatile int i = 0;
    BOOST_REQUIRE_MESSAGE(want.size() == got.size(), "Error want/got size not matching");

    for (int i = 0; i < want.size(); i++)
    {
        assertFunctionsEqual(want[i], got[i]);
    }
}

BOOST_AUTO_TEST_CASE(constructors_test)
{
    vector<string> want({"MyClass"});
    Big3 b3("testcases/Class.hpp");
    b3.FindClassConstructor();

    AssertVectorsEqual<string>(want, b3.fnTable);
}

BOOST_AUTO_TEST_CASE(destructors_test)
{
    // find destructor of a class that does exist
    []
    {
        vector<string> want({"~MyClass"});
        Big3 b3("testcases/Class.hpp");
        b3.FindClassDestructor();

        AssertVectorsEqual<string>(want, b3.fnTable);
    }();

    // check no destructor exists for this test case
    []
    {
        vector<string> want({});
        Big3 b3("testcases/ClassNoBig3.hpp");
        b3.FindClassDestructor();

        AssertVectorsEqual<string>(want, b3.fnTable);
    }();
    // check class has no derived types
    // check class has derived types and therefore the destructor should be virtual
}

// BOOST_AUTO_TEST_CASE(copyconstructor_test)
// {
//     //  find copy constructor exists
// }

// BOOST_AUTO_TEST_CASE(copyassignment_test)
// {
//     //  find copy constructor exists
// }

template <typename T>
void AssertVectorsEqual(vector<T> want, vector<T> got)
{
    BOOST_REQUIRE_MESSAGE(want == got, "failed with table dump, want " << vectorToString<T>(want) << " got " << vectorToString<T>(got));
}
template <typename T>
string vectorToString(const vector<T> v)
{
    std::string s;
    for (const auto &piece : v)
        s += piece + " ";
    return s;
}

void assertFunctionsEqual(Function want, Function got)
{

    BOOST_REQUIRE_MESSAGE(want.name == got.name, "Error, want " << want.name << " got " << got.name);
    BOOST_REQUIRE_MESSAGE(want.return_type == got.return_type, "Error, want " << want.return_type << " got " << got.return_type);

    for (pair<string, string> x : want.parameters)
    {

        auto it = find_if(got.parameters.begin(), got.parameters.end(), [x](const auto &pair)
                          { return (pair.first == x.first) && (pair.second == x.second); });

        BOOST_REQUIRE_MESSAGE(it != got.parameters.end(), "Error: parameter " << x.first << " " << x.second << " not found in " << got.toString());
    }
}