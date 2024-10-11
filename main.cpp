#include <iostream>
#include <clang-c/Index.h>
#include <vector>
#include <string>
using namespace std;

// ======================================================
// Helpers
// ======================================================

ostream &operator<<(ostream &stream, const CXString &str)
{
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

string clangStr(const CXCursor c)
{
  CXString str = clang_getCursorSpelling(c);
  string s = clang_getCString(str);
  clang_disposeString(str);
  return s;
}

static vector<std::string> fnTable;

CXChildVisitResult IsClassDeclaration(CXCursor c, CXCursor parent, CXClientData client_data)
{
  if ((clang_getCursorKind(c) == CXCursor_Constructor) || (clang_getCursorKind(c) == CXCursor_Destructor))
  {
    // if (clang_CXXConstructor_isCopyConstructor(c))
    fnTable.push_back(clangStr(c));
  }

  // std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
  //           << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
  return CXChildVisit_Recurse;
}

// ======================================================
// Main
// ======================================================

int main()
{
  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index,
      "header.hpp", nullptr, 0,
      nullptr, 0,
      CXTranslationUnit_None);
  if (unit == nullptr)
  {
    cerr << "Unable to parse translation unit. Quitting." << endl;
    exit(-1);
  }

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      &IsClassDeclaration,
      nullptr);

  for (auto x : fnTable)
  {
    cout << x << endl;
  }
  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}
