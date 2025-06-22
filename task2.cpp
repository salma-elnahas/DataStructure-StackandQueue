#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
 

using namespace std;

// stack class using linked list
class Stack {
    struct Node { // node structure for stack
        string data;
        Node* next;
        Node(string val) : data(val), next(nullptr) {}
    };
    Node* top; // stack top 
public:
    Stack() { top = NULL; } 
    ~Stack() { clear(); }    

    // push a new item onto stack
    void push(string newitem) {
        Node* newNode = new Node(newitem);
        newNode->next = top;
        top = newNode;
    }

    bool isEmpty() {
        return top == NULL;
    }

    // pop an item from stack
    string pop() {
        if (isEmpty()) {
            cout << "stack empty on pop";
            return "";
        } else {
            Node* temp = top;
            string data = temp->data;
            top = top->next;
            delete temp;
            return data;
        }
    }

    // get the top item without popping
    string getTop() {
        if (isEmpty()) {
            cout << "stack empty";
            return "";
        } else {
            return top->data;
        }
    }

    // clear all items from stack
    void clear() {
        while (!isEmpty()) pop();
    }
};

int main() {
    Stack backwardStack, forwardStack; // stacks for back and forward navigation
    vector<string> urls;               // vector to store urls
    vector<string> operations;         // store user operations (Backward or Forward)
    int currentIndex;                  // current index of url page

    // ordinal names for output  
    string ordinals[] = {"First", "Second", "Third", "Fourth", "Fifth",
                         "Sixth", "Seventh", "Eighth", "Ninth", "Tenth"};


    // read from "URLs.txt" and store them into urls vectors
    ifstream file("URLs.txt");
    if (!file.is_open()) {
        cout << "failed to open URLs.txt" << endl;
        return -1;  
    }

    int n;
    file >> n;
    file.ignore();  //ignore newline

    // Read URLs from the file
    for (int i = 0; i < n; ++i) {
        string url;
        getline(file, url);
        urls.push_back(url);
    }

    // Read operations (backward or forward) from file
    string opLine;
    getline(file, opLine);
    stringstream str(opLine);
    string op;
    while (str >> op) {
        operations.push_back(op);
    }

    // Set the current index to the last page visited
    currentIndex = urls.size() - 1;  // Initialize to the last URL

    // Print the last page as the current one
    cout << "The current page is: " << urls[currentIndex] << endl;

    for (size_t i = 0; i < operations.size(); ++i) {
        cout << ordinals[i] << " Choice [" << operations[i] << "]" << endl;

        if (operations[i] == "Backward") {
            // if user wants to go backward
            if (currentIndex > 0) {
                forwardStack.push(urls[currentIndex]); // push current page into forward stack
                currentIndex--; // move to previous URL
            } else {
                cout << "cannot go backward" << endl;
            }
        } else if (operations[i] == "Forward") {
            // if user wants to go forward
            if (!forwardStack.isEmpty()) {
                backwardStack.push(urls[currentIndex]); // save current in backward stack
                currentIndex++; // move to next URL
                forwardStack.pop(); // remove page from forward stack
            } else {
                cout << "cannot go forward" << endl;
            }
        }

        // Print the current page
        cout << "The current page is: " << urls[currentIndex] << endl;

        // print the next page if it exists
        if (currentIndex + 1 < urls.size()) {
            cout << "The Next one is: " << urls[currentIndex + 1] << endl;
        } else {
            cout << "The Next one is: None" << endl;
        }

        // print the previous page if it exists
        if (currentIndex - 1 >= 0) {
            cout << "The Previous one is: " << urls[currentIndex - 1] << endl;
        } else {
            cout << "The Previous one is: None" << endl;
        }
    }

    return 0;
}
