// Assignment_1_INFR2820.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Data {
public:
    void load(const string& filename);
    void insert(); // Various code functions
    void update();
    void deleteData();
    void search();
    void sort();    
    void convert();
    void printData() { // Prints the Array
        cout << "Updated Products:" << "\n";
        for (const auto& row : dataProducts) {
            for (const string& value : row) {
                cout << value << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    void printRow(const vector<string>& row) {
        for (const string& value : row) {
            cout << value << " ";
        }
        cout << "\n\n";
    }
    void swapRows(vector<string>& row1, vector<string>& row2) {
        vector<string> temp = row1;
        row1 = row2;
        row2 = temp;
    }

    // Function to convert a specific column to floats. 
    // Limits decimal points to 2 because it was coming up with random float points
    void convertColumnToFloat(vector<vector<string>>& dataProducts, size_t columnIndex) {
        for (auto& row : dataProducts) {
            istringstream iss(row[columnIndex]);
            float floatValue;
            if (iss >> floatValue) {
                ostringstream oss;
                oss << fixed << setprecision(2) << floatValue; // Limit to two decimal places
                row[columnIndex] = oss.str();
            }
            else {
                cerr << "Conversion failed for column " << columnIndex << "\n";
                return;
            }
        }
    }

    // Function to compare two rows based on a specific column
    bool compareRows(const vector<string>& row1, const vector<string>& row2, size_t columnIndex) {
        return stof(row1[columnIndex]) < stof(row2[columnIndex]);
    }

    // Function to perform bubble sort on the 2D array based on a specific column
    void sortByColumn(vector<vector<string>>& dataProducts, size_t columnIndex) {
        bool swapped;
        size_t n = dataProducts.size();
        do {
            swapped = false;
            for (size_t i = 1; i < n; ++i) {
                if (compareRows(dataProducts[i - 1], dataProducts[i], columnIndex)) {
                    swapRows(dataProducts[i - 1], dataProducts[i]);
                    swapped = true;
                }
            }
        } while (swapped);
    }
    Data();
private:
    vector<vector<string>> dataProducts;

    vector<string> splitByComma(const string& str) {
        vector<string> tokens;
        istringstream iss(str);
        string token;
        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

Data::Data() {}

void Data::load(const string& filename) {
    ifstream file(filename); // Loads data from file
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> row = splitByComma(line);
            dataProducts.push_back(row);
        }
        file.close();
    }
    else { cerr << "Unable to open file: " << filename << "\n"; }

    for (const auto& row : dataProducts) {
        for (string value : row) { cout << value << " "; }
        cout << "\n";
    }

    cout << "\n";
}

void Data::convert() {
    size_t rows = dataProducts.size();
    size_t cols = dataProducts[0].size();
    if (rows == 0) {
        cerr << "Error: Input vector array is empty." << "\n";
        return;
    }

    // Allocate memory for 2D array
    string** array2D = new string* [rows];
    for (int i = 0; i < rows; ++i) {
        array2D[i] = new string[cols];
        for (int j = 0; j < cols; ++j) { array2D[i][j] = dataProducts[i][j]; }
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) { dataProducts[i][j] = array2D[i][j]; }
    }

    // Deallocate memory for the 2D array
    for (int i = 0; i < rows; ++i) { delete[] array2D[i]; }
    delete[] array2D;
}

void Data::insert() {
    // Ask the user for the new row as a comma-separated string
    string newRowString;
    cout << "Enter the new row (comma-separated values): ";
    cin >> newRowString;
    getline(cin, newRowString);

    // Split the input string into individual values
    vector<string> newRow;
    istringstream iss(newRowString);
    string value;
    while (getline(iss, value, ',')) {
        newRow.push_back(value);
    }

    // Insert the new row into the dataProducts
    dataProducts.push_back(newRow);

    printData();
}

void Data::update() {
    string identifier;
    cout << "Enter the Product ID to find: ";
    cin >> identifier;

    // Ask the user for the value to update
    string oldValue, newValue;
    cout << "Enter the value to update: ";
    cin >> oldValue;
    cout << "Enter the new value: ";
    cin >> newValue;

    // Ask the user for the index of the string to update in the same row
    size_t indexToUpdate;
    cout << "Enter the index of the data to update in the same row: 0(ID), 1(Name), 2(Price), 3(Category): ";
    cin >> indexToUpdate;

    // Iterate over the array to find the id string and update the value
    bool found = false;
    for (auto& row : dataProducts) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (row[i] == identifier && identifier == oldValue) {
                row[i] = newValue; // Update the identifier
                if (indexToUpdate < row.size()) { row[indexToUpdate] = newValue; } // Update the specific string in the same row}
                else {
                    cerr << "Index not found." << "\n";
                    return;
                }
                found = true;
                break; // No need to continue searching
            }
            else if (row[i] == identifier) {
                if (indexToUpdate < row.size()) {
                    row[indexToUpdate] = newValue; // Update the specific string in the same row
                }
                else {
                    cerr << "Invalid index." << "\n";
                    return;
                }
                found = true;
                break; // No need to continue searching
            }
        }
        if (found) { break; }// No need to continue searching rows
    }

    if (!found) {
        cerr << "Product ID not found." << "\n";
        return;
    }
    printData();
}

