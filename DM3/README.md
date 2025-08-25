# T-Weight and D-Weight Calculation for Indian Students

## Project Description
This project implements the calculation of Transactional Weight (T-weight) and Discrimination Weight (D-weight) for Indian students' academic performance data using C++.

## Files
- `dm3.cpp` - Main C++ program for T-weight and D-weight calculation
- `students_study.csv` - Input data file with Indian students' Hindi and English marks
- `output.csv` - Generated output file with calculated weights
- `README.md` - This documentation file

## Dataset Information
**Source**: Indian Students' Academic Performance Data
- **Subjects**: Hindi and English Marks (out of 100)
- **Students**: 10 Indian students with realistic academic scores
- **Data Type**: Academic marks representing typical Indian school performance

## How to Compile and Run
```bash
g++ -o dm3 dm3.cpp
./dm3
```

## Understanding the Weights

### T-Weight (Transactional Weight)
- Measures how much each student contributes to the total marks in each subject
- Formula: (Student's marks in subject / Total marks in subject) × 100
- Shows representativeness within the subject

### D-Weight (Discrimination Weight)
- Measures how much of each student's total marks come from each subject
- Formula: (Student's marks in subject / Student's total marks) × 100
- Shows uniqueness across subjects

## Sample Output
The program generates a CSV file with columns:
- Student name
- Hindi marks and corresponding T-weight and D-weight
- English marks and corresponding T-weight and D-weight
- Combined marks and weights

## Data Source
The dataset contains realistic Indian student names and academic scores that reflect typical performance patterns in Indian schools, making it culturally relevant and educationally meaningful for analysis.
