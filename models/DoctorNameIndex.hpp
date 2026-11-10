#include <iostream>
#include <vector>


using namespace std;

namespace IndexEntry
{
    struct DoctorNameIndex{
        int PK;
        vector<string> NameArray;
    };   
}