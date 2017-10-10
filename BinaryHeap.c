// BinaryHeap.c
// Omar Wali

#include "BinaryHeap.h"
#include <stdlib.h>

#define HEAPIFY_UP     0
#define HEAPIFY_DOWN   1

// Define an element in a heap. Contains
// - key:       the element's priority
// - data:      the data
// - left:      pointer to left node in heap
// - right:     pointer to right node in heap
// - parent:    pointer to the parent node in heap
typedef struct Node {
    double key;
    void* data;
    
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

// Define Binary Heap. Keep track of size and pointer to root and lastNode
struct BinaryHeap {
    size_t size;
    Node* root;
    Node* lastNode;
};

BinaryHeap* createHeap(void) {
    BinaryHeap* heap = malloc(sizeof(BinaryHeap));
    if (heap == NULL)
        return NULL;
    
    heap->size = 0;
    heap->root = NULL;
    
    return heap;
}

void deleteHeap(BinaryHeap* heap) {
    if (heap->size == 0) {
    	free(heap);
    } else {
    	printf("WARNING: DELETING HEAP WITH NONZERO SIZE.\n");
    	while (heap->size > 0) {
    		free(extract_min(heap));
    	}
    	free(heap);
    }
}

int insert(BinaryHeap* heap, void* data, double key) {
	//load heap size and calculate insertion index, also load root node
    size_t hs = heap->size + 1;
    Node* node = heap->root;

    //edge case for null root
    if (node == NULL) {
    	node = malloc(sizeof(Node));
    	node->key = key;
    	node->data = data;
    } else {
    	//while loop to traverse heap till insertion point (which will be node with null
    	// children)
        while (node != NULL) {
        	// find route by extracting last bit with bitwise and operation
        	// then right shift for next iteration
        	// if bit is 0 we go left and if 1 we go right
        	int route = hs & 1;
        	hs = hs >> 1;
        	node = (route == 0) ? node->left : node->right;
        }

        //insertion point reached
        node = malloc(sizeof(Node));
    	node->key = key;
    	node->data = data;
    }

    int isHeapified;
    isHeapified = heapify(heap);
    if (isHeapified) {
    	printf("Heapify successful.\n");
    } else {
    	printf("Heapify unsuccessful.\n");
    }
}

Node* extract_min(BinaryHeap* heap) {
    if heap->size < 1 {
        printf("ERROR: HEAP UNDERFLOW\n");
        return NULL;
    }
    
    Node* min = heap->root;
    Node* max = heap->lastNode;


    
}

size_t size(BinaryHeap* heap) {
    // TODO
    return 0;
}

int heapify(BinaryHeap* heap) {

}