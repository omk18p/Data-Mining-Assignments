#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class Student {
public:
    string name;
    double hindi_marks;
    double english_marks;
    
    Student(string n, double h, double e) : name(n), hindi_marks(h), english_marks(e) {}
};

int main() {
    vector<Student> students;
    
    // Read data from CSV file
    ifstream file("students_study.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open students_study.csv" << endl;
        return 1;
    }
    
    string line;
    // Skip header
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string name, hindi_str, english_str;
        
        getline(ss, name, ',');
        getline(ss, hindi_str, ',');
        getline(ss, english_str, ',');
        
        double hindi = stod(hindi_str);
        double english = stod(english_str);
        
        students.push_back(Student(name, hindi, english));
    }
    file.close();
    
    // Calculate totals
    double total_hindi = 0, total_english = 0, total_all = 0;
    for (const auto& s : students) {
        total_hindi += s.hindi_marks;
        total_english += s.english_marks;
        total_all += s.hindi_marks + s.english_marks;
    }
    
    // Write output to CSV
    ofstream outfile("output.csv");
    if (!outfile.is_open()) {
        cout << "Error: Could not create output.csv" << endl;
        return 1;
    }
    
    // Write header
    outfile << "Student,Hindi_Marks,T-Weight(Hindi),D-Weight(Hindi),"
            << "English_Marks,T-Weight(English),D-Weight(English),"
            << "Both,T-Weight(Both),D-Weight(Both)" << endl;
    
    // Process each student
    for (const auto& s : students) {
        double total_student = s.hindi_marks + s.english_marks;
        
        // Calculate T-weights
        double t_weight_hindi = (total_hindi > 0) ? (s.hindi_marks / total_hindi) * 100 : 0;
        double t_weight_english = (total_english > 0) ? (s.english_marks / total_english) * 100 : 0;
        double t_weight_both = (total_all > 0) ? (total_student / total_all) * 100 : 0;
        
        // Calculate D-weights
        double d_weight_hindi = (total_student > 0) ? (s.hindi_marks / total_student) * 100 : 0;
        double d_weight_english = (total_student > 0) ? (s.english_marks / total_student) * 100 : 0;
        double d_weight_both = 100.0;
        
        // Write student data
        outfile << s.name << ","
                << fixed << setprecision(1) << s.hindi_marks << ","
                << fixed << setprecision(2) << t_weight_hindi << ","
                << fixed << setprecision(2) << d_weight_hindi << ","
                << fixed << setprecision(1) << s.english_marks << ","
                << fixed << setprecision(2) << t_weight_english << ","
                << fixed << setprecision(2) << d_weight_english << ","
                << fixed << setprecision(1) << total_student << ","
                << fixed << setprecision(2) << t_weight_both << ","
                << fixed << setprecision(2) << d_weight_both << endl;
    }
    
    // Write totals
    outfile << "Total,"
            << fixed << setprecision(1) << total_hindi << ",100.00,100.00,"
            << fixed << setprecision(1) << total_english << ",100.00,100.00,"
            << fixed << setprecision(1) << total_all << ",100.00,100.00" << endl;
    
    outfile.close();
    
    cout << "Output saved to output.csv" << endl;
    
    return 0;
}
