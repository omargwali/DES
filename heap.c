// BinaryHeap.c
// Omar Wali

#include "heap.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
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
    Node* lastNode;
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

unsigned int getRoute(unsigned int hs, int level) {

    int levels = floor(log2((hs)));
    int route;
    for (int i = 0; i < levels - level; i++) {
        route = hs & 1;
        hs >>= 1;
    }
    return route;
}

Node* getLastNode(BinaryHeap* heap) {
    
    Node* node = heap->root;
    int level;
    unsigned int hs = (unsigned int) heap->size;
    if (hs == 0) {
        printf("Heap Empty.\n");
        return NULL;
    } else if (hs == 1) {
        return node;
    } else if (hs == 2) {
        return node->left;
    } else if (hs == 3) {
        return node->right;
    } else {
        

        unsigned int route;
        level = 0;
        int levels = floor(log2((hs)));
        printf("levels: %d\n", levels);
        while (level < levels) {
            route = getRoute(hs, level);
            printf("route: %d key: %f\n", route, node->key);
            node = (route == 0)? node->left : node->right;
            level++;
        }
        printf("route: %d key: %f\n", route, node->key);
        
    }

    return node;
}

int insert(BinaryHeap* heap, void* data, double key) {
    printf("insert\n");
    Node* node = heap->root;
    size_t hs = heap->size;
    if (hs == 0) {
        node = malloc(sizeof(Node));

        node->key = key;
        node->data = data;
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;

        heap->root = node;
        heap->size = heap->size + 1;

    } else if (hs == 1) {
        Node* nodeLeft;
        nodeLeft = malloc(sizeof(Node));

        nodeLeft->key = key;
        nodeLeft->data = data;
        nodeLeft->parent = node;
        nodeLeft->left = NULL;
        nodeLeft->right = NULL;

        node->left = nodeLeft;
        heapify_up(heap);
        heap->size = heap->size + 1;
    } else if (hs == 2) {
        Node* nodeRight;
        nodeRight = malloc(sizeof(Node));

        nodeRight->key = key;
        nodeRight->data = data;
        nodeRight->parent = node;
        nodeRight->left = NULL;
        nodeRight->right = NULL;

        node->right = nodeRight;
        heapify_up(heap);
        heap->size = heap->size + 1;
    } else {
        unsigned int hs = (unsigned int) heap->size + 1;

        int level = 0;
        unsigned int route;
        Node* previousNode;

        while (node != NULL) {
            route = getRoute(hs, level);
            printf("insert while\n");
            previousNode = node;
            node = (route == 0)? node->left : node->right;
            level++;
        }

        node = malloc(sizeof(Node));
        node->key = key;
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        node->parent = previousNode;

        if (route == 0) {
            previousNode->left = node;
        } else {
            previousNode->right = node;
        }

        heap->size = heap->size + 1;
        printf("node key: %f, previousNode key: %f\n", node->key, previousNode->key);

        int isHeapified = heapify_up(heap);
        if (isHeapified) {
            printf("Heapify successful.\n");
            return 1;
        } else {
            printf("Heapify unsuccessful.\n");
            return 0;
        }
    }

}

Node* extract_min(BinaryHeap* heap) {

    printf("extract_min\n");
    if (heap->size < 1) {
        printf("ERROR: HEAP UNDERFLOW\n");
        return NULL;
    } else if (heap->size == 1) {
        return heap->root;
    }
    
    Node* min = heap->root;
    heap->root = getLastNode(heap);
    heap->root->left = min->left;
    heap->root->right = min->right;
    Node* lastNode = getLastNode(heap);
    printf("extract last node key: %f\n", lastNode->key);
    if(lastNode->parent->right != NULL) {
        printf("right null\n");
        lastNode->parent->right = NULL;
    } else if (lastNode->parent->left != NULL) {
        lastNode->parent->left = NULL;
    }
    
    heap->root->parent = NULL;
    heap->size = heap->size - 1;
    heapify_down(heap, heap->root);
    
    
    return min;
    
}

