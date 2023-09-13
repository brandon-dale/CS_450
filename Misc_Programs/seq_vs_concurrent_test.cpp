/** ******************************************************************
 * SSU - CS 450 (Operating Systems) - Thread Timing Test Program     *
 * @author Brandon Dale                                              *
 *                                                                   *
 * Program compares the execution time for running one function      *
 * which (arbitrarily) calculates pi^2 100000000 times.              *
 * Tests the total execution time for running function sequentially  *
 * versus on separate threads concurrently                           *
 *                                                                   *
 * @file seq_vs_concurrent_test.cpp                                  *
 * @date September 13th, 2023                                        *
 ********************************************************************/


#include <thread>
#include <chrono>

using namespace std;


/** ***********************************************************
 * Calculates pi^2 100000000 times                            *
 *************************************************************/
void testFunc () {
    double pi_sq;
    double pi = 3.1415;
    for (unsigned i = 0; i < 100000000; i++) {
        pi_sq = pi * pi;
    }
}


/** ********************************************************************
 * Calculates and prints the execution time to run the testFunc        *
 * function between 1 and maxThreads times. Tests time to run function *
 * calls sequentially and time to fun function calls concurrently      *
 *                                                                     *
 * @param maxThreads - the (inclusive) maximum number of calls/threads *
 *                     to test                                         *
 **********************************************************************/
void runThreadTests (int maxThreads) {
    // Print a test header
    printf("Calls     Concurrent Time (ms)    Sequential Time (ms)\n");

    for (int calls = 1; calls <= maxThreads; calls++) {
        /* * * * * Time the sequential execution time * * * * */
        auto start = chrono::steady_clock::now();

        for (int i = 0; i < calls; i++) {
            testFunc();
        }

        auto stop = chrono::steady_clock::now();
        auto seqTime = chrono::duration_cast<chrono::milliseconds>(stop - start).count();

        /* * * * * Time the concurrent execution time * * * * */
        start = chrono::steady_clock::now();

        thread threadArr[calls];
        for (int i = 0; i < calls; i++) {
            threadArr[i] = thread{testFunc};
        }
        for (auto& t : threadArr) {
            t.join();
        }

        stop = chrono::steady_clock::now();
        auto conTime = chrono::duration_cast<chrono::milliseconds>(stop - start).count();

        // Print the test results
        printf(" %2i           %5lli                    %5lli\n", calls, conTime, seqTime);
    }
}


/** **********************************************************
 * MAIN DRIVER CODE - Handles calling the testing function   *
 ************************************************************/
int main () {

    runThreadTests(20);

    return 0;
}
