#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;

// Structure to hold student record
struct StudentRecord {
    string semester;
    string university;
    string department;
    string subject;
    int score;
};

// Function to convert string to lowercase
string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Function to read CSV file
vector<StudentRecord> readCSV(string filename) {
    vector<StudentRecord> records;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "File not found!" << endl;
        return records;
    }
    
    string line;
    // Skip header
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> parts;
        
        while (getline(ss, token, ',')) {
            parts.push_back(token);
        }
        
        if (parts.size() == 5) {
            StudentRecord record;
            record.semester = parts[0];
            record.university = parts[1];
            record.department = parts[2];
            record.subject = parts[3];
            record.score = stoi(parts[4]);
            records.push_back(record);
        }
    }
    
    file.close();
    return records;
}

// Function to print a record
void printRecord(const StudentRecord& r) {
    cout << r.semester << " | " << r.university << " | " << r.department << " | " << r.subject << " | " << r.score << endl;
}

// Slice operation
void sliceOperation(const vector<StudentRecord>& data, string field, string value) {
    cout << "\nSlice: " << field << " = " << value << endl;
    
    for (const auto& r : data) {
        string fieldValue;
        if (field == "Semester") fieldValue = r.semester;
        else if (field == "University") fieldValue = r.university;
        else if (field == "Department") fieldValue = r.department;
        else if (field == "Subject") fieldValue = r.subject;
        
        if (toLower(fieldValue) == toLower(value)) {
            printRecord(r);
        }
    }
}

// Dice operation
void diceOperation(const vector<StudentRecord>& data, map<string, string> filters) {
    cout << "\nDice" << endl;
    
    for (const auto& r : data) {
        bool match = true;
        for (const auto& filter : filters) {
            string fieldValue;
            if (filter.first == "Semester") fieldValue = r.semester;
            else if (filter.first == "University") fieldValue = r.university;
            else if (filter.first == "Department") fieldValue = r.department;
            else if (filter.first == "Subject") fieldValue = r.subject;
            
            if (toLower(fieldValue) != toLower(filter.second)) {
                match = false;
                break;
            }
        }
        
        if (match) {
            printRecord(r);
        }
    }
}

// Roll-up operation
void rollupOperation(const vector<StudentRecord>& data, string groupField) {
    cout << "\nRoll up: (Total Score by " << groupField << ")" << endl;
    
    map<string, int> totals;
    
    for (const auto& r : data) {
        string key;
        if (groupField == "Semester") key = r.semester;
        else if (groupField == "University") key = r.university;
        else if (groupField == "Department") key = r.department;
        else if (groupField == "Subject") key = r.subject;
        
        totals[key] += r.score;
    }
    
    for (const auto& pair : totals) {
        cout << pair.first << " -> " << pair.second << endl;
    }
}

// Drill-down operation
void drilldownOperation(const vector<StudentRecord>& data) {
    cout << "\nDrill-down" << endl;
    cout << "Select a level to drill down from:" << endl;
    cout << "1. Semester totals" << endl;
    cout << "2. University totals" << endl;
    cout << "3. Department totals" << endl;
    
    string choice;
    cout << "Enter your choice (1-3): ";
    cin >> choice;
    
    if (choice == "1") {
        // Show semester totals first, then drill down
        cout << "\nSemester Totals:" << endl;
        map<string, int> semesterTotals;
        for (const auto& r : data) {
            semesterTotals[r.semester] += r.score;
        }
        for (const auto& pair : semesterTotals) {
            cout << pair.first << " -> " << pair.second << endl;
        }
        
        cout << "\nEnter semester to drill down (e.g., 2023-1): ";
        string selectedSemester;
        cin >> selectedSemester;
        
        cout << "\nDetailed data for " << selectedSemester << ":" << endl;
        for (const auto& r : data) {
            if (r.semester == selectedSemester) {
                printRecord(r);
            }
        }
    }
    else if (choice == "2") {
        // Show university totals first, then drill down
        cout << "\nUniversity Totals:" << endl;
        map<string, int> universityTotals;
        for (const auto& r : data) {
            universityTotals[r.university] += r.score;
        }
        for (const auto& pair : universityTotals) {
            cout << pair.first << " -> " << pair.second << endl;
        }
        
        cout << "\nEnter university to drill down: ";
        string selectedUniversity;
        cin.ignore();
        getline(cin, selectedUniversity);
        
        cout << "\nDetailed data for " << selectedUniversity << ":" << endl;
        for (const auto& r : data) {
            if (toLower(r.university) == toLower(selectedUniversity)) {
                printRecord(r);
            }
        }
    }
    else if (choice == "3") {
        // Show department totals first, then drill down
        cout << "\nDepartment Totals:" << endl;
        map<string, int> departmentTotals;
        for (const auto& r : data) {
            departmentTotals[r.department] += r.score;
        }
        for (const auto& pair : departmentTotals) {
            cout << pair.first << " -> " << pair.second << endl;
        }
        
        cout << "\nEnter department to drill down: ";
        string selectedDepartment;
        cin.ignore();
        getline(cin, selectedDepartment);
        
        cout << "\nDetailed data for " << selectedDepartment << ":" << endl;
        for (const auto& r : data) {
            if (toLower(r.department) == toLower(selectedDepartment)) {
                printRecord(r);
            }
        }
    }
    else {
        cout << "Invalid choice!" << endl;
    }
}

