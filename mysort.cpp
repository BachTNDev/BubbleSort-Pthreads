// Bach Nguyen
// CECS 325-02
// Prog 4 - Sortrace using pthreads
// Due Date: 03/28/2024
// I certify that this program is my own original work. I did not copy any part of this program from
// any other source. I further certify that I typed each and every line of code in this program.

#include <iostream>
#include <fstream>
#include <pthread.h>

#define NUM_THREADS 8
#define ARRAY_SIZE 1000000
#define SECTION_SIZE (ARRAY_SIZE / NUM_THREADS)

using namespace std;

void bubble(int A[], int size) 
{
    for (int i = 0; i < size - 1; ++i) 
    {
        for (int j = 0; j < size - i - 1; ++j) 
        {
            if (A[j] > A[j + 1]) 
            {
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
        }
    }
}

struct sortStuff
{
    int * start;
    int size;
};

void * bridge(void * ptr)
{
    sortStuff * arg = (sortStuff *)ptr;
    bubble(arg -> start, arg -> size);
    pthread_exit(NULL);   
}

void merge(int a1[], int s1, int a2[], int s2) 
{
    int * temp =  new int[s1 + s2];
    int i = 0, j = 0, k = 0;

    // Compare the first item in each array and copy the smallest to temp
    while (i < s1 && j < s2) 
    {
        if (a1[i] <= a2[j]) 
        {
            temp[k++] = a1[i++];
        } 
        else 
        {
            temp[k++] = a2[j++];
        }
    }

    // Copy remaining elements from the first array to temp
    while (i < s1) 
    {
        temp[k++] = a1[i++];
    }

    // Copy remaining elements from the second array to temp
    while (j < s2) 
    {
        temp[k++] = a2[j++];
    }

    // Copy all numbers from temp back to a1
    for (int m = 0; m < s1 + s2; ++m) 
    {
        a1[m] = temp[m];
    }
    delete[] temp;
}
void recursiveMerge(sortStuff sections[], int startIdx, int endIdx) 
{
    if (startIdx < endIdx) 
    {
        int midIdx = startIdx + (endIdx - startIdx) / 2;
        recursiveMerge(sections, startIdx, midIdx);
        recursiveMerge(sections, midIdx + 1, endIdx);
        merge(sections[startIdx].start, (midIdx - startIdx + 1) * SECTION_SIZE, 
            sections[midIdx + 1].start, (endIdx - midIdx) * SECTION_SIZE);
    }
}


int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        cerr << "Usage: " << argv[0] << " input_file output_file\n";
        return EXIT_FAILURE;
    }

    ifstream infile(argv[1]);
    if (!infile) 
    {
        cerr << "Error: Unable to open input file.\n";
        return EXIT_FAILURE;
    }

    ofstream outfile(argv[2]);
    if (!outfile) 
    {
        cerr << "Error: Unable to open output file.\n";
        return EXIT_FAILURE;
    }

    int * numbers = new int[ARRAY_SIZE];
    int count = 0;

    int num;
    while (infile >> num) 
    {
        numbers[count++] = num;
    }

    infile.close();
    
    pthread_t threads[NUM_THREADS];
    sortStuff sections[NUM_THREADS];

    // Split the array into sections and sort each section using pthreads
    for (int i = 0; i < NUM_THREADS; ++i) 
    {
        sections[i].start = &numbers[i * SECTION_SIZE];
        sections[i].size = SECTION_SIZE;
        pthread_create(&threads[i], NULL, bridge, (void *)&sections[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) 
    {
        pthread_join(threads[i], NULL);
    }

    // Merge the sorted sections - 2 at a time
    recursiveMerge(sections, 0, NUM_THREADS - 1);

    // Write sorted numbers to output file
    for (int i = 0; i < ARRAY_SIZE; ++i) 
    {
        outfile << numbers[i] << endl;
    }

    outfile.close();

    // Clean up dynamic memory
    delete[] numbers;

    return 0;
}
