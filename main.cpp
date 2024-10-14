#include <iostream>
using namespace std;

#include <Big3.hpp>

int main()
{
    Big3 b3("testcases/Class.hpp");

    vector<Function> fns = b3.GetFunctions();

    for (auto fn : fns)
    {
        cout << fn.parent_name << "::" << fn.return_type << " " << fn.name << "(";

        for (int i = 0; i < fn.parameters.size(); i++)
        {
            auto p = fn.parameters[i];
            cout << p.first << " " << p.second;
            if (i < fn.parameters.size() - 1)
                cout << ", ";
        }

        cout << "){}" << endl
             << endl;
    }
}