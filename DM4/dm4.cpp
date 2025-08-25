#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <numeric>
#include <fstream>
#include <sstream>

using namespace std;

// Structure to hold the five-number summary
struct FiveNumberSummary {
    double Q1;
    double median;
    double Q3;
    double IQR;
    double lowerWhisker;
    double upperWhisker;
    vector<double> outliers;
};

// Structure to hold data entries
struct DataEntry {
    string category;
    string name;
    double value;
    string description;
};

// Function to calculate median
double median(vector<double>& data) {
    int n = data.size();
    if (n % 2 == 0) {
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    } else {
        return data[n/2];
    }
}

// Function to calculate five-number summary with outliers
FiveNumberSummary fiveNumberSummaryWithOutliers(vector<double>& data) {
    vector<double> sortedData = data;
    sort(sortedData.begin(), sortedData.end());
    
    int n = sortedData.size();
    double Q2 = median(sortedData);
    
    vector<double> lowerHalf, upperHalf;
    
    if (n % 2 == 0) {
        lowerHalf = vector<double>(sortedData.begin(), sortedData.begin() + n/2);
        upperHalf = vector<double>(sortedData.begin() + n/2, sortedData.end());
    } else {
        lowerHalf = vector<double>(sortedData.begin(), sortedData.begin() + n/2);
        upperHalf = vector<double>(sortedData.begin() + n/2 + 1, sortedData.end());
    }
    
    double Q1 = median(lowerHalf);
    double Q3 = median(upperHalf);
    double IQR = Q3 - Q1;
    
    double lowerBound = Q1 - 1.5 * IQR;
    double upperBound = Q3 + 1.5 * IQR;
    
    // Find whiskers
    double lowerWhisker = sortedData[0];
    double upperWhisker = sortedData[n-1];
    
    for (double value : sortedData) {
        if (value >= lowerBound) {
            lowerWhisker = value;
            break;
        }
    }
    
    for (int i = n-1; i >= 0; i--) {
        if (sortedData[i] <= upperBound) {
            upperWhisker = sortedData[i];
            break;
        }
    }
    
    // Find outliers
    vector<double> outliers;
    for (double value : sortedData) {
        if (value < lowerBound || value > upperBound) {
            outliers.push_back(value);
        }
    }
    
    return {Q1, Q2, Q3, IQR, lowerWhisker, upperWhisker, outliers};
}

// Function to print simple summary (like the image)
void printSimpleSummary(const string& title, const FiveNumberSummary& summary) {
    cout << "\n" << title << " Summary:" << endl;
    cout << "Q1: " << fixed << setprecision(1) << summary.Q1 << endl;
    cout << "Median (Q2): " << summary.median << endl;
    cout << "Q3: " << summary.Q3 << endl;
    cout << "IQR: " << summary.IQR << endl;
    cout << "Lower Whisker: " << summary.lowerWhisker << endl;
    cout << "Upper Whisker: " << summary.upperWhisker << endl;
}

// Function to read CSV file
vector<DataEntry> readCSV(const string& filename) {
    vector<DataEntry> data;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return data;
    }
    
    string line;
    // Skip header
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string category, name, value, description;
        
        getline(ss, category, ',');
        getline(ss, name, ',');
        getline(ss, value, ',');
        getline(ss, description, ',');
        
        data.push_back({category, name, stod(value), description});
    }
    
    file.close();
    return data;
}

// Function to extract data by category
vector<double> extractDataByCategory(const vector<DataEntry>& data, const string& category) {
    vector<double> values;
    for (const auto& entry : data) {
        if (entry.category == category) {
            values.push_back(entry.value);
        }
    }
    return values;
}

int main() {
    cout << "Five-Number Summary Analysis" << endl;
    
    // Read data from CSV file
    vector<DataEntry> allData = readCSV("indian_data.csv");
    
    if (allData.empty()) {
        cout << "Error: No data found in CSV file." << endl;
        return 1;
    }
    
    // Extract data by categories
    vector<double> cricketAverages = extractDataByCategory(allData, "Cricket");
    vector<double> gdpPerCapita = extractDataByCategory(allData, "GDP");
    vector<double> literacyRates = extractDataByCategory(allData, "Literacy");
    
    // Calculate and display summaries
    FiveNumberSummary cricketSummary = fiveNumberSummaryWithOutliers(cricketAverages);
    FiveNumberSummary gdpSummary = fiveNumberSummaryWithOutliers(gdpPerCapita);
    FiveNumberSummary literacySummary = fiveNumberSummaryWithOutliers(literacyRates);
    
    printSimpleSummary("Cricket Batting Averages", cricketSummary);
    printSimpleSummary("GDP Per Capita", gdpSummary);
    printSimpleSummary("Literacy Rates", literacySummary);
    
    return 0;
}
