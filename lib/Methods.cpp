#include <iostream>
#include <clang-c/Index.h>
#include <vector>
#include <string>
using namespace std;

#include <Methods.hpp>

static vector<std::string> globalFnTable;

ostream &operator<<(ostream &stream, const CXString &str);
string CXCursorToString(const CXCursor c);
CXChildVisitResult GetClassConstructor(CXCursor c, CXCursor parent, CXClientData client_data);
CXChildVisitResult GetClassDestructor(CXCursor c, CXCursor parent, CXClientData client_data);
CXChildVisitResult IsMethod(CXCursor c, CXCursor parent, CXClientData client_data);

// ======================================================
// Classes
// ======================================================

Methods::Methods(const char *file)
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

Methods::~Methods()
{
  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}

void Methods::FindClassConstructor()
{
  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      &GetClassConstructor,
      static_cast<void *>(&fnTable));
}
void Methods::FindClassDestructor()
{
  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      &GetClassDestructor,
      static_cast<void *>(&fnTable));
}

vector<Function> Methods::GetFunctions()
{
  vector<Function> fnlist;
  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      &IsMethod,
      static_cast<void *>(&fnlist));
  return fnlist;
}

string Methods::DumpFnTable()
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

string CXStringtoString(CXString str)
{
  string s = clang_getCString(str);
  clang_disposeString(str);
  return s;
}

string CXCursorToString(const CXCursor c)
{
  CXString str = clang_getCursorSpelling(c);
  string s = CXStringtoString(str);
  return s;
}

CXChildVisitResult GetClassConstructor(CXCursor c, CXCursor parent, CXClientData client_data)
{
  if ((clang_getCursorKind(c) == CXCursor_Constructor))
  {
    std::vector<string> &fnTable = *static_cast<std::vector<string> *>(client_data);
    fnTable.push_back(CXCursorToString(c));
  }

  return CXChildVisit_Recurse;
}

CXChildVisitResult GetClassDestructor(CXCursor c, CXCursor parent, CXClientData client_data)
{
  if ((clang_getCursorKind(c) == CXCursor_Destructor))
  {
    std::vector<string> &fnTable = *static_cast<std::vector<string> *>(client_data);
    fnTable.push_back(CXCursorToString(c));
  }

  return CXChildVisit_Recurse;
}

CXChildVisitResult IsMethod(CXCursor c, CXCursor parent, CXClientData client_data)
{
  std::vector<Function> &ClassFunctions = *static_cast<std::vector<Function> *>(client_data);
  if ((clang_getCursorKind(c) == CXCursor_Constructor) || (clang_getCursorKind(c) == CXCursor_Destructor) || (clang_getCursorKind(c) == CXCursor_CXXMethod))
  {
    // result type
    string result_type = CXStringtoString(clang_getTypeSpelling(clang_getResultType(clang_getCursorType(c))));

    // can parse out non-variadic arguments
    int n = clang_Cursor_getNumArguments(c);
    vector<pair<string, string>> parameters;
    for (int i = 0; i < n; i++)
    {
      CXCursor argument = clang_Cursor_getArgument(c, i);
      CXType argument_type = clang_getCursorType(argument);
      string name = CXStringtoString(clang_getCursorSpelling(argument));
      string type = CXStringtoString(clang_getTypeSpelling(argument_type));
      // cout << clang_getTypeSpelling(argument_type) << " " << name << endl;
      parameters.push_back(make_pair(type, name));
    }
    string parent = CXCursorToString(clang_getCursorSemanticParent(c));

    Function fn(parent, result_type, CXCursorToString(c), parameters);
    ClassFunctions.push_back(fn);
  }
  return CXChildVisit_Recurse;
}