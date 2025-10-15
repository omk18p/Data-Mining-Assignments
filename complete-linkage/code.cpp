#include <bits/stdc++.h>
using namespace std;

// ====== STRUCTURE ======
struct Point { vector<double> coords; };
vector<Point> data;

// ====== EUCLIDEAN DISTANCE ======
double euclid(const Point &a, const Point &b) {
    double sum = 0;
    for (size_t i = 0; i < a.coords.size(); i++)
        sum += pow(a.coords[i] - b.coords[i], 2);
    return sqrt(sum);
}

// ====== COMPLETE LINKAGE DISTANCE ======
double clusterDistance(const vector<int> &c1, const vector<int> &c2, const vector<vector<double>> &distMat) {
    double maxDist = 0.0;
    for (int i : c1)
        for (int j : c2)
            maxDist = max(maxDist, distMat[i][j]);
    return maxDist;
}

// ====== PRINT DISTANCE MATRIX ======
void printDistanceMatrix(const vector<vector<int>> &clusters, const vector<vector<double>> &distMat) {
    cout << "\nDistance Matrix between clusters:\n";

    // Prepare cluster names
    vector<string> clusterNames;
    for (const auto &c : clusters) {
        string name;
        for (size_t i = 0; i < c.size(); i++) {
            name += "p" + to_string(c[i] + 1);
            if (i < c.size() - 1) name += "|";
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
            else cout << setw(width) << fixed << setprecision(2)
                      << clusterDistance(clusters[i], clusters[j], distMat);
        }
        cout << "\n";
    }
}

// ====== PRINT CLUSTERS ======
void printClusters(const vector<vector<int>> &clusters) {
    for (size_t i = 0; i < clusters.size(); i++) {
        cout << "Cluster " << i << ": ";
        for (int idx : clusters[i])
            cout << "p" << idx + 1 << " ";
        cout << "\n";
    }
}

// ====== MAIN FUNCTION ======
int main() {
    ifstream fin("data.csv");
    if (!fin) { cerr << "Cannot open data.csv\n"; return 1; }

    string line;
    getline(fin, line); // Skip header
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

    // Distance matrix between points
    vector<vector<double>> distMat(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            distMat[i][j] = distMat[j][i] = euclid(data[i], data[j]);

    // Start with each point as its own cluster
    vector<vector<int>> clusters;
    for (int i = 0; i < n; i++) clusters.push_back({i});

    int step = 1;
    while (clusters.size() > 1) {
        cout << "=============== Step " << step << " ===============\n";
        printDistanceMatrix(clusters, distMat);

        // Find two closest clusters
        double minDist = numeric_limits<double>::max();
        int idx1 = -1, idx2 = -1;
        for (size_t i = 0; i < clusters.size(); i++) {
            for (size_t j = i + 1; j < clusters.size(); j++) {
                double d = clusterDistance(clusters[i], clusters[j], distMat);
                if (d < minDist) {
                    minDist = d;
                    idx1 = i; idx2 = j;
                }
            }
        }

        // Merge
        vector<int> merged = clusters[idx1];
        merged.insert(merged.end(), clusters[idx2].begin(), clusters[idx2].end());

        auto getName = [](const vector<int> &c) {
            string name;
            for (size_t i = 0; i < c.size(); i++) {
                name += "p" + to_string(c[i] + 1);
                if (i < c.size() - 1) name += "|";
            }
            return name;
        };

        cout << "Merging Cluster (" << getName(clusters[idx1]) << ") and ("
             << getName(clusters[idx2]) << ") at distance = "
             << fixed << setprecision(2) << minDist << "\n";

        // Update cluster list
        vector<vector<int>> newClusters;
        for (size_t i = 0; i < clusters.size(); i++)
            if (i != (size_t)idx1 && i != (size_t)idx2) newClusters.push_back(clusters[i]);
        newClusters.push_back(merged);
        clusters = newClusters;

        printClusters(clusters);
        step++;
    }

    cout << "\nFinal cluster:\n";
    printClusters(clusters);
    return 0;
}
