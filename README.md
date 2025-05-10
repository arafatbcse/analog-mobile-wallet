MyCash
A simple command-line based digital wallet system in C++.
Developed by Monir Hossain.

Features
Member Registration: Create a new MyCash account with mobile verification and OTP.

Login: Secure login with mobile number and PIN.

Cash-In / Cash-Out: Add or withdraw money from your MyCash balance.

Send Money: Transfer money securely between users.

Pay Bills: Pay Gas, Electricity, Water, or Internet bills.

Transaction History: View all your past transactions.

Session Management: Session expires automatically after 2 minutes.

Data Persistence: All user data is saved and loaded from a local text file (myCashData.txt).

Technologies Used
C++17

Standard Libraries:

<vector>, <string>, <fstream>, <algorithm>, <chrono>, <unistd.h>, and more.

How to Run
Clone the Repository:

bash
Copy
Edit
git clone https://github.com/your-username/MyCash.git
cd MyCash
Compile the Code:

bash
Copy
Edit
g++ -o mycash mycash.cpp
Run the Program:

bash
Copy
Edit
./mycash
Program Flow
Main Menu:

Login

Register

Exit

After Login Menu:

Cash-In

Cash-Out

Send Money

Pay Bill

Update Profile

Check Balance

Transaction History

Remove Account

Logout

Data Storage
Member data (mobile, name, balance, pin) is saved in myCashData.txt located in the program's running directory.

On starting the program, data is automatically loaded from this file.

Upon exiting, any changes are saved automatically.

Screenshots
(You can add screenshots showing registration, login, money transfer, etc.)

Contributing
Pull requests are welcome.
For major changes, please open an issue first to discuss what you would like to change.

Author
Monir Hossain

ID: 2303028
