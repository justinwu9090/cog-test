#include <iostream>
#include <clang-c/Index.h>
#include <vector>
#include <string>
using namespace std;

#include <Big3.hpp>

static vector<std::string> globalFnTable;

ostream &operator<<(ostream &stream, const CXString &str);
string clangStr(const CXCursor c);
CXChildVisitResult GetClassConstructor(CXCursor c, CXCursor parent, CXClientData client_data);
CXChildVisitResult GetClassDestructor(CXCursor c, CXCursor parent, CXClientData client_data);

// ======================================================
// Classes
// ======================================================

Big3::Big3(const char *file)
{
  index = clang_createIndex(0, 0);
  unit = clang_parseTranslationUnit(
      index,
      file, nullptr, 0,
      nullptr, 0,
      CXTranslationUnit_None);
  if (unit == nullptr)
  {
    cerr << "Unable to parse translation unit. Quitting." << endl;
    exit(-1);
  }
}

Big3::~Big3()
{
  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}

void Big3::FindClassConstructor()
{
  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      &GetClassConstructor,
      static_cast<void *>(&fnTable));
}
void Big3::FindClassDestructor()
{
  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      &GetClassDestructor,
      static_cast<void *>(&fnTable));
}

string Big3::DumpFnTable()
{
  string ret;
  for (auto x : fnTable)
  {
    ret.append(" " + x);
  }
  return ret;
}

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

CXChildVisitResult GetClassConstructor(CXCursor c, CXCursor parent, CXClientData client_data)
{
  if ((clang_getCursorKind(c) == CXCursor_Constructor))
  {
    std::vector<string> &fnTable = *static_cast<std::vector<string>*>(client_data);
    fnTable.push_back(clangStr(c));
  }

  return CXChildVisit_Recurse;
}

CXChildVisitResult GetClassDestructor(CXCursor c, CXCursor parent, CXClientData client_data)
{
  if ((clang_getCursorKind(c) == CXCursor_Destructor))
  {
    std::vector<string> &fnTable = *static_cast<std::vector<string>*>(client_data);
    fnTable.push_back(clangStr(c));
  }

  return CXChildVisit_Recurse;
}