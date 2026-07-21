#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

// Returns true when quantity is 0 or greater
bool isValidQuantity(int quantity) {
    return quantity >= 0;
}

// Returns true when price is 0 or greater
bool isValidPrice(double price) {
    return price >= 0;
}

// Returns quantity multiplied by price, returns 0.0 if quantity or price is invalid
double calculateItemValue(const InventoryItem& item) {
    if (!isValidQuantity(item.quantity) || !isValidPrice(item.price)) {
        return 0.0;
    }

    return (item.quantity * item.price);
}

// Returns the number of records stored
// Reads records in the format: sku name quantity price
int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
    ifstream inputFile(filename);

    if (!inputFile) {
        return 0;
    }

    int count = 0;
    string currentSku, currentName;
    int currentQuantity;
    double currentPrice;

    while (count < maxItems && inputFile >> currentSku >> currentName >> currentQuantity >> currentPrice) {
        items[count].sku = currentSku;
        items[count].name = currentName;
        items[count].quantity = currentQuantity;
        items[count].price = currentPrice;

        count++;
    }

    inputFile.close();
    return count;
}

// Writes each item and its total value and the total inventory value
// Returns true if the report was written successfully
bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
    ofstream outputFile(filename);

    if (!outputFile) {
        return false;
    }

    double totalValue = 0.0;

    for (int i = 0; i < count; i++) {
        double itemValue = calculateItemValue(items[i]);
        totalValue += itemValue;

        outputFile << items[i].sku << " "
                   << items[i].name << " "
                   << items[i].quantity << " "
                   << items[i].price << " "
                   << itemValue << "\n";
    }

    outputFile << "Total inventory value: " << totalValue << "\n";

    outputFile.close();
    return true;
}

// // Returns 0.0 for null arrays or invalid counts, otherwise returns the sum of all item values
double calculateTotalInventoryValue(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0) {
        return 0.0;
    }

    double totalValue = 0.0;

    for (int i = 0; i < count; i++) {
        double itemValue = calculateItemValue(items[i]);
        totalValue += itemValue;
    }

    return totalValue;
}

// Returns the first index for a matching SKU
// Returns -1 for null arrays or invalid counts or if no matching SKU was found
int findItemBySku(const InventoryItem items[], int count, string sku) {
    if (items == nullptr || count <= 0) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (items[i].sku == sku) {
            return i;
        }
    }

    return -1;
}

// Returns the index of the item with the highest item value
// Returns -1 for null arrays or invalid counts
int findHighestValueItemIndex(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0) {
        return -1;
    }

    double highest = calculateItemValue(items[0]);
    int highestIndex = 0;

    for (int i = 1; i < count; i++) {
        if (calculateItemValue(items[i]) > highest) {
            highest = calculateItemValue(items[i]);
            highestIndex = i;
        }
    }

    return highestIndex;
}
