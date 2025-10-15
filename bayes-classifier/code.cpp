#include <bits/stdc++.h>
using namespace std;

struct Point {
    vector<double> features;
    int label;
};

vector<Point> data;

// Mean of a vector
double mean(const vector<double> &v) {
    double s = 0;
    for(double x : v) s += x;
    return s / v.size();
}

// Variance of a vector
double variance(const vector<double> &v, double m) {
    double s = 0;
    for(double x : v) s += (x - m) * (x - m);
    return s / v.size();
}

// Gaussian probability density function
double gaussian(double x, double m, double var) {
    if(var == 0) var = 1e-6; // avoid divide by zero
    return (1.0 / sqrt(2*M_PI*var)) * exp(-(x - m)*(x - m)/(2*var));
}

int main() {
    // Read dataset
    ifstream fin("data.csv");
    if(!fin) { cerr << "Cannot open data.csv\n"; return 1; }
    
    string line;
    getline(fin, line); // skip header
    while(getline(fin, line)) {
        stringstream ss(line);
        string x; vector<double> f;
        while(getline(ss,x,',')) f.push_back(stod(x));
        int label = (int)f.back();
        f.pop_back();
        data.push_back({f, label});
    }

    // Identify unique classes
    set<int> classes;
    for(auto &p : data) classes.insert(p.label);

    int nFeatures = data[0].features.size();

    // Compute prior probabilities
    map<int,double> prior;
    map<int, vector<double>> means;
    map<int, vector<double>> variances;

    for(int c : classes) {
        vector<Point> clsPoints;
        for(auto &p : data) if(p.label==c) clsPoints.push_back(p);

        prior[c] = (double)clsPoints.size() / data.size();

        vector<double> m(nFeatures,0), v(nFeatures,0);
        for(int j=0;j<nFeatures;j++){
            vector<double> feat;
            for(auto &p : clsPoints) feat.push_back(p.features[j]);
            m[j] = mean(feat);
            v[j] = variance(feat, m[j]);
        }
        means[c] = m;
        variances[c] = v;
    }

    // Predict each point
    cout << fixed << setprecision(4);
    cout << "Point\tPredicted\tActual\n";
    for(size_t i=0;i<data.size();i++){
        int bestClass = -1;
        double bestProb = -1;

        for(int c : classes){
            double prob = log(prior[c]);
            for(int j=0;j<nFeatures;j++)
                prob += log(gaussian(data[i].features[j], means[c][j], variances[c][j]));
            if(bestClass==-1 || prob>bestProb){
                bestClass = c;
                bestProb = prob;
            }
        }

        cout << "p" << i+1 << "\t" << bestClass << "\t\t" << data[i].label << "\n";
    }

    return 0;
}
