// Bach Nguyen
// CECS 325-02
// Prog 4 - Sortrace using pthreads
// Due Date: 03/28/2024
// I certify that this program is my own original work. I did not copy any part of this program from
// any other source. I further certify that I typed each and every line of code in this program.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void generate(const char* filename, int count, int min, int max) 
{
    ofstream outfile(filename);

    if (!outfile) 
    {
        cerr << "Error: Unable to open file for writing.\n";
        exit(EXIT_FAILURE);
    }

    srand(time(0));

    for (int i = 0; i < count; ++i) 
    {
        int num = rand() % (max - min + 1) + min;
        outfile << num << endl;
    }

    outfile.close();
}

int main(int argc, char* argv[]) 
{
    if (argc != 4) 
    {
        cerr << "Usage: " << argv[0] << " COUNT MIN MAX\n";
        return EXIT_FAILURE;
    }

    int count = atoi(argv[1]);
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);

    generate("numbers.dat", count, min, max);

    cout << "Arg[0]:" << argv[0] << endl;
    cout << "Arg[1]:" << argv[1] << endl;
    cout << "Arg[2]:" << argv[2] << endl;
    cout << "Arg[3]:" << argv[3] << endl;

    return 0;
}
