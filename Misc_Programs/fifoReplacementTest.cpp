/** ***************************************************************
 *  FIFO Page Replacement Testing Program                         *
 *  Tests the number of page faults (avg, min, max) against a set *
 *  of reference strings for an increasing number of pages.       *
 *                                                                *
 *  The user will be prompted for the testing parameters.         *
 *                                                                *
 *  @author Brandon Dale                                          *
 *  @file fifoReplacementTest.cpp                                 *
 *  @date August 30th, 2023                                       *
 *****************************************************************/


#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>
#include <random>
#include <string>
#include <cassert>

using namespace std;


/** *******************************************************************
 * Returns the number of page faults that occur                       *
 * @param numPages  The number of pages available                     *
 * @param refString The string of page references to use as a vector  *
 * @return the number of page faults that occurred                    *
 *********************************************************************/
int numFIFOFaults(int numPages, vector<int>& refString) {
    // Initialize the empty set of pages, queue, and the page fault counter
    unordered_set<int> pages;
    queue<int> q;
    int numFaults = 0;

    // Process each page reference
    for (auto& ref : refString) {
        if (pages.size() < numPages) {
            // If there are empty pages and the current page reference is not in the set
            // of pages, add it and add a page fault
            if (pages.find(ref) == pages.end()) {
                pages.insert(ref);
                numFaults++;
                q.push(ref);
            }
        }
        else {
            // If page reference is not in the set of pages -> get rid of oldest and
            // add a page fault
            if (pages.find(ref) == pages.end()) {
                int valToErase = q.front();
                q.pop();
                pages.erase(valToErase);
                pages.insert(ref);
                q.push(ref);
                numFaults++;
            }
        }
    }

    return numFaults;
}


/** ************************************************************************************
 * Evaluates and prints a report on the average number of page faults using the FIFO   *
 * page replacement algorithm for a specified range of pages and references.           *
 * Each reference string has random values [0, refStrLen)                              *
 * @param minPages   The minimum number of pages to test                               *
 * @param maxPages   The maximum number of pages to test                               *
 * @param pageInc    The increment size between each tested number of pages            *
 * @param refStrLen  The length of each reference string                               *
 * @param numRefStrings The number of reference strings to test each page size against *
 **************************************************************************************/
void testFaults(int minPages, int maxPages, int pageInc, int refStrLen, int numRefStrings) {
    // Error Checking on Parameters
    assert(minPages <= maxPages);
    assert(pageInc > 1);
    assert(refStrLen > 0);
    assert(numRefStrings > 0);

    // Initialize the random number generator
    const unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(0, refStrLen - 1);

    // Print a testing header
    printf("\nNum Pages  |  Num References  |  Num Ref Strings  |  Avg Page Faults  |  Min Page Faults  |  Max Page Faults\n");
    printf("------------------------------------------------------------------------------------------------------------\n");

    // Generate a set of reference strings
    vector<vector<int> > refStrings(numRefStrings, vector<int>(refStrLen, -1));
    for (auto& refString : refStrings) {
        for (auto& ref : refString) {
            ref = distribution(generator);
        }
    }

    // Test each page size on the set of reference strings
    for (int pages = minPages; pages <= maxPages; pages += pageInc) {
        // Test the number of pages against each reference string
        int minFaults = INT_MAX, maxFaults = INT_MIN, sumFaults = 0;
        for (int i = 0; i < numRefStrings; i++) {
            int faults = numFIFOFaults(pages, refStrings[i]);
            minFaults = min(minFaults, faults);
            maxFaults = max(maxFaults, faults);
            sumFaults += faults;
        }
        double mean = double(sumFaults) / double(numRefStrings);

        // Print a report of the test
        printf("%7i       %10i   %14i      %17.2f %17i %20i\n", pages, refStrLen, numRefStrings, mean, minFaults, maxFaults);
    }
    printf("\n");
}


/** *************************************************************************
 * Prompts the user for an integer value and sets valToSet with that value. *
 * User is prompted to set the variable specified by varTitle.              *
 * @param valToSet The value to set if it is not the default value          *
 * @param varTitle The variable title to prompt the user with               *
 ***************************************************************************/
void getUserInput(int& valToSet, const string& varTitle) {
    static const int DEFAULT_VAL = 0;
    int userVal;
    printf("Enter value for %s or '0' to use the default value (%i): ", varTitle.c_str(), valToSet);
    cin >> userVal; 
    assert(userVal >= 0);
    valToSet = (userVal == DEFAULT_VAL) ? valToSet : userVal;
}


/** ***********************************************************
 *  Main driver function for testing and parameter gathering  *
 *************************************************************/
int main(int argc, char* argv[]) {
    // Set the default testing values
    int minPages = 1000;
    int maxPages = minPages + 1000;
    int pageInc = 100;
    int refStringLen = 10000;
    int numRefStrings = 10;

    // Get user input for testing parameters
    getUserInput(minPages, "minimum pages to test");
    getUserInput(maxPages, "maximum pages to test");
    getUserInput(pageInc,  "test page increment size");
    getUserInput(refStringLen,  "number of references per test");
    getUserInput(numRefStrings, "number of reference strings per page size");

    // Run the tests
    testFaults(minPages, maxPages, pageInc, refStringLen, numRefStrings);

    return 0;
}
