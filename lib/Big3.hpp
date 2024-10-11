#include <clang-c/Index.h>
#include <vector>
#include <string>
using namespace std;

class Big3
{
private:
    CXIndex index;
    CXTranslationUnit unit;
public:
    vector<std::string> fnTable;
    Big3(const char *file);
    ~Big3();
    void FindClassDeclarations();
    string DumpFnTable();
};