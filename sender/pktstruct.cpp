#include <iostream>
#include <cstring>
#include <string>


struct pktstruct
{
    int header;
    size_t length;
    char* message; // to be changed later to string
};