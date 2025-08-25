Title- To perform Data Cube operations (Slice, Dice, Roll-Up,Dril-Down,Pivot)

Theory:
OLAP (Online Analytical Processing)
OLAP is a data analysis technology that allows users to examine large volumes of data from multiple perspectives. It is commonly used in business decision-making and reporting systems.
Useful for:
●	Helps summarize complex data

●	Supports decision-making with analytical insights

●	Provides fast answers to analytical queries

●	Allows users to interactively explore data
Common OLAP Operations:
1. Slice
 The slice operation selects a single layer (or slice) of the data cube by fixing a value for one dimension. It is used to view data across the remaining dimensions while keeping one fixed.
Example:
From a 3D cube with dimensions Region, Product, and Time, selecting only the records where Region = 'India' is a slice.
2. Dice
 The dice operation selects a sub-cube by choosing specific values for multiple dimensions. It is useful to view a more focused portion of data by applying multiple filters.
Example:
 Select all records where Region = 'USA' and Product = 'Laptop'.
3. Roll-up
 Roll-up performs aggregation along a dimension. It summarizes the data by climbing up a concept hierarchy. It is used  to get summarized or higher-level information from detailed data
Example:
 From daily sales → monthly sales → yearly sales.
 Or from city → state → country.
4. Drill-Down
 Drill-down is the opposite of roll-up. It provides more detailed data by going down the hierarchy. Its purpose is to get a more granular view of the data for analysis.
Example:
 From yearly sales → monthly sales → daily sales.
5. Pivot (Rotation)
 Pivot reorients the multidimensional view of data — i.e., it rotates the data axes to provide a new perspective. It is very useful for  analyzing the data in a more understandable or insightful layout.
Example:
 Change rows to columns:
 Original: Regions as rows, Products as columns
 After Pivot: Products as rows, Regions as columns

Python Implementation:
def read_csv(filename):
   records = []
   try:
       with open(filename, 'r') as file:
           header = next(file)  # Skip header
           for line in file:
               parts = line.strip().split(',')
               if len(parts) == 4:
                   record = {
                       'Date': parts[0],
                       'Region': parts[1],
                       'Product': parts[2],
                       'Sales': int(parts[3])
                   }
                   records.append(record)
   except FileNotFoundError:
       print("File not found!")
   return records


def print_record(r):
   print(f"{r['Date']} | {r['Region']} | {r['Product']} | {r['Sales']}")


def slice_operation(data, field, value):
   print(f"\nSlice: {field} = {value} ")
   for r in data:
       if r[field].lower() == value.lower():
           print_record(r)


def dice_operation(data, filters):
   print("\nDice")
   for r in data:
       match = all(r[key].lower() == value.lower() for key, value in filters.items())
       if match:
           print_record(r)


def rollup_operation(data, group_field):
   print(f"\nRoll up: (Total Sales : {group_field} )")
   totals = {}
   for r in data:
       key = r[group_field]
       totals[key] = totals.get(key, 0) + r['Sales']
   for key, total in totals.items():
       print(f"{key} → {total}")


def drilldown_operation(data):
   print("\nDrill-down")
   for r in data:
       print_record(r)


def pivot_operation(data, row_field, col_field):
   print(f"\nPivot: {row_field} vs {col_field}")
   pivot = {}
   col_values = set()

   for r in data:
       row = r[row_field]
       col = r[col_field]
       sales = r['Sales']
       col_values.add(col)

       if row not in pivot:
           pivot[row] = {}
       pivot[row][col] = pivot[row].get(col, 0) + sales

   col_values = sorted(col_values)
   print(row_field, *col_values, sep='\t')
   for row in pivot:
       print(row, end='\t')
       for col in col_values:
           print(pivot[row].get(col, 0), end='\t')
       print()


def main():
   filename = input("Enter CSV file name: ")
   data = read_csv(filename)

   if not data:
       print("No data loaded")
       return

   while True:
       print("\n* OLAP Menu *")
       print("1. Original Data")
       print("2. Slice")
       print("3. Dice")
       print("4. Roll-Up")
       print("5. Drill-Down")
       print("6. Pivot")
       print("0. Exit")

       choice = input("Enter your choice: ")

       if choice == '1':
           print("\nOriginal Data ")
           for r in data:
               print_record(r)

       elif choice == '2':
           field = input("Enter field to slice by (Date/Region/Product): ")
           value = input(f"Enter value for {field}: ")
           slice_operation(data, field, value)

       elif choice == '3':
           num_filters = int(input("How many filters? "))
           filters = {}
           for _ in range(num_filters):
               field = input("Enter field name (Date/Region/Product): ")
               value = input(f"Enter value for {field}: ")
               filters[field] = value
           dice_operation(data, filters)

       elif choice == '4':
           field = input("Enter field to group by (Region/Product/Date): ")
           rollup_operation(data, field)

       elif choice == '5':
           drilldown_operation(data)

       elif choice == '6':
           row_field = input("Enter row field: ")
           col_field = input("Enter column field: ")
           pivot_operation(data, row_field, col_field)

       elif choice == '0':
           print("Exiting...")
           break

       else:
           print("Invalid choice")


if __name__ == "__main__":
   main()
