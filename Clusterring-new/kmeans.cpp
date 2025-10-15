#include <bits/stdc++.h>
using namespace std;

// ======== DATA STRUCTURES ========
struct Point {
    vector<double> coords;
};

vector<Point> data;
vector<Point> centroids;
vector<int> labels;

// ======== HELPER FUNCTIONS ========
double euclideanDist(const Point& a, const Point& b) {
    double sum = 0;
    for (size_t i = 0; i < a.coords.size(); i++)
        sum += (a.coords[i] - b.coords[i]) * (a.coords[i] - b.coords[i]);
    return sqrt(sum);
}

Point meanPoint(const vector<Point>& pts) {
    size_t dim = pts[0].coords.size();
    Point res;
    res.coords.resize(dim, 0.0);
    for (auto& p : pts)
        for (size_t i = 0; i < dim; i++)
            res.coords[i] += p.coords[i];
    for (double &v : res.coords)
        v /= pts.size();
    return res;
}

void printDistanceMatrix(const vector<Point>& pts) {
    int n = pts.size();
    cout << "\nDistance Matrix:\n";
    cout << fixed << setprecision(2);
    cout << "       ";
    for (int i = 0; i < n; i++) cout << "p" << i << "     ";
    cout << "\n";
    for (int i = 0; i < n; i++) {
        cout << "p" << i << "  ";
        for (int j = 0; j < n; j++) {
            if (i == j) cout << "0.0000 ";
            else if (i > j) cout << euclideanDist(pts[i], pts[j]) << " ";
            else cout << "       ";
        }
        cout << "\n";
    }
}

// ======== MAIN FUNCTION ========
int main() {
    ifstream fin("data.csv");
    if (!fin.is_open()) { cerr << "Cannot open dataset\n"; return 1; }

    string line;
    getline(fin, line); // skip header
    while (getline(fin, line)) {
        stringstream ss(line);
        string x;
        vector<double> coords;
        while (getline(ss, x, ',')) coords.push_back(stod(x));
        data.push_back({coords});
    }

    int k;
    cout << "Enter number of clusters: ";
    cin >> k;
    if (k < 1) k = 1;
    if (k > data.size()) k = data.size();

    srand(time(nullptr));
    set<int> chosen;
    while (centroids.size() < k) {
        int idx = rand() % data.size();
        if (chosen.count(idx)) continue;
        centroids.push_back(data[idx]);
        chosen.insert(idx);
    }

    cout << "\nInitial distance matrix:\n";
    printDistanceMatrix(data);

    bool changed = true;
    int iter = 0;
    while (changed && iter < 1000) {
        changed = false;
        labels.assign(data.size(), -1);
        vector<vector<Point>> clusters(k);

        for (size_t i = 0; i < data.size(); i++) {
            double minDist = 1e18;
            int minIdx = -1;
            for (int j = 0; j < k; j++) {
                double d = euclideanDist(data[i], centroids[j]);
                if (d < minDist) { minDist = d; minIdx = j; }
            }
            labels[i] = minIdx;
            clusters[minIdx].push_back(data[i]);
        }

        for (int j = 0; j < k; j++) {
            if (!clusters[j].empty()) {
                Point newC = meanPoint(clusters[j]);
                if (!changed) {
                    for (size_t d = 0; d < newC.coords.size(); d++)
                        if (abs(newC.coords[d] - centroids[j].coords[d]) > 1e-6)
                            changed = true;
                }
                centroids[j] = newC;
            }
        }

        cout << "\nDistance matrix at iteration " << iter << ":\n";
        printDistanceMatrix(data);

        iter++;
    }

    cout << "\nFinal cluster centroids:\n";
    for (int i = 0; i < k; i++) {
        cout << "C" << i << ": ";
        for (double x : centroids[i].coords) cout << x << " ";
        cout << "\n";
    }

    ofstream fout("kmeans_out.csv");
    fout << "X,Y,cluster\n";
    for (size_t i = 0; i < data.size(); i++) {
        for (double x : data[i].coords) fout << x << ",";
        fout << labels[i] << "\n";
    }
    fout.close();

    cout << "\nCluster sizes:\n";
    vector<int> counts(k,0);
    for (int l : labels) counts[l]++;
    for (int i = 0; i < k; i++) cout << "Cluster " << i << ": " << counts[i] << " points\n";

    return 0;
}
