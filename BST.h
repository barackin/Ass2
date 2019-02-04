/**

 * Binary Search Tree - Template

 *

 * Store values in a BST

 * Can use Inorder, Preorder and Postorder to traverse tree

 * Can use Add/Remove to modify tree

 * Rebalance creates a balanced tree

 *

 * @author Bazen Nega

 * @date 2 - 3 - 2019

 */



#ifndef BST_HPP

#define BST_HPP


#include <iostream>
#include <climits>
#include <sstream>
#include <string>

using namespace std;


template <class T>

class BST {


private:

    // Node for BST
    typedef struct node {

        T data;

        struct node *leftPtr;

        struct node *rightPtr;

    } Node;

    // visits elements in traversal for checking equality of structure
    class TreeVisitor {

    public:

        TreeVisitor() = delete;

        static stringstream SS;
        static stringstream SS2;

        static string GetSS() {

            return SS.str();
        }

        static string GetSS2() {

            return SS2.str();
        }

        static void ResetSS() {

            SS.str(string());
        }

        static void ResetSS2() {

            SS2.str(string());
        }

        static void visitor(const T &item) {

            SS << item;
        }

        static void visitor2(const T &item) {

            SS2 << item;
        }
    };

    // root of the tree
    Node *rootPtr{nullptr};

    // pointer for aiding in removing elements from tree
    Node* tempPtr;

    // number of nodes in tree
    int nodeCount;

    // array to hold sorted tree elements to aid in balancing
    T *array;

    // counter for "array"
    int arrayCount;

    // Additional private functions

    // Make a new BST Node
    Node *makeNode(const T &value) const {

        Node *newNode = new Node();

        newNode->data = value;
        newNode->leftPtr = nullptr;
        newNode->rightPtr = nullptr;

        return newNode;
    }

    // does algorithm work for clearing the tree. recursively does postorder
    // traversal to remove items
    void clearHelper(Node* rootPtr) {

        if(rootPtr->leftPtr != nullptr) {
            clearHelper(rootPtr->leftPtr);
        }
        if(rootPtr->rightPtr != nullptr) {
            clearHelper(rootPtr->rightPtr);
        }
        Remove(rootPtr->data);
    }

    // does algorithm work for adding element to the tree
    bool addHelper(Node *&rootPtr, const T &item) {

        if(rootPtr == nullptr) {

            rootPtr = new Node();
            rootPtr->data = item;
            nodeCount++;
            return true;
        }
        if(item == rootPtr->data) {

            return false;
        }

        if(item < rootPtr->data) {

            if(rootPtr->leftPtr != nullptr) {

                addHelper(rootPtr->leftPtr, item);
            }
            else {

                rootPtr->leftPtr = new Node();
                rootPtr->leftPtr->data = item;
                nodeCount++;
                return true;
            }
        }

        if(item > rootPtr->data) {

            if(rootPtr->rightPtr != nullptr) {

                addHelper(rootPtr->rightPtr, item);
            }
            else {
                rootPtr->rightPtr = new Node();
                rootPtr->rightPtr->data = item;
                nodeCount++;
                return true;
            }
        }
    }

    // helper function for removeValue, in aiding the removal of an element from
    // the tree
    int findMinNode(Node *rootPtr) {

        if(rootPtr == nullptr) {

            return INT_MAX;
        }
        if(rootPtr->leftPtr != nullptr) {

            return findMinNode(rootPtr->leftPtr);
        }
        return rootPtr->data;
    }

    // does the algorithm work of removing an element from the tree
    Node *removeValue(Node *rootPtr, const T &item) {

        if(rootPtr == nullptr) {

            return nullptr;
        }
        if(item < rootPtr->data) {

            rootPtr->leftPtr = removeValue(rootPtr->leftPtr, item);
        }
        else if(item > rootPtr->data) {

            rootPtr->rightPtr = removeValue(rootPtr->rightPtr, item);
        }
        else {

            if(rootPtr->leftPtr == nullptr && rootPtr->rightPtr == nullptr) {

                delete(rootPtr);
                rootPtr = nullptr;
                nodeCount--;
            }

            else if(rootPtr->leftPtr == nullptr) {

                tempPtr = rootPtr;
                rootPtr = rootPtr->rightPtr;
                delete tempPtr;
                nodeCount--;
            }

            else if(rootPtr->rightPtr == nullptr) {

                tempPtr = rootPtr;
                rootPtr = rootPtr->leftPtr;
                delete tempPtr;
                nodeCount--;
            }

            else {

                int tempInt = findMinNode(rootPtr->rightPtr);
                rootPtr->data = tempInt;
                rootPtr->rightPtr = removeValue(rootPtr->rightPtr, tempInt);
            }

            return rootPtr;
        }
    }

    // refactored function for traversing through tree via flag ("order"),
    // that tells whether to do inorder(I), preorder(P), or postorder(O))
    void traversalHelper(Node *rootPtr, void visit(const T &item), char order) const {

        if(rootPtr == nullptr) {
            return;
        }
        if(order == 'I') {

            traversalHelper(rootPtr->leftPtr, visit, 'I');
            visit(rootPtr->data);
            traversalHelper(rootPtr->rightPtr, visit, 'I');
        }
        if(order == 'P') {

            visit(rootPtr->data);
            traversalHelper(rootPtr->leftPtr, visit, 'P');
            traversalHelper(rootPtr->rightPtr, visit, 'P');
        }
        if(order == 'O') {

            traversalHelper(rootPtr->leftPtr, visit, 'O');
            traversalHelper(rootPtr->rightPtr, visit, 'O');
            visit(rootPtr->data);
        }
    }

