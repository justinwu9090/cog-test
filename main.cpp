#include <iostream>
using namespace std;

#include <Methods.hpp>

int main(int argc, char* argv[])
{
    if (argc != 2) 
    {
        cout << "incorrect # arguments passed. need 1 argument for header file name";
        return 1;
    }
    
    cout << "// parsed from: " << argv[1] << endl;
    
    Methods b3(argv[1]);

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