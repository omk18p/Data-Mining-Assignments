#include <bits/stdc++.h>
using namespace std;

struct Point { vector<double> coords; };
vector<Point> data;
vector<int> labels;

double euclid(const Point &a, const Point &b){
    double s = 0;
    for(size_t i=0;i<a.coords.size();i++) s += (a.coords[i]-b.coords[i])*(a.coords[i]-b.coords[i]);
    return sqrt(s);
}

vector<int> regionQuery(const vector<Point>& pts, int idx, double eps){
    vector<int> neighbors;
    for(size_t j=0;j<pts.size();j++){
        if(euclid(pts[idx], pts[j]) <= eps) neighbors.push_back(j);
    }
    return neighbors;
}

void expandCluster(int idx, int clusterId, double eps, int minPts){
    queue<int> q;
    q.push(idx);
    while(!q.empty()){
        int cur = q.front(); q.pop();
        vector<int> neighbors = regionQuery(data, cur, eps);
        for(int n : neighbors){
            if(labels[n] == -2){ // unvisited
                labels[n] = clusterId;
                vector<int> nbs = regionQuery(data, n, eps);
                if(nbs.size() >= (size_t)minPts)
                    q.push(n);
            } else if(labels[n] == -1){ // previously noise
                labels[n] = clusterId;
            }
        }
    }
}

int main(){
    ifstream fin("data.csv");
    if(!fin){ cerr << "Cannot open data.csv\n"; return 1; }

    string line; 
    getline(fin,line); // skip header
    while(getline(fin,line)){
        stringstream ss(line);
        string x; vector<double> c;
        while(getline(ss,x,',')){
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

    for(size_t i=0;i<data.size();i++){
        if(labels[i]!=-2) continue;
        vector<int> neighbors = regionQuery(data,i,eps);
        if(neighbors.size() < (size_t)minPts){
            labels[i]=-1; // noise
        } else {
            labels[i]=clusterId;
            expandCluster(i, clusterId, eps, minPts);
            clusterId++;
        }
    }

    ofstream fout("dbscan_out.csv");
    fout << "X,Y,cluster\n";
    for(size_t i=0;i<data.size();i++){
        for(double x : data[i].coords) fout << x << ",";
        fout << labels[i] << "\n";
    }
    fout.close();

    map<int,int> counts;
    int noise = 0;
    for(int l : labels){
        if(l==-1) noise++;
        else counts[l]++;
    }

    cout << "DBSCAN results:\n";
    for(auto it = counts.begin(); it != counts.end(); ++it)
        cout << "Cluster " << it->first << ": " << it->second << " points\n";
    cout << "Noise: " << noise << " points\n";

    return 0;
}