void Data::deleteData() {
    string identifier;
    cout << "Enter the Product ID to find: "; // find ID and match
    cin >> identifier;

    // Iterate over the array to find the identifier string
    bool found = false;
    auto it = dataProducts.begin(); // Iterator for rows
    while (it != dataProducts.end()) {
        bool deleteRow = false;
        for (const string& value : *it) {
            if (value == identifier) {
                found = true;
                // Ask the user if they want to delete this row
                char choice;
                cout << "ID found. Do you want to delete this row? (y/n): ";
                cin >> choice;
                if (choice == 'y' || choice == 'Y') { deleteRow = true; }
                break; // No need to check other values in this row
            }
        }
        if (deleteRow) { it = dataProducts.erase(it); }// Erase the row and move iterator to the next row
        else { ++it; }// Move to the next row
    }

    if (!found) {
        cerr << "Identifier string not found in dataProducts." << "\n";
        return;
    }
    printData();
}

void Data::search() {
    string identifier;
    cout << "Enter the Product ID to find: "; // find ID and match
    cin >> identifier;

    // Iterate over the array to find the identifier string
    bool found = false;
    for (const auto& row : dataProducts) {
        for (const string& value : row) {
            if (value == identifier) {
                found = true;
                cout << "Row with identifier found:" << "\n";
                printRow(row);
                break; // No need to check other values in this row
            }
        }
        if (found) { break; } // No need to continue searching rows
    }
    if (!found) {
        cerr << "Identifier string not found in dataProducts." << "\n";
        return;
    }
}

void Data::sort() {
    size_t columnIndex = 2; // We are sorting price which is array column index 2

    if (columnIndex >= dataProducts[0].size()) {
        cerr << "Invalid column index." << "\n";
        return;
    }

    // Convert the specified column to floats
    convertColumnToFloat(dataProducts, columnIndex);

    // Sort the dataProducts by the specified column (as floats)
    sortByColumn(dataProducts, columnIndex);

    printData();
}

int main() {
    Data data1;
    data1.load("product_data.txt");
    data1.convert();

    int choice; // Choice network
    do {
        cout << "0 - Quit\n";
        cout << "1 - Insert new Data\n";
        cout << "2 - Update Data\n";
        cout << "3 - Delete Data\n";
        cout << "4 - Search for Data\n";
        cout << "5 - Sort Data by Price\n\n";
        cout << "Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            cout << "Exiting.\n";
            break;
        case 1:
            data1.insert();
            break;
        case 2:
            data1.update();
            break;
        case 3:
            data1.deleteData();
            break;
        case 4:
            data1.search();
            break;
        case 5:
            data1.sort();
            break;
        default:
            cout << "\n" << choice << " is not a valid choice.\n";
        }
    } while (choice != 0);
    return 0;
}
