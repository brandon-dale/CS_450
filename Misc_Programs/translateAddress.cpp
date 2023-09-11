/** *************************************************************
 * Program repeatedly translates a series of used specified     *
 * logical/physical addresses.                                  *
 * C++ version of the identical C# program by Dr. Richard Beer  *
 *                                                              *
 * @author Brandon Dale, Richard Beer                           *
 * @date August 29th, 2023                                      *
 * @file translateAddress.cpp                                   *
 ***************************************************************/


#include <iostream>
#include <vector>

using namespace std;


/** *****************************************************************
 * Translates a series of addresses that are specified by the user  *
 *******************************************************************/
void translateAddresses() {
    // Initialize the page table and the page size
    static const vector<int> PageTable = { 2, 6, 8, 7, 1, 4, 5 };
    static const int PageSize = 4096;

    // Print a Translation Header.
    cout << "Page Table Entries: ";
    for (auto& tableEntry : PageTable) {
        cout << tableEntry << " ";
    }
    cout << "\nPage Size = " << PageSize << endl;

    // Translate Addresses Until The User Quits
    do {
        cout << "\nEnter 'L' for Logical Addr -> Physical Addr  or 'P' for Physical Addr -> Logical Addr"
             << " or 'Q' to Quit:  ";
        char translateType;
        cin >> translateType;

        int LogicalAddr, PhysicalAddr;
        if (translateType == 'L') {
            // Get the logical address
            cout << "\nEnter Logical Address =  ";
            cin >> LogicalAddr;

            // Convert Logical address to physical address
            int PageTableIndex = LogicalAddr / PageSize;
            cout << "\nPageTableIndex = " << PageTableIndex << endl;
            if (PageTableIndex > PageTable.size()) {
                cout << "Page Fault: Corresponding Physical Address Not in Memory" << endl;
            } else {
                cout << "Offset into the page frame = " << LogicalAddr % PageSize << endl;
                PhysicalAddr = PageTable[PageTableIndex] * PageSize + LogicalAddr % PageSize;
                cout << "Corresponding Physical Address is = " << PhysicalAddr << endl;
            }
        }
        else if (translateType == 'P') {
            // Get the physical address
            cout << "\nEnter Physical Address = ";
            cin >> PhysicalAddr;

            // Convert the physical address to the logical address
            int PageFrame = PhysicalAddr / PageSize;
            bool found = false;
            for (int PageTableIndex = 0; PageTableIndex < 7; PageTableIndex++) {
                if (PageTable[PageTableIndex] == PageFrame) {
                    found = true;
                    LogicalAddr = PageTableIndex * PageSize + PhysicalAddr % PageSize;
                    cout << "\nCorresponding Logical Address is = " << LogicalAddr << endl;
                    break;
                }
            }
            if (!found) {
                cout << "\nPage Frame not in Page Table => Physical Address currently not allocated to the process"
                     << endl;
            }
        } else if (translateType == 'Q') {
            cout << "\nGoodbye" << endl;
            break;
        } else {
            cout << "\nINVALID TRANSLATION TYPE GIVEN -- TRY AGAIN" << endl;
        }
    } while (true);
}


/** *********************************************
 *  MAIN DRIVER CODE FOR TRANSLATING ADDRESSES  *
 ***********************************************/
int main() {

    translateAddresses();

    return 0;
}
