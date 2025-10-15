#include <bits/stdc++.h>
using namespace std;

vector<set<string>> transactions;                // Stores all transactions
vector<map<set<string>, int>> frequentItemsets;  // Frequent itemsets by level

// Load transactions from CSV file
void loadDataset(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error reading dataset file!" << endl;
        exit(1);
    }

    string line;
    bool headerSkipped = false;
    while (getline(file, line)) {
        if (!headerSkipped) { headerSkipped = true; continue; } // skip header
        if (line.empty()) continue;

        stringstream ss(line);
        string tid, item;
        getline(ss, tid, ','); // skip TID
        set<string> transaction;

        while (getline(ss, item, ',')) {
            item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
            if (!item.empty())
                transaction.insert(item);
        }

        if (!transaction.empty())
            transactions.push_back(transaction);
    }

    cout << transactions.size() << " transactions loaded successfully.\n";
    file.close();
}

// Count support of an itemset
int countSupport(const set<string> &itemset) {
    int count = 0;
    for (auto &t : transactions)
        if (includes(t.begin(), t.end(), itemset.begin(), itemset.end()))
            count++;
    return count;
}

// Generate frequent itemsets using Apriori
void generateFrequentItemsets(double minSupport) {
    map<set<string>, int> L1;
    map<string, int> itemCount;

    // Count 1-itemsets
    for (auto &t : transactions)
        for (auto &item : t)
            itemCount[item]++;

    int totalTrans = transactions.size();
    for (auto &it : itemCount)
        if ((double)it.second / totalTrans >= minSupport)
            L1[{it.first}] = it.second;

    frequentItemsets.push_back(L1);

    cout << "\nFrequent 1-itemsets:\n";
    for (auto &it : L1)
        cout << "{ " << *it.first.begin() << " } - Support: "
             << fixed << setprecision(2)
             << (double)it.second / totalTrans << endl;

    // Generate k-itemsets
    int k = 2;
    while (true) {
        map<set<string>, int> Lk;
        auto prev = frequentItemsets.back();
        vector<set<string>> prevItemsets;

        for (auto &it : prev)
            prevItemsets.push_back(it.first);

        for (int i = 0; i < prevItemsets.size(); i++) {
            for (int j = i + 1; j < prevItemsets.size(); j++) {
                set<string> candidate = prevItemsets[i];
                candidate.insert(prevItemsets[j].begin(), prevItemsets[j].end());
                if (candidate.size() == k) {
                    int supportCount = countSupport(candidate);
                    if ((double)supportCount / totalTrans >= minSupport)
                        Lk[candidate] = supportCount;
                }
            }
        }

        if (Lk.empty()) break;
        frequentItemsets.push_back(Lk);

        cout << "\nFrequent " << k << "-itemsets:\n";
        for (auto &it : Lk) {
            cout << "{ ";
            for (auto &item : it.first) cout << item << " ";
            cout << "} - Support: "
                 << fixed << setprecision(2)
                 << (double)it.second / totalTrans << endl;
        }

        k++;
    }
}

// Generate association rules
void generateAssociationRules(double minConfidence) {
    cout << "\nAssociation Rules:\n";
    for (auto &level : frequentItemsets) {
        for (auto &it : level) {
            set<string> itemset = it.first;
            if (itemset.size() < 2) continue;

            vector<string> items(itemset.begin(), itemset.end());
            int totalSupport = it.second;

            for (int i = 0; i < items.size(); i++) {
                set<string> A = {items[i]};
                set<string> B = itemset;
                B.erase(items[i]);

                int countA = countSupport(A);
                double confidence = (double)totalSupport / countA;

                if (confidence >= minConfidence) {
                    cout << "{ ";
                    for (auto &x : A) cout << x << " ";
                    cout << "} => { ";
                    for (auto &x : B) cout << x << " ";
                    cout << "}  Confidence: "
                         << fixed << setprecision(2)
                         << confidence << endl;
                }
            }
        }
    }
}

int main() {
    cout << "=== MARKET BASKET ANALYSIS USING APRIORI ALGORITHM ===\n";

    string filename = "dataset.csv";
    loadDataset(filename);

    double minSupport, minConfidence;
    cout << "\nEnter minimum support (0-1): ";
    cin >> minSupport;
    cout << "Enter minimum confidence (0-1): ";
    cin >> minConfidence;

    generateFrequentItemsets(minSupport);
    generateAssociationRules(minConfidence);

    return 0;
}
