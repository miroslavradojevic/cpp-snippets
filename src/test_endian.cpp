#include <bits/stdc++.h> 

using namespace std; 

int main(){
    
    unsigned int testInteger = 1;
    
    char *endianCheck = (char*)&testInteger;
    
    if (*endianCheck) {
        cout << "Your machine is little-endian" << endl;
    }
    else {
        cout << "Your machine is big-endian" << endl;
    }
    
    return 0;
}