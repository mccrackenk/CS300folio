//============================================================================
// Name        : LinkedList.cpp
// Author      : Kaleb McCracken
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

// Forward declarations
double strToDouble(string str, char ch);

// Define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// Linked-List class definition
class LinkedList {

private:
    // Internal structure for list entries
    struct Node {
        Bid bid;
        Node* next;

        // Default constructor
        Node() {
            next = nullptr;
        }

        // Initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size; // Variable to keep track of the size of the list

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

// Default constructor
LinkedList::LinkedList() {
    // Initialize housekeeping variables
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Destructor
LinkedList::~LinkedList() {
    Node* current = head;
    Node* temp;

    // Loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current;
        current = current->next;
        delete temp;
    }
}

// Append a new bid to the end of the list
void LinkedList::Append(Bid bid) {
    Node* newNode = new Node(bid);

    if (head == nullptr) {
        // If the list is empty, new node becomes both head and tail
        head = newNode;
        tail = newNode;
    } else {
        // Make current tail node point to the new node
        tail->next = newNode;
        // Update tail to be the new node
        tail = newNode;
    }

    // Increase size count
    size++;
}

// Prepend a new bid to the start of the list
void LinkedList::Prepend(Bid bid) {
    Node* newNode = new Node(bid);

    if (head != nullptr) {
        // If there is already something at the head, new node points to current head as its next node
        newNode->next = head;
    }

    // Head now becomes the new node
    head = newNode;

    // Increase size count
    size++;
}

// Simple output of all bids in the list
void LinkedList::PrintList() {
    Node* current = head;

    // Loop over each node and output bid information
    while (current != nullptr) {
        displayBid(current->bid);
        current = current->next; // Move to the next node
    }
}

// Remove a specified bid
void LinkedList::Remove(string bidId) {
    Node* current = head;

    // Special case if matching node is the head
    if (head != nullptr && head->bid.bidId == bidId) {
        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
        return;
    }

    // Start at the head and loop over each node looking for a match
    while (current != nullptr && current->next != nullptr) {
        // If the next node bidID is equal to the current bidID
        if (current->next->bid.bidId == bidId) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            size--;
            return;
        }

        current = current->next; // Move to the next node
    }
}

// Search for the specified bidId
Bid LinkedList::Search(string bidId) {
    Node* current = head;

    // Start at the head of the list
    while (current != nullptr) {
        // If the current node bidID matches, return it
        if (current->bid.bidId == bidId) {
            return current->bid;
        }

        current = current->next; // Move to the next node
    }

    // Return an empty bid if not found
    Bid emptyBid;
    return emptyBid;
}

// Returns the current size (number of elements) in the list
int LinkedList::Size() {
    return size;
}

// Static methods used for testing
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

// Load a CSV file containing bids into a LinkedList
void loadBids(string csvPath, LinkedList* list) {
    cout << "Loading CSV file " << csvPath << endl;

    // Initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // Loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {
            // Initialize a bid using data from the current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // Add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
}

// Simple C function to convert a string to a double
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

// The one and only main() method
int main(int argc, char* argv[]) {
    // Process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);
            break;

        case 2:
            ticks = clock();
            loadBids(csvPath, &bidList);
            cout << bidList.Size() << " bids read" << endl;
            ticks = clock() - ticks;
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 3:
            bidList.PrintList();
            break;

        case 4:
            ticks = clock();
            bid = bidList.Search(bidKey);
            ticks = clock() - ticks;

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 5:
            bidList.Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
