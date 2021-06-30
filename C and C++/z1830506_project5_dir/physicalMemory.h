//Ian Sullivan
//Assignment 5
//physicalMemory.h
//Due: 4/10/21

#ifndef physicalMemory_H
#define physicalMemory_H

#include <string>
#include <vector>
#include <cstddef>

using std::vector;
using std::string;
using std::cout;
using std::endl;

class PhysicalMemory {

    public:
        PhysicalMemory(int memSize);
        std::string access(int frameID); //access the frame, update the time stamp and time list
        void printMemory() const; //print out the physical memory layout
        int swapIn(std::string item); //returns the frame ID of the item just swapped in
        int getMemSize(); //returns memory size
        int getTime(); //returns a time from the time list
        int getNextAvailableFrame(); //get a frame, either available or via replacement
        

    private:

        vector<std::string> memory;
        vector<int> timeList;
        int currentTimeIndex; //current clock for timestamp
        int algorithmFlag; //indicate if using FIFO or LRU
        
        bool isFull(); //check if the memory is full
        void incrementFrames();
        

};

//Constructor for a physicalmemory object
//Takes a size to make the physical memory
PhysicalMemory::PhysicalMemory(int memSize) {

    for(int i = 0; i < memSize; i++) {

        memory.push_back("empty"); //this will push back some empty spaces
        timeList.push_back(-2);
    }
    
}

//Simple get method for the size of the memory
int PhysicalMemory::getMemSize() {

    return memory.size();
}

//Takes nothing
//This get's the next frame that will be used based on the time inside timeList
//Returns an int that is the frame that will be replaced
int PhysicalMemory::getTime() {

    for(uint i = 0; i < memory.size(); i++) {

        if(timeList.at(i) == (int)(memory.size() - 1)) {
            return i;
        }
    }
    
    return -1; //there was an error or the list isn't full
}

//Takes nothing
//Finds the next frame to be replaced and sets it = to -1 one to make the frame valid for use
//Returns an int that is the frame for use.
int PhysicalMemory::getNextAvailableFrame() {

    for(uint i = 0; i < memory.size(); i++) {

        //this is empty
        if(memory.at(i) == "empty") {

            timeList.at(i) = -1; //Set the same frame in timeList = 0 so we know that it's the most recent
            //incrementFrames(); //keep track of queue
            return i; //When there's nothing in the frames they should return here everytime
        }

    }
    //if we got this far then the frames are full
    for(uint i = 0; i < memory.size(); i++) {

        //the last item in the queue will have the value 3;
        if(timeList.at(i) == (int)(memory.size() - 1)) {

            //incrementFrames(); //move the queue up because this is only called when we're changing a frame
            timeList.at(i) = -1;
            return i;
        }
    }

    return 0;

}

//Simply gets the string at the frame id.
std::string PhysicalMemory::access(int frameID) {

    return memory.at(frameID);

}

//Takes a string to swap in
//Finds the next available frame and then increments the timeList in order to keep track of which frame is to be replaced
//Returns an int the frame that was replaced
int PhysicalMemory::swapIn(std::string item) {

    //std::cout << "oooohh swapping in " + item << std::endl;

    
    int tempI = getNextAvailableFrame(); //this is easier to return the same number
    incrementFrames(); //update the time list
    //cout << tempI << endl;

    memory.at(tempI) = item; //set the string in memory of the next available frame

    return tempI; //return the frame
    
    //return 0;

}

//Simply prints the physical memory in a formatted manner
void PhysicalMemory::printMemory() const {

    for (uint i = 0; i < memory.size(); i++) {

        cout << "Physical: Memory Frame: " << i << " contains: " << memory.at(i) << endl;
    }

    cout << endl; //just for formatting
}

//Takes nothing
//This goes through every valid item in timeList and increments it. If it's set to -2 it's not ready to be used yet.
//Returns nothing.
void PhysicalMemory::incrementFrames() {

    for(uint i = 0; i < timeList.size(); i++) {

        //if timelist is -1 then we know that the spot isn't being 'used'
        if(timeList.at(i) != -2) {
            timeList.at(i)++; //Everything in the makeshift Queue moves up one
        }
        
        
        
    }

}

#endif