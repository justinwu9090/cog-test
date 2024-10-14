#include <clang-c/Index.h>
#include <vector>
#include <string>
using namespace std;


struct Function
{
    string name;
    string return_type;
    vector<pair<string, string>> parameters;
    // constructors
    Function() {}
    Function(string _return_type, string _name, vector<pair<string, string>> _parameters) : return_type(_return_type), name(_name), parameters(_parameters) {}
    string toString()
    {
        string ret;
        ret += name + " " + return_type + ", parameters.size() = " + std::to_string(parameters.size());
        return ret;
    }
};

class Big3
{
private:
    CXIndex index;
    CXTranslationUnit unit;

public:
    vector<std::string> fnTable;
    Big3(const char *file);
    ~Big3();
    void FindClassConstructor();
    void FindClassDestructor();
    vector<Function> GetFunctions();
    string DumpFnTable();
};