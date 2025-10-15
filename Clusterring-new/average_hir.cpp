#include <bits/stdc++.h>
using namespace std;

struct Point {
    vector<double> coords;
};

double euclideanDistance(const Point &a, const Point &b) {
    double sum = 0;
    for (size_t i = 0; i < a.coords.size(); i++)
        sum += (a.coords[i] - b.coords[i]) * (a.coords[i] - b.coords[i]);
    return sqrt(sum);
}

// Compute Average Linkage distance between two clusters
double averageLinkageDistance(const vector<int> &c1, const vector<int> &c2, const vector<Point> &data) {
    double totalDist = 0.0;
    int count = 0;
    for (int i : c1) {
        for (int j : c2) {
            totalDist += euclideanDistance(data[i], data[j]);
            count++;
        }
    }
    return count > 0 ? totalDist / count : 0.0;
}

int main() {
    ifstream fin("data.csv");
    if (!fin.is_open()) {
        cerr << "Cannot open data.csv\n";
        return 1;
    }

    string line;
    getline(fin, line); // skip header
    vector<Point> data;
    while (getline(fin, line)) {
        stringstream ss(line);
        string val;
        vector<double> coords;
        while (getline(ss, val, ',')) coords.push_back(stod(val));
        data.push_back({coords});
    }
    fin.close();

    int n = data.size();
    vector<vector<int>> clusters;
    for (int i = 0; i < n; i++) clusters.push_back({i});

    vector<int> labels(n, -1);
    int step = 1;

    while (clusters.size() > 1) {
        int mergeA = -1, mergeB = -1;
        double minDist = DBL_MAX;

        // Find pair with smallest average linkage distance
        for (size_t i = 0; i < clusters.size(); i++) {
            for (size_t j = i + 1; j < clusters.size(); j++) {
                double d = averageLinkageDistance(clusters[i], clusters[j], data);
                if (d < minDist) {
                    minDist = d;
                    mergeA = i;
                    mergeB = j;
                }
            }
        }

        // Merge clusters
        vector<int> merged = clusters[mergeA];
        merged.insert(merged.end(), clusters[mergeB].begin(), clusters[mergeB].end());

        vector<vector<int>> newClusters;
        for (size_t i = 0; i < clusters.size(); i++) {
            if (i != mergeA && i != mergeB) newClusters.push_back(clusters[i]);
        }
        newClusters.push_back(merged);
        clusters = newClusters;

        // Print step
        cout << "Step " << step << " (Merged distance: " << minDist << "):\n";
        for (size_t i = 0; i < clusters.size(); i++) {
            cout << " Cluster " << i << ": ";
            for (int idx : clusters[i]) cout << "p" << idx + 1 << " ";
            cout << "\n";
        }
        cout << "\n";
        step++;
    }

    // Assign labels
    for (size_t i = 0; i < clusters[0].size(); i++) {
        labels[clusters[0][i]] = 0;
    }

    // Output to CSV
    ofstream fout("hierarchical_average_out.csv");
    fout << "X,Y,cluster\n";
    for (size_t i = 0; i < data.size(); i++) {
        for (double x : data[i].coords) fout << x << ",";
        fout << labels[i] << "\n";
    }
    fout.close();

    cout << "Final cluster contains all points.\n";

    return 0;
}
