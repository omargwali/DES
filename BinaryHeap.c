// BinaryHeap.c
// Omar Wali

#include "BinaryHeap.h"
#include <stdlib.h>


// Define an element in a heap. Contains
// - key:       the element's priority
// - data:      the data
// - left:      pointer to left node in heap
// - right:     pointer to right node in heap
// - parent:    pointer to the parent node in heap
struct Node {
    double key;
    void* data;
    
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};

// Define Binary Heap. Keep track of size and pointer to root
struct BinaryHeap {
    size_t size;
    Node* root;
};

// create heap
BinaryHeap* createHeap(void) {
    BinaryHeap* heap = malloc(sizeof(BinaryHeap));
    if (heap == NULL) {
        printf("Heap Null\n");
        return NULL;
    }
    
    heap->size = 0;
    heap->root = NULL;
    
    return heap;
}

// delete heap, call extract_min 
void deleteHeap(BinaryHeap* heap) {
    if (heap->size == 0) {
    	free(heap);
    } else {
    	printf("WARNING: DELETING HEAP WITH NONZERO SIZE.\n");
    	while (heap->size > 0) {
    		free(extract_min(heap));
            heap->size = heap->size - 1;
    	}
    	free(heap);
    }
}

Node* getLastNode(BinaryHeap* heap) {
    printf("get last\n");
    Node* node = heap->root;
    size_t hs = heap->size;

    if (hs == 0) {
        printf("Heap Empty.\n");
        return NULL;
    } else {
        int route = hs & 1;
        printf("get last 1\n");
        while (node->left != NULL && node->right != NULL) {
            printf("get last while\n");
            // find route by extracting last bit with bitwise and operation
            // then right shift for next iteration
            // if bit is 0 we go left and if 1 we go right
            node = (route == 0) ? node->left : node->right;
            hs = hs >> 1;
            route = hs & 1;
        }
        printf("get last post while\n");
        return (route == 0) ? node->left : node->right;;
    }
}

// insert element by creating node with given data and key
int insert(BinaryHeap* heap, void* data, double key) {
    printf("insert\n");
	//load heap size and calculate insertion index, also load root node
    size_t hs = heap->size + 1;
    Node* node = heap->root;

    //edge case for null root
    if (node == NULL) {
    	node = malloc(sizeof(Node));

    	node->key = key;
    	node->data = data;
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;

        heap->root = node;
        heap->size = heap->size + 1;

        printf("edge case insert\n");
        return 1;
    } else {
    	//while loop to traverse heap till insertion point (which will be node with null
    	// children)
        int route = hs & 1;
        printf("insert 1\n");
        while (node->left != NULL || node->right != NULL) {
            printf("insert while\n");
        	// find route by extracting last bit with bitwise and operation
        	// then right shift for next iteration
        	// if bit is 0 we go left and if 1 we go right
        	node = (route == 0) ? node->left : node->right;
            hs = hs >> 1;
            route = hs & 1;
        }
        printf("insert 2\n");
        //insertion point reached
        printf("route: %d\n", route)
;        Node* newNode;
        if (route) {
            node->right = malloc(sizeof(Node)); 
            newNode = node->right;
        } else {
            node->left = malloc(sizeof(Node));
            newNode = node->left;
        }

        newNode->parent = node;
        newNode->left = NULL;
        newNode->right = NULL;
    	newNode->key = key;
    	newNode->data = data;
    }

    int isHeapified;

    heap->size = heap->size + 1;
    isHeapified = heapify_up(heap);
    if (isHeapified) {
    	printf("Heapify successful.\n");
        return 1;
    } else {
    	printf("Heapify unsuccessful.\n");
        return 0;
    }
}

