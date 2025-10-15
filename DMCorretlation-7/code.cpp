#include <bits/stdc++.h>
using namespace std;

// Function to split a string by delimiter
vector<string> split(const string &line, char delim) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to calculate Pearson correlation coefficient between two vectors
double pearsonCorrelation(const vector<double> &x, const vector<double> &y) {
    int n = x.size();
    double x_mean = accumulate(x.begin(), x.end(), 0.0) / n;
    double y_mean = accumulate(y.begin(), y.end(), 0.0) / n;

    double numerator = 0.0;
    double denominator_x = 0.0;
    double denominator_y = 0.0;

    for (int i = 0; i < n; i++) {
        double dx = x[i] - x_mean;
        double dy = y[i] - y_mean;
        numerator += dx * dy;
        denominator_x += dx * dx;
        denominator_y += dy * dy;
    }

    double denominator = sqrt(denominator_x * denominator_y);
    return (denominator != 0) ? numerator / denominator : numeric_limits<double>::quiet_NaN();
}

int main() {
    string filename = "data.csv";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    vector<string> headers;
    vector<vector<double>> data;

    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> tokens = split(line, ',');
        if (firstLine) {
            headers = tokens;  // store column names
            firstLine = false;
        } else {
            vector<double> row;
            for (int i = 1; i < tokens.size(); i++) {  // skip TID column
                row.push_back(stod(tokens[i]));
            }
            data.push_back(row);
        }
    }
    file.close();

    int numItems = headers.size() - 1;  // excluding TID

    // Transpose data for easier column access
    vector<vector<double>> columns(numItems, vector<double>(data.size()));
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < numItems; j++)
            columns[j][i] = data[i][j];

    // Compute Pearson correlation for all pairs
    cout << fixed << setprecision(4);
    cout << "Pearson Correlation between items:\n";
    for (int i = 0; i < numItems; i++) {
        for (int j = i + 1; j < numItems; j++) {
            double r = pearsonCorrelation(columns[i], columns[j]);
            cout << headers[i + 1] << " & " << headers[j + 1] << ": " << r << endl;
        }
    }

    return 0;
}
