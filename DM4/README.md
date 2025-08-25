# Five-Number Summary Analysis - C++ Implementation

## Overview
This project implements the five-number summary algorithm in C++ based on the Python code from the instruction file. The implementation analyzes Indian-related data including state temperatures, populations, and exam scores.

## Files Description

### 1. `five_number_summary.cpp`
- **Purpose**: Basic implementation with hardcoded Indian data
- **Features**: 
  - Calculates five-number summary (Q1, Median, Q3, IQR, Whiskers, Outliers)
  - Uses three datasets: temperatures, populations, and exam scores
  - Displays detailed analysis and statistics

### 2. `five_number_summary_csv.cpp`
- **Purpose**: Enhanced version that reads data from CSV files
- **Features**:
  - Reads data from `indian_data.csv`
  - Falls back to default data if CSV file is not found
  - Displays detailed state-wise data table
  - Provides comprehensive statistical analysis

### 3. `indian_data.csv`
- **Purpose**: CSV file containing Indian states' data
- **Columns**: State, Temperature, Population, ExamScore
- **Data**: 25 Indian states with realistic values

## Data Description

### Temperature Data
- **Source**: Indian states' average summer temperatures (2023)
- **Range**: 27.9°C to 40.2°C
- **Units**: Celsius

### Population Data
- **Source**: Indian states' population estimates (2021)
- **Range**: 1.9 to 20.4 million
- **Units**: Millions

### Exam Score Data
- **Source**: CBSE Class 12 Board Exam scores (sample)
- **Range**: 189 to 456 points
- **Units**: Points out of 500

## How to Compile and Run

### Compilation
```bash
# For basic version
g++ -o five_number_summary five_number_summary.cpp

# For CSV version
g++ -o five_number_summary_csv five_number_summary_csv.cpp
```

### Execution
```bash
# Run basic version
./five_number_summary

# Run CSV version
./five_number_summary_csv
```

## Algorithm Implementation

### Five-Number Summary Steps:
1. **Sort the data** in ascending order
2. **Find Median (Q2)** - middle value of the dataset
3. **Find Q1** - median of the lower half
4. **Find Q3** - median of the upper half
5. **Calculate IQR** - Q3 - Q1
6. **Find Whiskers** - data points within 1.5 × IQR bounds
7. **Identify Outliers** - points beyond whisker bounds

### Key Functions:
- `median()`: Calculates median of a dataset
- `fiveNumberSummaryWithOutliers()`: Main algorithm implementation
- `printSummary()`: Displays formatted results
- `readCSV()`: Reads data from CSV file (CSV version only)

## Output Format

The program provides:
1. **Five-Number Summary** for each dataset
2. **Detailed data table** (CSV version)
3. **Additional analysis** including ranges and averages
4. **Outlier identification**

## Sample Output Structure
```
Five-Number Summary Analysis of Indian Data
===========================================

Indian States' Average Temperatures (Summer 2023) Summary:
==========================================
Q1 (First Quartile): 33.40
Median (Q2): 35.20
Q3 (Third Quartile): 37.20
IQR (Interquartile Range): 3.80
Lower Whisker: 31.80
Upper Whisker: 39.10
Minimum: 31.80
Maximum: 39.10
Outliers: 40.20
```

## Educational Value

This implementation demonstrates:
- **Statistical Analysis**: Five-number summary concepts
- **Data Mining**: Outlier detection and data distribution analysis
- **C++ Programming**: File I/O, data structures, algorithms
- **Real-world Application**: Analysis of Indian demographic and educational data

## Comparison with Python Version

| Feature | Python Version | C++ Version |
|---------|----------------|-------------|
| Data Source | CSV file | CSV file + hardcoded |
| Data Type | Ball/Gel pens | Indian states data |
| Output Format | Basic summary | Detailed analysis |
| File Handling | csv.DictReader | Custom CSV parser |
| Error Handling | Basic | Fallback to default data |

## Conclusion

The C++ implementation successfully replicates the Python algorithm while providing enhanced features and Indian-specific data analysis. It demonstrates the practical application of five-number summary in data mining and statistical analysis.
