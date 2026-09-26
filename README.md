# Personal Finance Tracker

## Overview

The Personal Finance Tracker is a console-based application for recording and managing income and expenses. It allows users to add transactions, organize them by category, view financial totals, search and filter records, sort transactions, and save data for use in future sessions.

I created this software to strengthen my understanding of C++ by applying the language to a practical problem. The project demonstrates object-oriented programming, functions, conditionals, loops, input validation, Standard Template Library (STL) containers and algorithms, and file input/output. It also gave me experience organizing a program across multiple header and source files instead of placing all functionality in one file.

The demonstration video will show the application running, walk through the main parts of the source code, and explain the C++ syntax and concepts I learned.

[Software Demo Video](http://youtube.link.goes.here)

## Development Environment

The project was developed in Cursor on Windows. Git and GitHub are used for version control and maintaining the project repository. The application can be compiled from a terminal with a C++ compiler such as GNU `g++`.

The software is written in C++ and uses the C++ Standard Library. Important library features include:

- `std::vector` for storing transactions
- `std::string` for text data
- `<algorithm>` for searching and sorting
- `<fstream>` for saving and loading transaction data
- `<iomanip>` for formatted console output
- `<limits>` and string streams for safe input handling

Example compilation command:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Transaction.cpp FinanceManager.cpp InputHelper.cpp -o finance_tracker
```

Run on Windows:

```powershell
.\finance_tracker.exe
```

Or use the helper script to compile and run in one step:

```powershell
.\run_app.bat
```

## Features

- Add income and expense transactions with automatically generated IDs.
- Validate dates, monetary amounts, descriptions, categories, and menu choices.
- Display all transactions in a formatted table.
- Calculate total income, total expenses, and the current balance.
- Search descriptions without case-sensitive matching.
- Filter transactions by income, expense, or category.
- Sort transactions by amount, date, or category in either direction.
- Save transactions to and load transactions from `data/transactions.txt`.
- Skip malformed saved records instead of crashing.

## How It Works

The application is divided into three main components:

- `Transaction` represents one income or expense and stores its ID, date, description, category, amount, and type.
- `FinanceManager` owns the `std::vector<Transaction>` collection and performs calculations, searching, filtering, sorting, saving, and loading.
- `InputHelper` validates console input and keeps invalid values from reaching the rest of the program.

`main.cpp` displays the menus and coordinates these components. Saved records use a simple pipe-delimited format:

```text
1|2026-09-19|Salary|Work|2500.00|Income
```

## C++ Concepts Demonstrated

- Variables and arithmetic expressions for financial calculations
- Conditionals and `switch` statements for program decisions
- Loops for menus, validation, and transaction processing
- Functions for separating individual operations
- Classes, constructors, encapsulation, getters, and setters
- `enum class` for type-safe transaction types
- `std::vector` and `std::unordered_set` STL containers
- `std::sort` algorithms with lambda expressions
- File input and output with streams
- Exception handling and input validation
- Header/source file organization

## Testing

The manual test checklist includes:

1. Start the application with and without an existing data file.
2. Add valid income and expense records.
3. Reject invalid menu options, empty text, bad dates, non-numeric amounts, and negative amounts.
4. Verify income, expense, and balance calculations.
5. Search using different letter casing.
6. Filter by transaction type and category.
7. Sort in ascending and descending order.
8. Save, restart, and reload the saved records.
9. Load an empty or partially malformed file without crashing.
10. Exit both with and without saving.

Automated tests for date validation, calculations, searching, filtering, sorting, saving, and loading are included in `tests/FinanceManagerTests.cpp`.

```bash
g++ -std=c++17 -Wall -Wextra -pedantic tests/FinanceManagerTests.cpp Transaction.cpp FinanceManager.cpp InputHelper.cpp -o finance_tests
./finance_tests
```

## Useful Websites

- [C++ Reference](https://en.cppreference.com/w/)
- [Microsoft C++ Documentation](https://learn.microsoft.com/en-us/cpp/cpp/)
- [Git Documentation](https://git-scm.com/doc)
- [GitHub Documentation](https://docs.github.com/)

## Future Work

- Add monthly reports and spending summaries.
- Allow users to create budgets and receive warnings when limits are reached.
- Support recurring transactions and multiple financial accounts.
- Add CSV export for use with spreadsheet applications.
- Create charts or a graphical interface for visualizing financial activity.