int heapify_up(BinaryHeap* heap) {
//initialize pointers for all relevant nodes to be updated
    printf("Heap up\n");
    if (heap->size < 1) return 1;
    Node* lastNode = getLastNode(heap);
    printf("post get, lastNode key: %f\n", lastNode->key);
    Node* parent = lastNode->parent;
    Node* grandParent = parent->parent;
    printf("post set family\n");
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
    //printf("%f, %f\n", root->left->key, root->right->key);
    
    Node* left = root->left;
    Node* right = root->right;
    Node* smallest;
    int isLeft = 0;

    if (left != NULL && left->key < root->key) {
        printf("cmon son\n");
        isLeft = 1;
        smallest = left;
    } else {
        smallest = root;
        

    }

    if (right!= NULL && right->key < smallest->key) {
        printf("cmon son right\n");
        isLeft = 0;
        smallest = right;
    }

    if (smallest != root) {
        printf("cmon son\n");
        Node* smallestLeft = smallest->left;
        //printf("smallest left value: %f\n", smallestLeft->key);
        Node* smallestRight = smallest->right;
        
        //update all links, cover edge case
        if (heap->root == root) {
            heap->root = smallest;
            smallest->parent = NULL;
        }

        
        printf("isleft: %d\n", isLeft);
        if (isLeft) {
            smallest->left = root;
            smallest->right = root->right;
            smallest->parent = root->parent;
        } else {
            smallest->right = root;
            smallest->left = root->left;
            smallest->parent = root->parent;
        }

        if (root->parent == heap->root) {
            if (heap->root->left == root) {
                heap->root->left = smallest;
            } else {
                heap->root->right = smallest;
            }
            //return 1;
        }
        //printf("smallest left value: %f\n", smallestLeft->key);

        
        
        root->left = smallestLeft;
        root->right = smallestRight;
        root->parent = smallest;
        // ~R E C U R S I O N~
        printf("root value: %f\n", root->key);
        if (smallest == heap->root) return 1;
        heapify_down(heap, root);
    }

    return 1;
}

void printList(BinaryHeap* heap) {
    printf("print list\n");
    Node* node = extract_min(heap);
    int count = 0;
    while (node != NULL) {
        printf("%d: %f\n", count, node->key);
        node = extract_min(heap);
        count++;
    }
}

int main() {
    BinaryHeap* priorityQueue = createHeap();

    
    insert(priorityQueue, NULL, 2);
    insert(priorityQueue, NULL, 1);
    insert(priorityQueue, NULL, 3);
    insert(priorityQueue, NULL, 5);
    insert(priorityQueue, NULL, 4);
    insert(priorityQueue, NULL, 6);

    printf("%d, %f\n", 0, priorityQueue->root->key);
    printf("%d, %f\n", 1, priorityQueue->root->left->key);
    printf("%d, %f\n", 2, priorityQueue->root->right->key);
    printf("%d, %f\n", 3, priorityQueue->root->left->left->key);
    printf("%d, %f\n", 4, priorityQueue->root->left->right->key);
    printf("%d, %f\n", 5, priorityQueue->root->right->left->key);
    printf("%d, %f\n", 6, priorityQueue->root->right->right->key);

    extract_min(priorityQueue);

    printf("%d, %f\n", 0, priorityQueue->root->key);
    printf("%d, %f\n", 1, priorityQueue->root->left->key);
    printf("%d, %f\n", 2, priorityQueue->root->right->key);
    printf("%d, %f\n", 3, priorityQueue->root->left->left->key);
    printf("%d, %f\n", 4, priorityQueue->root->left->right->key);
    printf("%d, %f\n", 5, priorityQueue->root->right->left->key);
    //printf("%d, %f\n", 6, priorityQueue->root->right->right->key);

    extract_min(priorityQueue);
    printf("%d, %f\n", 0, priorityQueue->root->key);
    printf("%d, %f\n", 1, priorityQueue->root->left->key);
    printf("%d, %f\n", 2, priorityQueue->root->right->key);
    printf("%d, %f\n", 3, priorityQueue->root->left->left->key);
    printf("%d, %f\n", 4, priorityQueue->root->left->right->key);
    
    extract_min(priorityQueue);
    printf("%d, %f\n", 0, priorityQueue->root->key);
    printf("%d, %f\n", 1, priorityQueue->root->left->key);
    printf("%d, %f\n", 2, priorityQueue->root->right->key);
    printf("%d, %f\n", 3, priorityQueue->root->left->left->key);


    extract_min(priorityQueue);
    printf("%d, %f\n", 0, priorityQueue->root->key);
    printf("%d, %f\n", 1, priorityQueue->root->left->key);
    printf("%d, %f\n", 2, priorityQueue->root->right->key);

    extract_min(priorityQueue);
    printf("%d, %f\n", 0, priorityQueue->root->key);
    printf("%d, %f\n", 1, priorityQueue->root->left->key);

    extract_min(priorityQueue);
    printf("%d, %f\n", 0, priorityQueue->root->key);

    Node* end = extract_min(priorityQueue);
    printf("%d, %f\n", 0, end->key);
    return 1;
}







