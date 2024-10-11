#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE xxx
#include <boost/test/unit_test.hpp>

#include <Big3.hpp>

// helpers
string vectorToString(const vector<string> v);

// tests
BOOST_AUTO_TEST_CASE(constructors_test)
{
    vector<string> want({"MyClass"});
    Big3 b3("testcases/Class.hpp");
    b3.FindClassDeclarations();
    
    BOOST_REQUIRE_MESSAGE(want == b3.fnTable, "failed with table dump, want " << vectorToString(want) << " got " << b3.DumpFnTable());
}

// BOOST_AUTO_TEST_CASE(destructors_test)
// {
//     // find constructor of a class that does exist

//     // check class has no derived types
//     // check class has derived types and therefore the destructor should be virtual
// }

// BOOST_AUTO_TEST_CASE(copyconstructor_test)
// {
//     //  find copy constructor exists
// }

// BOOST_AUTO_TEST_CASE(copyassignment_test)
// {
//     //  find copy constructor exists
// }

string vectorToString(const vector<string> v)
{
    std::string s;
    for (const auto &piece : v)
        s += piece + " ";
    return s;
}