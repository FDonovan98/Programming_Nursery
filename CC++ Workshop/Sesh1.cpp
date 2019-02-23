#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    int i;
    int errorcode = 0; 

    // if(argc > 1) {
    //     printf("param %d: %s \n", i, argv[i]);
    
    // } else  {
    //     errorcode = 1;
    // }

    // if (errorcode == 1) {
    //     cout << "No parameters passed \n";
    // }

    try {
        for (i=1; i<argc; i++) {
            stoi(argv[i]);
        }
    } catch (const std::exception& e) {
        cout << "Arguments weren't integers \n";
    }

}