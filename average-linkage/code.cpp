#include <bits/stdc++.h>
using namespace std;

struct Point { vector<double> coords; };
vector<Point> data;

// Euclidean distance between two points
double euclid(const Point &a, const Point &b) {
    double sum = 0;
    for (size_t i = 0; i < a.coords.size(); i++)
        sum += pow(a.coords[i] - b.coords[i], 2);
    return sqrt(sum);
}

// Compute average distance between two clusters (average linkage)
double clusterDistance(const vector<int> &c1, const vector<int> &c2, const vector<vector<double>> &distMat) {
    double total = 0.0;
    int count = 0;
    for (int i : c1)
        for (int j : c2)
            total += distMat[i][j], count++;
    return total / count;
}

// Print distance matrix between clusters
void printDistanceMatrix(const vector<vector<int>> &clusters, const vector<vector<double>> &distMat) {
    cout << "\nDistance Matrix between clusters:\n";

    vector<string> clusterNames;
    for (const auto &c : clusters) {
        string name;
        for (size_t i = 0; i < c.size(); i++) {
            name += "p" + to_string(c[i]+1);
            if (i < c.size()-1) name += "|";
        }
        clusterNames.push_back(name);
    }

    const int width = 12;
    cout << setw(width) << " ";
    for (const auto &name : clusterNames)
        cout << setw(width) << name;
    cout << "\n";

    for (size_t i = 0; i < clusters.size(); i++) {
        cout << setw(width) << clusterNames[i];
        for (size_t j = 0; j < clusters.size(); j++) {
            if (i == j) cout << setw(width) << "-";
            else {
                double d = clusterDistance(clusters[i], clusters[j], distMat);
                cout << setw(width) << fixed << setprecision(2) << d;
            }
        }
        cout << "\n";
    }
}

// Print current clusters
void printClusters(const vector<vector<int>> &clusters) {
    int idx = 0;
    for (const auto &c : clusters) {
        cout << "Cluster " << idx << ": ";
        for (int i : c) cout << "p" << (i+1) << " ";
        cout << "\n";
        idx++;
    }
}

int main() {
    ifstream fin("data.csv");
    if (!fin) { cerr << "Cannot open data.csv\n"; return 1; }

    string line;
    getline(fin, line); // skip header
    while (getline(fin, line)) {
        stringstream ss(line);
        string x;
        vector<double> coords;
        while (getline(ss, x, ',')) {
            x.erase(remove(x.begin(), x.end(), ' '), x.end());
            if (!x.empty()) coords.push_back(stod(x));
        }
        data.push_back({coords});
    }

    int n = data.size();
    vector<vector<double>> distMat(n, vector<double>(n, 0.0));

    // Calculate Euclidean distance matrix
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++)
            distMat[i][j] = distMat[j][i] = euclid(data[i], data[j]);

    vector<vector<int>> clusters;
    for (int i = 0; i < n; i++) clusters.push_back({i});

    int step = 1;
    while (clusters.size() > 1) {
        cout << "================ Step " << step << " ================\n";
        printDistanceMatrix(clusters, distMat);

        double minDist = numeric_limits<double>::max();
        int idx1 = -1, idx2 = -1;
        for (size_t i = 0; i < clusters.size(); i++) {
            for (size_t j = i+1; j < clusters.size(); j++) {
                double d = clusterDistance(clusters[i], clusters[j], distMat);
                if (d < minDist) {
                    minDist = d;
                    idx1 = i; idx2 = j;
                }
            }
        }

        auto getClusterName = [](const vector<int>& c) {
            string name;
            for (size_t i = 0; i < c.size(); i++) {
                name += "p" + to_string(c[i] + 1);
                if (i < c.size() - 1) name += "|";
            }
            return name;
        };

        cout << "Merging Cluster (" << getClusterName(clusters[idx1]) << ") and Cluster (" 
             << getClusterName(clusters[idx2]) << ") with average distance: " 
             << fixed << setprecision(2) << minDist << "\n";

        // Merge clusters
        vector<int> merged = clusters[idx1];
        merged.insert(merged.end(), clusters[idx2].begin(), clusters[idx2].end());

        vector<vector<int>> newClusters;
        for (size_t i = 0; i < clusters.size(); i++)
            if (i != (size_t)idx1 && i != (size_t)idx2)
                newClusters.push_back(clusters[i]);
        newClusters.push_back(merged);

        clusters = newClusters;

        printClusters(clusters);
        step++;
    }

    cout << "\nFinal cluster:\n";
    printClusters(clusters);

    return 0;
}
