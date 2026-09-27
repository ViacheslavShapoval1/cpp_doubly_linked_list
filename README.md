# Templated Doubly Linked List

## Overview
This repository contains a generic, templated doubly linked list implemented in C++. The container supports deep copying,
move semantics, dynamic insertion/removal, and highly optimized positional traversal. 

The primary engineering focus of this project was enforcing strict memory safety via the copy-and-swap idiom and achieving O(1) 
ownership transfers during move operations.

## Architectural Highlights
*   **Optimized Traversal:** Positional operations (like `at()`, `insert()`, and `erase()`) utilize an O(min(index, n - index))
*   traversal algorithm, calculating the shortest path from either the head or the tail to minimize iteration time.
*   **Exception Safety:** The copy-assignment operator utilizes the copy-and-swap idiom, ensuring a Strong Exception Guarantee.
*   If memory allocation fails during the copy phase, the original list remains entirely untainted.
*   **O(1) Move Semantics:** Move construction and move assignment operate in strictly O(1) constant time. Ownership of the
*   node chain is transferred by reassigning the internal `head`, `tail`, and `elementCount` pointers, bypassing individual node
*   allocation or traversal.

## Testing & Memory Validation
The project includes a systematic, macro-driven test driver executing 22 distinct assertions to verify boundary conditions and 
exception handling (`std::out_of_range`).

To mathematically prove the integrity of the O(1) move semantics and ensure zero memory leaks, the test suite injects 
a custom `MoveTracker` struct. This object utilizes static counters to track every constructor and destructor call, definitively 
verifying that the memory manager executes constant-time pointer pilfering without triggering deep copies or individual element moves. 
Memory safety is further validated against GCC's AddressSanitizer.

## Compilation & Execution
Compiled under C++11 utilizing GCC. 

```bash
g++ -std=c++11 -Wall -Wextra -Wpedantic -Wconversion -fsanitize=address,undefined -fno-omit-frame-pointer main.cpp -o linkedlist
./linkedlist
