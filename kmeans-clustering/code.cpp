#include <bits/stdc++.h>
using namespace std;

struct Point {
    vector<double> coords;
};

vector<Point> data;
vector<Point> centroids;
vector<int> labels;

double euclideanDist(const Point& a, const Point& b) {
    double sum = 0;
    for (size_t i = 0; i < a.coords.size(); i++)
        sum += pow(a.coords[i] - b.coords[i], 2);
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

int main() {
    ifstream fin("data.csv");
    if (!fin.is_open()) {
        cerr << "Cannot open data.csv\n";
        return 1;
    }

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
    while (centroids.size() < (size_t)k) {
        int idx = rand() % data.size();
        if (chosen.count(idx)) continue;
        centroids.push_back(data[idx]);
        chosen.insert(idx);
    }

    bool changed = true;
    int iter = 0;

    cout << fixed << setprecision(4);

    while (changed && iter < 100) {
        changed = false;
        labels.assign(data.size(), -1);
        vector<vector<Point>> clusters(k);

        cout << "\n=== Iteration " << iter + 1 << " ===\n";
        cout << left << setw(8) << "Point";
        for (int j = 0; j < k; j++) cout << setw(15) << ("Dist_to_C" + to_string(j));
        cout << setw(10) << "Assigned" << "\n";
        cout << string(8 + 15 * k + 10, '-') << "\n";

        for (size_t i = 0; i < data.size(); i++) {
            double minDist = 1e18;
            int minIdx = -1;
            vector<double> dists;

            for (int j = 0; j < k; j++) {
                double d = euclideanDist(data[i], centroids[j]);
                dists.push_back(d);
                if (d < minDist) { minDist = d; minIdx = j; }
            }

            labels[i] = minIdx;
            clusters[minIdx].push_back(data[i]);

            cout << left << setw(8) << ("p" + to_string(i + 1));
            for (double d : dists) cout << setw(15) << d;
            cout << setw(10) << ("C" + to_string(minIdx)) << "\n";
        }

        // Recompute centroids
        for (int j = 0; j < k; j++) {
            if (!clusters[j].empty()) {
                Point newC = meanPoint(clusters[j]);
                for (size_t d = 0; d < newC.coords.size(); d++) {
                    if (abs(newC.coords[d] - centroids[j].coords[d]) > 1e-6)
                        changed = true;
                }
                centroids[j] = newC;
            }
        }

        iter++;
    }

    cout << "\nFinal Cluster Centroids:\n";
    for (int i = 0; i < k; i++) {
        cout << "C" << i << ": ";
        for (double x : centroids[i].coords) cout << x << " ";
        cout << "\n";
    }

    vector<int> counts(k, 0);
    for (int l : labels) counts[l]++;
    cout << "\nCluster sizes:\n";
    for (int i = 0; i < k; i++) cout << "Cluster " << i << ": " << counts[i] << " points\n";

    return 0;
}
