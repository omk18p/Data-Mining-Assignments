# Market Basket Analysis using Apriori Algorithm

## Overview
This C++ implementation demonstrates the Apriori algorithm for market basket analysis, finding frequent itemsets in grocery transaction data.

## Files
- `MarketBasketAnalyzer.cpp` - Main C++ implementation
- `grocery_transactions.txt` - Sample grocery transaction dataset
- `README.md` - This documentation file

## Features
- Object-oriented design with MarketBasketAnalyzer class
- Configurable minimum support threshold
- Detailed transaction display
- Progressive itemset generation (1-itemsets, 2-itemsets, 3-itemsets, etc.)
- Support count reporting for each frequent itemset

## Dataset
The `grocery_transactions.txt` file contains 20 grocery transactions with items like:
- bread, milk, eggs, butter, cheese, yogurt

## Compilation and Execution
```bash
g++ -o MarketBasketAnalyzer MarketBasketAnalyzer.cpp
./MarketBasketAnalyzer
```

## Algorithm Parameters
- Minimum Support Threshold: 2 (configurable in code)
- Input: Transaction file with space-separated items per line
- Output: Frequent itemsets with their support counts

## Key Differences from Original Java Implementation
1. **Different naming**: MarketBasketAnalyzer vs Apriori class
2. **Different dataset**: Grocery items vs generic items
3. **Different structure**: Object-oriented C++ vs procedural Java
4. **Different variables**: purchaseHistory vs transactions, etc.
5. **Enhanced output**: More detailed formatting and progress indicators
