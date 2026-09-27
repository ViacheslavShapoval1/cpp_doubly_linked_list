/*
Author: Viacheslav Shapoval
Title: Templated List Class
Description: Implementation of a generic doubly linked list
             that supports deep copying, moving, insertion, removal,
             and O(min(index, n - index)) traversal.
Date Created: 9/18/2026
Date Last Modified: 9/20/2026
*/

#include <iostream>     // Included for console output operations
#include <string>       // Included to test list's compatibility with strings
#include <stdexcept>    // Included to catch std::out_of_range exceptions
#include "LinkedList.h"

using namespace std;

// description: A custom type that tracks exactly how many copies and moves of 
//              itself currently exist to verify O(1) list ownership transfers.
struct MoveTracker {
    // Variable Dictionary for MoveTracker
    static int default_const;  // Number of default and value constructor calls
    static int copy_const;     // Number of copy constructor calls
    static int move_const;     // Number of move constructor calls
    static int copy_assign;    // Number of copy assignment operator calls
    static int move_assign;    // Number of move assignment operator calls
    static int active_objects; // Currently allocated objects (memory leaks)

    int value;

    MoveTracker(int v = 0) : value(v) { 
        ++default_const; 
        ++active_objects;
    }
    MoveTracker(const MoveTracker& other) : value(other.value) { 
        ++copy_const; 
        ++active_objects;
    }
    MoveTracker(MoveTracker&& other) noexcept : value(other.value) { 
        ++move_const; 
        ++active_objects;
    }
    ~MoveTracker() { 
        --active_objects; 
    }
    MoveTracker& operator=(const MoveTracker& other) { 
        value = other.value; 
        ++copy_assign; 
        return *this; 
    }
    MoveTracker& operator=(MoveTracker&& other) noexcept { 
        value = other.value; 
        ++move_assign; 
        return *this; 
    }

    // description: Resets all counters back to zero
    // return: void
    // precondition: static counters exist
    // postcondition: all counters (except active_objects) are set to 0
    static void reset() {
        default_const = copy_const = move_const = copy_assign = move_assign = 0;
    }
};

// Initialize static counters for the MoveTracker
int MoveTracker::default_const = 0;
int MoveTracker::copy_const = 0;
int MoveTracker::move_const = 0;
int MoveTracker::copy_assign = 0;
int MoveTracker::move_assign = 0;
int MoveTracker::active_objects = 0;

// Required for the list's operator<< to compile
ostream& operator<<(ostream& os, const MoveTracker& mt) {
    return os << mt.value;
}

// Macro to format and track test results directly to console.
#define RUN_TEST(num, name, cond) \
if (cond) { \
    cout << "PASS: " << num << ". " << name << "\n"; \
    ++passed; \
} else { \
    cout << "FAIL: " << num << ". " << name << "\n"; \
} \
++total;

