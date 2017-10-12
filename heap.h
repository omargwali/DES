//
//  BinaryHeap.h
//  BinaryHeap
//
//  Omar Wali
//

#ifndef BinaryHeap_h
#define BinaryHeap_h

#include <stdio.h>

// Binary heap in ascending order

typedef struct BinaryHeap BinaryHeap;
typedef struct Node Node;       


BinaryHeap* createHeap   (void);
void        deleteHeap   (BinaryHeap* heap);
int         insert       (BinaryHeap* heap, void* data, double key);
Node *      extract_min  (BinaryHeap* heap);
size_t      size         (BinaryHeap* heap);
int         heapify_up   (BinaryHeap* heap);
int         heapify_down (BinaryHeap* heap, Node* root);
Node*       getLastNode  (BinaryHeap* heap);
void        printList    (BinaryHeap* heap);

#endif /* BinaryHeap_h */