#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

// ---------- Normalization Functions ----------

// Range Normalization (Min-Max)
void normalizeRange(vector<double>& values, double low, double high) {
    double mn = *min_element(values.begin(), values.end());
    double mx = *max_element(values.begin(), values.end());
    for (size_t i = 0; i < values.size(); i++) {
        if (mx == mn) values[i] = (low + high) / 2.0;
        else values[i] = ((values[i] - mn) / (mx - mn)) * (high - low) + low;
    }
}

// Standardization (Z-Score)
void normalizeStandard(vector<double>& values) {
    double sum = 0.0;
    for (double v : values) sum += v;
    double mean = sum / values.size();

    double sq_sum = 0.0;
    for (double v : values) sq_sum += (v - mean) * (v - mean);
    double sd = sqrt(sq_sum / values.size());

    for (size_t i = 0; i < values.size(); i++) {
        if (sd == 0) values[i] = 0.0;
        else values[i] = (values[i] - mean) / sd;
    }
}

// Decimal Scaling
void normalizeDecimal(vector<double>& values) {
    double maxAbs = 0;
    for (double v : values) maxAbs = max(maxAbs, fabs(v));

    int j = 0;
    while (maxAbs >= 1) {
        maxAbs /= 10;
        j++;
    }
    for (size_t i = 0; i < values.size(); i++) {
        values[i] = values[i] / pow(10, j);
    }
}

// ---------- CSV Writing ----------
void saveCSV(const string& file,
             const vector<double>& a,
             const vector<double>& b,
             const vector<double>& c,
             const vector<double>& d,
             const vector<string>& label) {
    ofstream out(file);
    out << "SepalLength,SepalWidth,PetalLength,PetalWidth,Class\n";
    for (size_t i = 0; i < a.size(); i++) {
        out << a[i] << "," << b[i] << "," << c[i] << "," << d[i] << "," << label[i] << "\n";
    }
    out.close();
}

// ---------- Main ----------
int main() {
    ifstream fin("expt1.csv");
    if (!fin.is_open()) {
        cout << "Error: expt1.csv not found!\n";
        return 1;
    }

    string row;
    getline(fin, row); // Skip header
    vector<double> sl, sw, pl, pw;
    vector<string> labels;

    // Read dataset
    while (getline(fin, row)) {
        stringstream ss(row);
        string val;
        vector<string> parts;
        while (getline(ss, val, ',')) parts.push_back(val);

        if (parts.size() == 5) {
            sl.push_back(stod(parts[0]));
            sw.push_back(stod(parts[1]));
            pl.push_back(stod(parts[2]));
            pw.push_back(stod(parts[3]));
            labels.push_back(parts[4]);
        }
    }
    fin.close();

    if (sl.empty()) {
        cout << "Dataset is empty!\n";
        return 1;
    }

    // ---- Min-Max ----
    double low, high;
    cout << "Enter Min value: ";
    cin >> low;
    cout << "Enter Max value: ";
    cin >> high;

    vector<double> sl_mm = sl, sw_mm = sw, pl_mm = pl, pw_mm = pw;
    normalizeRange(sl_mm, low, high);
    normalizeRange(sw_mm, low, high);
    normalizeRange(pl_mm, low, high);
    normalizeRange(pw_mm, low, high);
    saveCSV("iris_minmax.csv", sl_mm, sw_mm, pl_mm, pw_mm, labels);

    // ---- Z-Score ----
    vector<double> sl_z = sl, sw_z = sw, pl_z = pl, pw_z = pw;
    normalizeStandard(sl_z);
    normalizeStandard(sw_z);
    normalizeStandard(pl_z);
    normalizeStandard(pw_z);
    saveCSV("iris_zscore.csv", sl_z, sw_z, pl_z, pw_z, labels);

    // ---- Decimal Scaling ----
    vector<double> sl_d = sl, sw_d = sw, pl_d = pl, pw_d = pw;
    normalizeDecimal(sl_d);
    normalizeDecimal(sw_d);
    normalizeDecimal(pl_d);
    normalizeDecimal(pw_d);
    saveCSV("iris_decimal.csv", sl_d, sw_d, pl_d, pw_d, labels);

    cout << "Normalization done. Files generated:\n";
    cout << " - iris_minmax.csv\n";
    cout << " - iris_zscore.csv\n";
    cout << " - iris_decimal.csv\n";

    return 0;
}