// description: Main execution driver for testing the LinkedList class
// return: int
// precondition: LinkedList.h is correctly implemented
// postcondition: Returns 0 upon successful program execution
int main() {
    // Variables
    int passed = 0; // Total number of successfully passed tests
    int total = 0;  // Total number of tests executed

    // Ensures all list destructors run before the final memory check
    {
        // Basic Setup and Boundary Operations
        // Tests empty construction, pushing, popping, and front/back
        LinkedList<int> empty_list;
        RUN_TEST(1, "construction of an empty list", empty_list.empty() && 
            empty_list.size() == 0);

        LinkedList<int> list;
        list.pushFront(10);
        RUN_TEST(2, "insertion at the front", list.front() == 10 && 
            list.size() == 1);

        list.pushBack(20);
        RUN_TEST(3, "insertion at the back", list.back() == 20 && 
            list.size() == 2);

        // Positional Operations
        // Tests insertion and removal at arbitrary indexes
        list.insert(0, 5);   // Beginning
        list.insert(3, 30);  // End
        list.insert(2, 15);  // Middle
        RUN_TEST(4, "insertion at the beginning, middle, and end", 
            list.size() == 5 && list.at(2) == 15);

        list.popFront();
        RUN_TEST(5, "removal from the front", list.front() == 10 && 
            list.size() == 4);

        list.popBack();
        RUN_TEST(6, "removal from the back", list.back() == 20 && 
            list.size() == 3);

        list.erase(1);
        RUN_TEST(7, "removal from the middle", list.at(1) == 20 && 
            list.size() == 2);

        RUN_TEST(8, "access using front, back, and at", list.front() == 10 && 
            list.back() == 20 && list.at(0) == 10);

        list.insert(1, 15);
        RUN_TEST(9, "forward ordering of values", list.at(0) == 10 && 
            list.at(1) == 15 && list.at(2) == 20);

        list.clear();
        list.pushBack(99);
        list.popFront();
        RUN_TEST(10, "correct behavior after removing the only element", 
            list.empty() && list.size() == 0);

        // Rule of Five - Deep Copying
        // Tests the copy constructor and copy assignment operator
        list.pushBack(1);
        list.pushBack(2);
        LinkedList<int> copy_list(list);
        copy_list.at(0) = 99;
        RUN_TEST(11, "deep-copy construction", list.at(0) == 1 && 
            copy_list.at(0) == 99);

        LinkedList<int> assign_list;
        assign_list.pushBack(500); 
        assign_list = list;
        assign_list.at(0) = 88;
        RUN_TEST(12, "deep-copy assignment", list.at(0) == 1 && 
            assign_list.at(0) == 88);

        LinkedList<int>* list_ptr = &list;
        list = *list_ptr; // Trigger self-assignment
        RUN_TEST(13, "copy self-assignment", list.size() == 2 && 
            list.at(0) == 1);

        // Rule of Five - Move Semantics
        // Tests ownership transfer in O(1) time without copying
        LinkedList<int> move_list(std::move(copy_list));
        RUN_TEST(14, "move construction ownership transfer",
            copy_list.empty() &&
            move_list.size() == 2);

        LinkedList<int> move_assign;
        move_assign = std::move(assign_list);
        RUN_TEST(15, "move assignment ownership transfer", assign_list.empty()
            && move_assign.size() == 2);

        LinkedList<int>* move_ptr = &move_list;
        move_list = std::move(*move_ptr); // Trigger self-assignment
        RUN_TEST(16, "move self-assignment", move_list.size() == 2);

        copy_list.pushBack(42);
        RUN_TEST(17, "reuse of an object after it has been moved from", 
            copy_list.size() == 1 && copy_list.front() == 42);

        // Templated Type Verification
        // Tests list execution with alternate data types
        RUN_TEST(18, "lists containing integers", move_list.back() == 2);

        LinkedList<string> str_list;
        str_list.pushBack("alpha");
        str_list.pushBack("beta");
        RUN_TEST(19, "lists containing strings", str_list.size() == 2 && 
            str_list.front() == "alpha");

        // Explicit Memory and Move Tracking
        // Verifies std::move
        MoveTracker::reset();
        LinkedList<MoveTracker> tracker_list;
        tracker_list.pushBack(MoveTracker(100)); // Rvalue insertion
        MoveTracker lvalue_item(200);
        tracker_list.pushBack(lvalue_item);      // Lvalue insertion (copies)
        
        // Capture baseline counts prior to executing a full list move
        int copies_before_move = MoveTracker::copy_const;
        int moves_before_move = MoveTracker::move_const;
        
        LinkedList<MoveTracker> tracker_moved(std::move(tracker_list));
        
        // Verifies list move transfers pointers without touching elements
        bool pure_pointer_transfer =
            (MoveTracker::copy_const == copies_before_move &&
            MoveTracker::move_const == moves_before_move);

        RUN_TEST(20, "lists containing user-defined movable type and "
                     "O(1) transfer",
            tracker_moved.size() == 2 && pure_pointer_transfer);

        // Exception Handling
        // Tests boundary protection against bad user indexes
        bool bounds_caught = false;
        try {
            empty_list.at(50); 
        } catch (const std::out_of_range&) {
            bounds_caught = true;
        }
        RUN_TEST(21, "exception handling for invalid positions", bounds_caught);

    } // end of block scoping; forces all destructors to execute before
      // final leak check

    RUN_TEST(22, "destruction of nonempty lists (zero memory leaks)", 
        MoveTracker::active_objects == 0);

    cout << "\nTests passed: " << passed << " / " << total << "\n";
    return 0;
}