// Pivot operation
void pivotOperation(const vector<StudentRecord>& data, string rowField, string colField) {
    cout << "\nPivot: " << rowField << " vs " << colField << endl;
    
    map<string, map<string, int>> pivot;
    set<string> colValues;
    
    for (const auto& r : data) {
        string row, col;
        
        if (rowField == "Semester") row = r.semester;
        else if (rowField == "University") row = r.university;
        else if (rowField == "Department") row = r.department;
        else if (rowField == "Subject") row = r.subject;
        
        if (colField == "Semester") col = r.semester;
        else if (colField == "University") col = r.university;
        else if (colField == "Department") col = r.department;
        else if (colField == "Subject") col = r.subject;
        
        colValues.insert(col);
        pivot[row][col] += r.score;
    }
    
    // Print header
    cout << rowField;
    for (const auto& col : colValues) {
        cout << "\t" << col;
    }
    cout << endl;
    
    // Print data
    for (const auto& rowPair : pivot) {
        cout << rowPair.first;
        for (const auto& col : colValues) {
            auto it = rowPair.second.find(col);
            cout << "\t" << (it != rowPair.second.end() ? it->second : 0);
        }
        cout << endl;
    }
}

int main() {
    string filename;
    cout << "Enter CSV file name: ";
    cin >> filename;
    
    vector<StudentRecord> data = readCSV(filename);
    
    if (data.empty()) {
        cout << "No data loaded" << endl;
        return 0;
    }
    
    while (true) {
        cout << "\n* OLAP Menu *" << endl;
        cout << "1. Original Data" << endl;
        cout << "2. Slice" << endl;
        cout << "3. Dice" << endl;
        cout << "4. Roll-Up" << endl;
        cout << "5. Drill-Down" << endl;
        cout << "6. Pivot" << endl;
        cout << "0. Exit" << endl;
        
        string choice;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (choice == "1") {
            cout << "\nOriginal Data" << endl;
            for (const auto& r : data) {
                printRecord(r);
            }
        }
        else if (choice == "2") {
            string field, value;
            cout << "Enter field to slice by (Semester/University/Department/Subject): ";
            cin >> field;
            cout << "Enter value for " << field << ": ";
            cin.ignore();
            getline(cin, value);
            sliceOperation(data, field, value);
        }
        else if (choice == "3") {
            int numFilters;
            cout << "How many filters? ";
            cin >> numFilters;
            
            map<string, string> filters;
            for (int i = 0; i < numFilters; i++) {
                string field, value;
                cout << "Enter field name (Semester/University/Department/Subject): ";
                cin >> field;
                cout << "Enter value for " << field << ": ";
                cin.ignore();
                getline(cin, value);
                filters[field] = value;
            }
            diceOperation(data, filters);
        }
        else if (choice == "4") {
            string field;
            cout << "Enter field to group by (Semester/University/Department/Subject): ";
            cin >> field;
            rollupOperation(data, field);
        }
        else if (choice == "5") {
            drilldownOperation(data);
        }
        else if (choice == "6") {
            string rowField, colField;
            cout << "Enter row field: ";
            cin >> rowField;
            cout << "Enter column field: ";
            cin >> colField;
            pivotOperation(data, rowField, colField);
        }
        else if (choice == "0") {
            cout << "Exiting..." << endl;
            break;
        }
        else {
            cout << "Invalid choice" << endl;
        }
    }
    
    return 0;
}
