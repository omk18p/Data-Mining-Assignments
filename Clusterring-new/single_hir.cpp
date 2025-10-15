#include <bits/stdc++.h>
using namespace std;

struct Point { vector<double> coords; };
vector<Point> data;

double euclid(const Point &a, const Point &b) {
    double sum = 0;
    for (size_t i = 0; i < a.coords.size(); i++)
        sum += (a.coords[i] - b.coords[i]) * (a.coords[i] - b.coords[i]);
    return sqrt(sum);
}

// Compute minimum distance between two clusters (single linkage)
double clusterDistance(const vector<int> &c1, const vector<int> &c2) {
    double minDist = numeric_limits<double>::max();
    for (int i : c1) {
        for (int j : c2) {
            double d = euclid(data[i], data[j]);
            if (d < minDist) minDist = d;
        }
    }
    return minDist;
}

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
    if(!fin) { cerr << "Cannot open data.csv\n"; return 1; }

    string line; 
    getline(fin,line); // skip header
    while(getline(fin,line)){
        stringstream ss(line);
        string x; vector<double> coords;
        while(getline(ss,x,',')){
            x.erase(remove(x.begin(), x.end(), ' '), x.end());
            if(!x.empty()) coords.push_back(stod(x));
        }
        data.push_back({coords});
    }

    // Initialize each point as its own cluster
    vector<vector<int>> clusters;
    for (size_t i = 0; i < data.size(); i++) clusters.push_back({(int)i});

    int step = 1;
    while (clusters.size() > 1) {
        // Find the two closest clusters
        double minDist = numeric_limits<double>::max();
        int idx1 = -1, idx2 = -1;
        for (size_t i = 0; i < clusters.size(); i++) {
            for (size_t j = i + 1; j < clusters.size(); j++) {
                double d = clusterDistance(clusters[i], clusters[j]);
                if (d < minDist) {
                    minDist = d;
                    idx1 = i; idx2 = j;
                }
            }
        }

        // Merge clusters
        vector<int> merged = clusters[idx1];
        merged.insert(merged.end(), clusters[idx2].begin(), clusters[idx2].end());

        vector<vector<int>> newClusters;
        for (size_t i = 0; i < clusters.size(); i++) {
            if (i != (size_t)idx1 && i != (size_t)idx2) newClusters.push_back(clusters[i]);
        }
        newClusters.push_back(merged);
        clusters = newClusters;

        cout << "Step " << step << " merged distance: " << minDist << "\n";
        printClusters(clusters);
        cout << "\n";
        step++;
    }

    cout << "Final cluster:\n";
    printClusters(clusters);

    // Save clusters to CSV
    ofstream fout("hier_single_out.csv");
    fout << "Point,Cluster\n";
    for (size_t c = 0; c < clusters.size(); c++) {
        for (int idx : clusters[c])
            fout << "p" << (idx+1) << "," << c << "\n";
    }
    fout.close();

    return 0;
}