    // does the algorithm work for "Contains"
    bool containsHelper(Node *rootPtr, const T &item) const {

        if(rootPtr == nullptr) {

            return false;
        }

        if(rootPtr->data == item) {
            return true;
        }

        if(item > rootPtr->data) {
            if (rootPtr->rightPtr != nullptr) {
                return containsHelper(rootPtr->rightPtr, item);
            }
        }
        else if(item < rootPtr->data) {
            if (rootPtr->leftPtr != nullptr) {
                return containsHelper(rootPtr->leftPtr, item);
            }
        }
        return false;
    }

    // does the algorithm work for getHeight
    int maxHeight(Node* rootPtr) const {

        if(rootPtr == nullptr) {
            return 0;
        }

        return 1 + max(maxHeight(rootPtr->leftPtr), maxHeight(rootPtr->rightPtr));
    }

    // fills and sorts the field array with tree elements
    void rebalanceHelper(Node* rootPtr) {

        if(rootPtr->leftPtr != nullptr) {
            rebalanceHelper(rootPtr->leftPtr);
        }
        array[arrayCount] = rootPtr->data;
        arrayCount++;
        if(rootPtr->rightPtr != nullptr) {
            rebalanceHelper(rootPtr->rightPtr);
        }
    }

    // helps in balancing the tree after a sorted array is given
    Node* bstBalancer(T *array, int start, int end) {

        if(start > end) {

            return nullptr;
        }

        int mid = (start + end) / 2;

        Node *root = makeNode(array[mid]);

        root->leftPtr = bstBalancer(array, start, mid - 1);

        root->rightPtr = bstBalancer(array, mid + 1, end);

        return root;
    }

public:

    // constructor, empty tree

    BST() {

        rootPtr = nullptr;
        nodeCount = 0;
        array = nullptr;
        arrayCount = 0;
        tempPtr = nullptr;
    }

// constructor, tree with root

    explicit BST(const T &rootItem) {

        rootPtr = new Node();
        rootPtr->data = rootItem;
        nodeCount = 1;
        array = new T[1];
        arrayCount = 0;
        tempPtr = nullptr;
    }

// given an array of length n

// create a tree to have all items in that array

// with the minimum height (i.e. rebalance)

    BST(const T arr[], int n) {

        array = nullptr;
        arrayCount = 0;
        tempPtr = nullptr;
        for(int i = 0; i < n; i++) {

            Add(arr[i]);
        }

        nodeCount = n;

        Rebalance();
    }

// copy constructor

    explicit BST(const BST<T> &bst) {

        rootPtr = bst.rootPtr;
        nodeCount = bst.NumberOfNodes();
        tempPtr = bst.tempPtr;
        array = bst.array;
        arrayCount = bst.arrayCount;
    }

// destructor

    ~BST() {

        Clear();
        delete array;
        delete tempPtr;
    }

// Additional private functions

// true if no nodes in BST

    bool IsEmpty() const {

        return rootPtr == nullptr && nodeCount == 0;
    }

// 0 if empty, 1 if only root, otherwise

// height of root is max height of subtrees + 1

    int getHeight() const {

        return maxHeight(rootPtr);
    }

// number of nodes in BST

    int NumberOfNodes() const {

        return nodeCount;
    }

// add a new item, return true if successful

    bool Add(const T &item) {

        return addHelper(rootPtr, item);
    }

// remove item, return true if successful

    bool Remove(const T &item) {

        int temp = item;
        if(Contains(item)) {

            rootPtr = removeValue(rootPtr, item);

            if(!Contains(temp)) {

                return true;
            }
            return false;
        }
        return false;
    }

// true if item is in BST

    bool Contains(const T &item) const {

        return containsHelper(rootPtr, item);
    }

// inorder traversal: left-root-right

// takes a function that takes a single parameter of type T

    void InorderTraverse(void visit(const T &item)) const {

        traversalHelper(rootPtr, visit, 'I');
    }

// preorder traversal: root-left-right

    void PreorderTraverse(void visit(const T &item)) const {

        traversalHelper(rootPtr, visit, 'P');
    }

// postorder traversal: left-right-root

    void PostorderTraverse(void visit(const T &item)) const {

        traversalHelper(rootPtr, visit, 'O');
    }

// create dynamic array, copy all the items to the array

// and then read the array to re-create this tree from scratch

// so that resulting tree should be balanced

    void Rebalance() {

        delete array;
        arrayCount = 0;
        array = new T[NumberOfNodes()];
        int tempArrayLen = NumberOfNodes();

        rebalanceHelper(rootPtr);

        Clear();

        rootPtr = bstBalancer(array, 0, tempArrayLen - 1);

        nodeCount = tempArrayLen;
    }

// delete all nodes in tree

    void Clear() {

        if(rootPtr != nullptr) {
            clearHelper(rootPtr);
        }
    }


// trees are equal if they have the same structure

// AND the same item values at all the nodes

    bool operator==(const BST<T> &other) const {

        TreeVisitor::ResetSS();
        TreeVisitor::ResetSS2();

        PostorderTraverse(TreeVisitor::visitor);

        other.PostorderTraverse(TreeVisitor::visitor2);

        return TreeVisitor::GetSS() == TreeVisitor::GetSS2();
    }

// not == to each other

    bool operator!=(const BST<T> &other) const {

        return !operator==;
    }

};

#endif  // BST_HPP
