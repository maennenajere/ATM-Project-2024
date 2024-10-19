# ATM-Project-2024

## Overview
This project is a **simulation** of an Automated Teller Machine (ATM) system, developed by our group. It offers functionalities like balance inquiry, cash withdrawal, deposits, user authentication, session management, and transaction history. Primarily coded in **C** and **C++**, this project aims to provide a comprehensive ATM simulation experience with real-world inspired functionality.

## Authors
- [Jere Mäennenä](https://github.com/maennenajere)
- [Veeti Punnonen](https://github.com/SheIITear)
- [Santeri Pasula](https://github.com/santeripas)
- [Martti Prittinen](https://github.com/MarttiPrittinen)

## ER Diagram
![ER Diagram](https://github.com/maennenajere/ATM-Project-2024/blob/main/ER-Kaavio.png)

## Features
- **Basic ATM operations** like balance inquiry, cash withdrawal, and deposits.
- **User authentication** and secure session management to ensure privacy and data integrity.
- **Detailed transaction history** with the option for receipt generation, mimicking real-world ATM receipts.
- **Database integration** to manage customers, their cards, and transaction records efficiently.
- **RFID simulation**: Simulates the customer's bank card using RFID technology.

## Installation

### Prerequisites
Ensure that the following tools are installed on your system:
- **CMake**: For managing the build process
- **GCC or Clang**: For compiling the project

To build and run the project, follow these steps:

### Using CMake
1. Create a build directory:
    ```sh
    mkdir build && cd build
    ```
2. Run CMake to configure the project:
    ```sh
    cmake ..
    ```
3. Build the project:
    ```sh
    make
    ```
4. Run the executable:
    ```sh
    ./atm_project
    ```

## Usage
Once the project is built, you can launch the executable to simulate ATM operations. The user interface will guide you through various operations like checking balance, withdrawing cash, and depositing funds. A transaction history will be generated after each session, simulating an ATM receipt.
