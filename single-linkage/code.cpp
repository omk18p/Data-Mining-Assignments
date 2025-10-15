#include <bits/stdc++.h>
using namespace std;

struct Node {
    vector<double> values;
};

// Calculate Euclidean distance
double getDistance(const Node &a, const Node &b) {
    double total = 0;
    for (size_t i = 0; i < a.values.size(); i++)
        total += pow(a.values[i] - b.values[i], 2);
    return sqrt(total);
}

// Find minimum (single linkage) distance between two clusters
double findMinDist(const vector<int> &A, const vector<int> &B, const vector<vector<double>> &dist) {
    double minVal = DBL_MAX;
    for (int i : A)
        for (int j : B)
            minVal = min(minVal, dist[i][j]);
    return minVal;
}

// Display distance matrix between clusters
void showDistanceTable(const vector<vector<int>> &clusters, const vector<vector<double>> &dist) {
    cout << "\nDistance Matrix between clusters:\n";

    // Create cluster labels like p1|p2
    vector<string> labels;
    for (const auto &c : clusters) {
        string s = "";
        for (size_t i = 0; i < c.size(); i++) {
            s += "p" + to_string(c[i] + 1);
            if (i < c.size() - 1) s += "|";
        }
        labels.push_back(s);
    }

    int w = 12;
    cout << setw(w) << " ";
    for (auto &lbl : labels) cout << setw(w) << lbl;
    cout << "\n";

    for (size_t i = 0; i < clusters.size(); i++) {
        cout << setw(w) << labels[i];
        for (size_t j = 0; j < clusters.size(); j++) {
            if (i == j) cout << setw(w) << "-";
            else cout << setw(w) << fixed << setprecision(2)
                      << findMinDist(clusters[i], clusters[j], dist);
        }
        cout << "\n";
    }
}

// Print all clusters
void showClusters(const vector<vector<int>> &clusters) {
    int cNo = 0;
    for (auto &c : clusters) {
        cout << "Cluster " << cNo++ << ": ";
        for (int i : c) cout << "p" << i + 1 << " ";
        cout << "\n";
    }
}

int main() {
    ifstream fin("data.csv");
    if (!fin.is_open()) {
        cerr << "Error: Could not open data.csv\n";
        return 1;
    }

    vector<Node> points;
    string line;
    getline(fin, line); // skip header

    while (getline(fin, line)) {
        stringstream ss(line);
        string cell;
        vector<double> vals;
        while (getline(ss, cell, ',')) {
            if (!cell.empty())
                vals.push_back(stod(cell));
        }
        points.push_back({vals});
    }
    fin.close();

    int n = points.size();
    vector<vector<double>> distMat(n, vector<double>(n, 0.0));

    // Fill distance matrix
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            distMat[i][j] = distMat[j][i] = getDistance(points[i], points[j]);

    // Initially, each point is its own cluster
    vector<vector<int>> clusters;
    for (int i = 0; i < n; i++) clusters.push_back({i});

    int step = 1;
    while (clusters.size() > 1) {
        cout << "\n=============== Step " << step++ << " ===============\n";
        showDistanceTable(clusters, distMat);

        // Find two nearest clusters
        double minD = DBL_MAX;
        int a = -1, b = -1;
        for (size_t i = 0; i < clusters.size(); i++)
            for (size_t j = i + 1; j < clusters.size(); j++) {
                double d = findMinDist(clusters[i], clusters[j], distMat);
                if (d < minD) {
                    minD = d;
                    a = i;
                    b = j;
                }
            }

        // Merge a and b
        vector<int> merged = clusters[a];
        merged.insert(merged.end(), clusters[b].begin(), clusters[b].end());

        auto label = [&](const vector<int> &c) {
            string s = "";
            for (size_t i = 0; i < c.size(); i++) {
                s += "p" + to_string(c[i] + 1);
                if (i < c.size() - 1) s += "|";
            }
            return s;
        };

        cout << "Merging Cluster (" << label(clusters[a]) << ") and ("
             << label(clusters[b]) << ") at distance = " << fixed << setprecision(2) << minD << "\n";

        // Rebuild cluster list
        vector<vector<int>> newC;
        for (size_t i = 0; i < clusters.size(); i++)
            if (i != a && i != b) newC.push_back(clusters[i]);
        newC.push_back(merged);
        clusters = newC;

        showClusters(clusters);
    }

    cout << "\nFinal cluster:\n";
    showClusters(clusters);

    return 0;
}
