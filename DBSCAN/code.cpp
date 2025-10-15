#include <bits/stdc++.h>
using namespace std;

struct Point { vector<double> coords; };
vector<Point> data;
vector<int> labels;

// Euclidean distance between two points
double euclid(const Point &a, const Point &b) {
    double sum = 0;
    for(size_t i = 0; i < a.coords.size(); i++)
        sum += (a.coords[i] - b.coords[i]) * (a.coords[i] - b.coords[i]);
    return sqrt(sum);
}

// Find neighbors within eps
vector<int> regionQuery(int idx, double eps) {
    vector<int> neighbors;
    for(size_t j=0; j<data.size(); j++) {
        if(euclid(data[idx], data[j]) <= eps)
            neighbors.push_back(j);
    }
    return neighbors;
}

// Expand cluster from a point
void expandCluster(int idx, int clusterId, double eps, int minPts) {
    queue<int> q;
    q.push(idx);
    while(!q.empty()) {
        int cur = q.front(); q.pop();
        vector<int> neighbors = regionQuery(cur, eps);
        for(int n : neighbors) {
            if(labels[n] == -2) { // unvisited
                labels[n] = clusterId;
                vector<int> nbs = regionQuery(n, eps);
                if(nbs.size() >= (size_t)minPts)
                    q.push(n);
            } else if(labels[n] == -1) { // previously noise
                labels[n] = clusterId;
            }
        }
    }
}

int main() {
    // Read dataset
    ifstream fin("data.csv");
    if(!fin){ cerr << "Cannot open data.csv\n"; return 1; }
    
    string line; 
    getline(fin,line); // skip header
    while(getline(fin,line)) {
        stringstream ss(line);
        string x; vector<double> c;
        while(getline(ss,x,',')) {
            x.erase(remove(x.begin(), x.end(), ' '), x.end());
            if(!x.empty()) c.push_back(stod(x));
        }
        data.push_back({c});
    }

    double eps; int minPts;
    cout << "Enter eps: "; cin >> eps;
    cout << "Enter minPts: "; cin >> minPts;

    labels.assign(data.size(), -2); // -2 = unvisited
    int clusterId = 0;

    for(size_t i=0;i<data.size();i++) {
        if(labels[i]!=-2) continue;
        vector<int> neighbors = regionQuery(i, eps);
        if(neighbors.size() < (size_t)minPts) {
            labels[i] = -1; // noise
        } else {
            labels[i] = clusterId;
            expandCluster(i, clusterId, eps, minPts);
            clusterId++;
        }
    }

    // Save results to CSV
    ofstream fout("dbscan_out.csv");
    fout << "X,Y,cluster\n";
    for(size_t i=0;i<data.size();i++){
        for(double x : data[i].coords) fout << x << ",";
        fout << labels[i] << "\n";
    }
    fout.close();

    // Print clusters summary
    map<int, vector<int>> clustersMap;
    int noise = 0;
    for(size_t i=0;i<labels.size();i++) {
        if(labels[i]==-1) noise++;
        else clustersMap[labels[i]].push_back(i+1);
    }

    cout << "\n=== DBSCAN Clustering Summary ===\n";
    for(map<int, vector<int>>::iterator it = clustersMap.begin(); it != clustersMap.end(); ++it){
        cout << "Cluster " << it->first << ": ";
        for(size_t j=0;j<it->second.size();j++) cout << "p" << it->second[j] << " ";
        cout << "\n";
    }
    cout << "Noise points: " << noise << "\n";

    return 0;
}
