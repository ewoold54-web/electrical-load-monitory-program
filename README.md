Electrical Appliance Load Monitoring System
Overview

The Electrical Appliance Load Monitoring System is a C++ console application developed to help users track and estimate the electrical energy consumption of household appliances.

The program allows users to register appliances with their power ratings and daily usage hours. It then calculates energy consumption in kilowatt-hours (kWh) and generates a billing estimate based on the electricity tariff provided by the user.

This project demonstrates practical use of core C++ programming concepts such as structures, vectors, input validation, file handling, and modular programming.

Features

The system provides the following functions:

1. Register Appliance

Users can enter:

Appliance name

Power rating (Watts)

Usage hours per day

The appliance data is stored in memory and saved to a file.

2. View Appliances

Displays all registered appliances in a formatted table including:

Appliance number

Appliance name

Power rating

Daily usage hours

3. Energy Summary

Calculates the daily energy consumption (kWh/day) of each appliance using the formula:

Energy (kWh) = Power(W) × Hours / 1000

It also displays the total daily energy consumption.

4. Search Appliance

Users can search for an appliance by name.
The search is case-insensitive, allowing partial matches using string transformation.

5. Billing Summary

The program generates a billing report based on the electricity tariff entered by the user.

The report includes:

Daily energy usage

Daily cost

Estimated monthly energy usage (30 days)

Estimated monthly cost

The billing report is also saved to a text file.

File Storage

The system uses two text files:

appliances.txt

Stores appliance data in this format:
Electric cooker|250|2
TV|46|7

This allows the program to reload saved appliances when restarted.

billing_summary.txt

Stores the generated billing report for reference.

Technologies Used

This project was implemented using:

C++ Programming Language

Visual Studio Code

GCC (g++ compiler)

Git Version Control

GitHub Repository Hosting

Key Programming Concepts Used
Structures

Used to represent each appliance with attributes:

Name

Power rating

Daily usage hours

Vectors

A vector<Appliance> is used to dynamically store appliance records.

File Handling

The program uses:

ifstream
ofstream

to read and write appliance data and billing reports.

Input Validation

Input is validated to prevent:

Empty appliance names

Invalid numbers

Hours outside the range 0–24

String Manipulation

The transform() function and tolower() are used to implement case-insensitive searching.

Modular Programming

The program is divided into several functions such as:

registerAppliance()

viewAppliances()

energySummary()

searchAppliance()

billingSummary()

This improves readability and maintainability.

How to Compile and Run

Compile the program:

g++ main.cpp -o main

Run the program:

./main

(On Windows PowerShell)

.\main.exe
Author

Dennis Asiedu Ewool

Repository

GitHub Repository:

https://github.com/ewoold54-web/electrical-load-monitory-program