Node* extract_min(BinaryHeap* heap) {

    printf("extract_min\n");
    if (heap->size < 1) {
        printf("ERROR: HEAP UNDERFLOW\n");
        return NULL;
    }
    
    Node* min = heap->root;
    Node* lastNode = getLastNode(heap);
    if(lastNode->parent->right == NULL) {
        lastNode->parent->left = NULL;
    } else {
        lastNode->parent->right = NULL;
    }
    heap->root = lastNode;
    lastNode->left = min->left;
    lastNode->right = min->right;
    lastNode->parent = NULL;
    heap->size = heap->size - 1;
    heapify_down(heap, heap->root);

    return min;
    
}

size_t size(BinaryHeap* heap) {
    return heap->size;
}

int heapify_up(BinaryHeap* heap) {
//initialize pointers for all relevant nodes to be updated
    printf("Heap up\n");
    Node* lastNode = getLastNode(heap);
    Node* parent = lastNode->parent;
    printf("heap up after getLast, hs : %zu\n", heap->size);
    //edge case for heap with 2 levels
    if (heap->size == 2) {
        printf("edge case heap up\n");
        printf("%f %f\n", lastNode->key, parent->key);
        int shouldSwap = (lastNode->key < parent->key);
        printf("edge case heap up\n");
        if (shouldSwap) {

            heap->root = lastNode;
            lastNode->left = parent;
            lastNode->right = NULL;
            lastNode->parent = NULL;
            parent->left = NULL;
        } 

        return 1;
    } else if (heap->size == 3) {
        int shouldSwap = lastNode->key < parent->key;
        if (shouldSwap) {
            heap->root = lastNode;
            lastNode->left = parent->left;
            lastNode->right = parent;
            lastNode->parent = NULL;
            parent->left = NULL;
            parent->right = NULL;
        }
        return 1;
    } 

    Node* grandParent = parent->parent;
    //check if last node key is less than parent key violating heap property
    //loop until property is false
    while (lastNode->key < parent->key) {

        printf("heap up while\n");
        //check if node's parent is left child of grandparent to swap correctly
        int isLeftChild = (grandParent->left == parent) ? 1 : 0;
        if (isLeftChild) {
            //reassign child pointer to lastNode 
            grandParent->left = lastNode;
        } else {
            grandParent->right = lastNode;
        }

        //check if lastNode was left child of parent to swap correctly
        //make copies of children before reassigning 
        Node* leftChild = lastNode->left;
        Node* rightChild = lastNode->right;
        isLeftChild = (parent->left == lastNode) ? 1 : 0;
        if (isLeftChild) {
            lastNode->left = parent;
            lastNode->right = parent->right;
        } else {
            lastNode->right = parent;
            lastNode->left = parent->left;
        }
        

        parent->left = leftChild;
        parent->right = rightChild;

        parent->parent = lastNode;
        lastNode->parent = grandParent;
        
        
        parent = grandParent;
        grandParent = parent->parent;
    }

    return 1; 
}

int heapify_down(BinaryHeap* heap, Node* root) {
    printf("heap down\n");
    Node* left = root->left;
    Node* right = root->right;
    Node* smallest;

    if (left != NULL && left->key < root->key) {
        smallest = left;
    } else {
        smallest = root;
    }

    if (right!= NULL && right->key < smallest->key) {
        smallest = right;
    }

    if (smallest != root) {

        Node* smallestLeft = smallest->left;
        Node* smallestRight = smallest->right;
        
        //update all links, cover edge case
        if (heap->root == root) {
            heap->root = smallest;
            smallest->parent = NULL;
        }
        smallest->left = root;
        smallest->right = root->right;
        smallest->parent = root->parent;

        root->left = smallestLeft;
        root->right = smallestRight;
        root->parent = smallest;
        // ~R E C U R S I O N~
        heapify_down(heap, root);
    }

    return 1;
}

void printList(BinaryHeap* heap) {
    printf("print list\n");
    Node* node = extract_min(heap);
    int count = 0;
    while (node != NULL) {
        printf("%d: %f", count, node->key);
        node = extract_min(heap);
    }
}

int main() {
    BinaryHeap* priorityQueue = createHeap();

    for (int i = 0; i < 10; i++) {
        insert(priorityQueue, NULL, i*1.0);
    }

    printList(priorityQueue);

    return 1;
}













