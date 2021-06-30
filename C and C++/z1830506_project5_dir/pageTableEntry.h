//Ian Sullivan Z1830506
//Assignment 5
//pageTableEntry.h
//Due 4/10/21

#ifndef pageTableEntry_H
#define pageTableEntry_H

#include <vector>
#include "physicalMemory.h"
#include <string>

using std::vector;
using std::cout;
using std::endl;


struct pageTableEntry {

    bool valid;
    int physicalMemoryIndex;
};

//Also contains the page table class because pageTableEntry is really small otherwise.
class PageTable {

    public:
        PageTable(PhysicalMemory* pmIn, int tableSize); //initialize all entries to invalid
        void reference(int pageID, std::string item); //reference a logical page, if not in memory, call pageFault()
        //Page id will be the location in memory or the postion in the list
        //item will be the string relating to said page
        int getFaults() const; //return the number of faults
        void printTable() const; //print the layout of page table
        void reset(); //reset the validity flag to false and numberOfFaults to 0
        

    private:

        vector<pageTableEntry> entryList; //the page table!
        int numFaults;
        PhysicalMemory* mainPhysicalMemory;
        void pageFault(int pageID, std::string item); //increment numFaults; need to swap the item into physical memory by calling
        //swapIn() of the PhysicalMemory class
        void updateReplacedEntry(int replacedIndex, int pageID); //after page replacement, update the page table correspondingly
        //by making the original mapping for the frame invalid

};

PageTable::PageTable(PhysicalMemory* pmIn, int tableSize) {

    for(int i = 0; i < tableSize; i++) {
        pageTableEntry p;
        entryList.push_back(p);
    } //this contains all of the page id's 0-7? evne if they're not in memory
    numFaults = 0; //the number of faults that have occured
    mainPhysicalMemory = pmIn; //This links the current objects phys memory to the pyhsical memory class

    //We go through all the page ids and set their current validty to false
    for(int i = 0; i < tableSize; i++) {
        entryList.at(i).valid = false;
        entryList.at(i).physicalMemoryIndex = -1;
    }

    //cout << "Debug memSize: " << mainPhysicalMemory->getMemSize() << endl;
    

}

//Destructor for Page Table
//Just deletes the dynamically created entryList

//reference takes a pageID and the name of an item(function to execute)
//It first needs to check if the item is in memory
//If the item is in memory we simply need to print the item out and thats 'like executing the function'
//If the item isn't in memory we have a page fault and we need to move the item into memory.
//We then need to 'restart the process'
//So we check the memory again and 'execute the funtion'
//returns nothing
void PageTable::reference(int pageID, std::string item) {

    //this checks the memory for the item/string to see if it's currently valid
    for(int i = 0; i < mainPhysicalMemory->getMemSize(); i++) {

        //call access with the i in order to get the correct page
        //if the item is in memory
        //cout << "Debug item: " << item << endl;
        //cout << "Debug trying to access at: " << i << endl;
        //cout << "Debug Item at access: " << mainPhysicalMemory->access(i) << endl; 
        
        if (item == mainPhysicalMemory->access(i)) {

            cout << "Physical: Accessed frameID: " << i << " contains: " << item << std::endl; //this is a debug line
            
            //I think we return here?
            return;

        }
    }

    //if we got this far it's not in memory so put it in memory
    pageFault(pageID, item);

    //we might run the above loop again here or in pageFault()
    


}

//Takes an int pageID and a string item
//The pageId is the table entry that needs to be updated and the string is what we store in physical memory
//It returns nothing
void PageTable::pageFault(int pageID, std::string item) {

    numFaults++; //we have another fault
    //cout << endl;
    
    //okay so we know that getTime returns the *last item in the queue's* frame
    //that frame is indexed somewhere in entryList
    //so like if the fram is say i = 2 then in entry list whatever phsyical index is = 2 is the one being replaced
    int replacedFrame = mainPhysicalMemory->getTime(); //get the last items frame
    //cout << "Debug: Frame to be replaced : " << replacedFrame << endl;
    for(uint i = 0; i < entryList.size(); i++) {

        if(entryList.at(i).physicalMemoryIndex == replacedFrame) {
            
            //cout << "Debug: Replacing EntryList at " << i << ": Memory Inded: " << entryList.at(i).physicalMemoryIndex << endl;
            entryList.at(i).physicalMemoryIndex = -1; //it's no longer valid
            entryList.at(i).valid = false; //This should invalidate the pageID before replacing it
        }
    }


    //we need to call swap in to get the item into memory
    int memLocation = mainPhysicalMemory->swapIn(item); //the physical memory recieves a string and returns a location
    entryList.at(pageID).physicalMemoryIndex = memLocation; //the page becomes linked to an actual memory location here
    entryList.at(pageID).valid = true;
    //cout << "Debug: Replaced EntryList at " << pageID << ": Memory Inded: " << entryList.at(pageID).physicalMemoryIndex << endl;

    cout << "Page Table: Page Fault Occured" << endl;

    for(int i = 0; i < mainPhysicalMemory->getMemSize(); i++) {

        //call access with the i in order to get the correct page
        //if the item is in memory
        //cout << "Debug item: " << item << endl;
        //cout << "Debug trying to access at: " << i << endl;
        //cout << "Debug Item at access: " << mainPhysicalMemory->access(i) << endl; 
        
        if (item == mainPhysicalMemory->access(i)) {

            cout << "Physical: Stored: " << item << endl; //this is a debug line
            
            return;

        }
    }

    //cout << "Debug: Swapped in " << item << " at " << memLocation << endl;
    //cout << "Debug useless: " << pageID << endl;
    //cout << "Debug: entryList at pageID = " << entryList.at(pageID).physicalMemoryIndex << " is " << entryList.at(pageID).valid << endl;  

}

//Takes nothing
//Prints the entryTable in a formatted manner
//Returns nothing
void PageTable::printTable() const {

    for(uint i = 0; i < entryList.size(); i++) {

        cout << "PageTable: Index " << i << ": Physical Index: " << entryList.at(i).physicalMemoryIndex << ": In Use: " << entryList.at(i).valid << endl;
    }

    cout << "PageTable: Current number of page faults: " << getFaults() << endl;
}

//Get method to return the number of faults
int PageTable::getFaults() const {

    return numFaults;
}


#endif