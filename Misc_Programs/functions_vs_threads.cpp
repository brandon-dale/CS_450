/** ******************************************************************
 * C++ Version of the Original C# Program (CS_Function_vs_Thread)    *
 * by Dr. Richard Beer.                                              *
 * @author Brandon Dale, Richard Beer                                *
 *                                                                   *
 * Program compares the execution time for running two functions     *
 * which sleep for 5 and 10 seconds. Tests the total execution time  *
 * for using one thread vs two threads.                              *
 *                                                                   *
 * If you would like to experiment with the timing for different     *
 * functions, just overwrite the functions func_1 and func_2.        *
 *                                                                   *
 * @file functions_vs_threads.cpp                                    *
 * @date September 11th, 2023                                        *
 ********************************************************************/


#include <iostream>
#include <thread>
#include <chrono>

using namespace std;


/** ***********************************************************
 * Sleeps the current thread for 5 seconds and then prints    *
 * a message that the function is done.                       *
 *************************************************************/
void func_1 () {
    this_thread::sleep_for(chrono::milliseconds{5000});
    cout << "Function #1 is done\n";
}


/** ***********************************************************
 * Sleeps the current thread for 10 seconds and then prints   *
 * a message that the function is done.                       *
 *************************************************************/
void func_2 () {
    this_thread::sleep_for(chrono::milliseconds{10000});
    cout << "Function #2 is done\n";
}


/** ******************************************************************
 * Compares the run-time for running func_1 and func_2 when they     *
 * are run in the same thread and in different threads concurrently. *
 ********************************************************************/
void runThreadTests () {
    // Run test 1 - Both functions on the same thread
    cout << "\n---------- TEST 1 - RUN ON ONE THREAD ----------\n";
    auto start = chrono::steady_clock::now();

    func_1();
    func_2();

    auto stop = chrono::steady_clock::now();
    cout << "Execution Time of the two function calls: "
         << chrono::duration_cast<chrono::seconds>(stop - start).count()
         << " s\t("
         << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
         << " ms)\n\n";

    // Run test 2 - both functions on different threads concurrently
    cout << "---------- TEST 2 - RUN ON TWO THREADS ----------\n";
    start = chrono::steady_clock::now();

    thread t1{func_1};
    thread t2{func_2};
    t1.join();
    t2.join();

    stop = chrono::steady_clock::now();
    cout << "Execution Time of the two function calls: "
         << chrono::duration_cast<chrono::seconds>(stop - start).count()
         << " s\t("
         << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
         << " ms)\n\n";
}


/** **********************************************************
 * MAIN DRIVER CODE - Handles calling the testing function   *
 ************************************************************/
int main() {

    runThreadTests();

    return 0;
}
