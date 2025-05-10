#include <bits/stdc++.h>
#include <unistd.h>
#include <chrono>
using namespace std;

class Member
{
private:
    string mobile;
    string name;
    double amount;
    string pin;

public:
    Member() {}
    Member(const string &m, const string &n, double a, const string &p)
        : mobile(m), name(n), amount(a), pin(p) {}

    string getMobile() const { return mobile; }
    string getName() const { return name; }
    double getAmount() const { return amount; }
    string getPin() const { return pin; }

    void setName(const string &n) { name = n; }
    void setAmount(double a) { amount = a; }
    void setPin(const string &p) { pin = p; }
};

class History
{
private:
    struct Transaction
    {
        int transactionID;
        string description;
        double amount;
        double balance;
    };

    vector<Transaction> transactions;

public:
    void addTransaction(int id, const string &desc, double amt, double bal)
    {
        Transaction t;
        t.transactionID = id;
        t.description = desc;
        t.amount = amt;
        t.balance = bal;
        transactions.push_back(t);
    }

    void displayHistory()
    {
        cout << "Tran ID\tDescription\tAmount\tBalance" << endl;
        for (const auto &t : transactions)
        {
            cout << t.transactionID << "\t" << t.description << "\t"
                 << t.amount << "\t" << t.balance << endl;
        }
    }
};

class MyCash
{
private:
    vector<Member> members;
    History history;
    string mobile;

    std::chrono::time_point<std::chrono::system_clock> loginTime;

    void startSession()
    {
        loginTime = std::chrono::system_clock::now();
    }

    bool isSessionExpired() const
    {
        auto currentTime = std::chrono::system_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - loginTime).count();
        return elapsedSeconds >= 120; // 2 minutes session timeout
    }

    string trim(const string &str) const
    {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");

        if (start == string::npos || end == string::npos)
        {
            return "";
        }

        return str.substr(start, end - start + 1);
    }

    bool isMemberExists(const string &mobile) const
    {
        string trimmedMobile = trim(mobile);
        auto it = find_if(members.begin(), members.end(),
                          [this, trimmedMobile](const Member &m)
                          { return trim(m.getMobile()) == trimmedMobile; });

        return it != members.end();
    }

    Member &getMember(const string &mobile)
    {
        auto it = find_if(members.begin(), members.end(),
                          [mobile](const Member &m)
                          { return m.getMobile() == mobile; });

        if (it == members.end())
        {
            throw logic_error("Member not found");
        }

        return *it;
    }

    void saveDataToFile()
    {
        char buffer[PATH_MAX];
        if (getcwd(buffer, sizeof(buffer)) != NULL)
        {
            string filePath = string(buffer) + "/myCashData.txt";
            ofstream outFile(filePath);

            if (outFile.is_open())
            {
                for (const auto &member : members)
                {
                    outFile << member.getMobile() << " "
                            << member.getName() << " "
                            << member.getAmount() << " "
                            << member.getPin() << "\n";
                }

                outFile.close();
            }
            else
            {
                cerr << "Error opening file for writing." << endl;
            }
        }
        else
        {
            cerr << "Error getting the current working directory." << endl;
        }
    }
    // Function to load member data from a file
    void loadDataFromFile()
    {
        char buffer[PATH_MAX];
        if (getcwd(buffer, sizeof(buffer)) != NULL)
        {
            string filePath = string(buffer) + "/myCashData.txt"; // Combine with the file name
            ifstream inFile(filePath);

            if (inFile.is_open())
            {
                members.clear();
                string mobile, name, pin;
                double amount;

                while (inFile >> mobile >> name >> amount >> pin)
                {
                    members.emplace_back(mobile, name, amount, pin);
                }

                inFile.close();
            }
        }
        else
        {
            cerr << "Error getting the current working directory." << endl;
        }
    }

    int generateOTP() const
    {
        int otp = rand() % 9000 + 1000;
        cout << "myCash OTP: " << otp << endl;
        return otp;
    }

    bool verifyOTP(int otpEntered, int otpGenerated) const
    {
        return otpEntered == otpGenerated;
    }

public:
    MyCash()
    {
        srand(time(0));
        loadDataFromFile();
    }
    void registerMember()
    {
        cout << "Enter Mobile No. (11-digit): ";
        string mobile;
        cin >> mobile;

        if (isMemberExists(mobile))
        {
            cout << "1. Member already exists" << endl;
            return;
        }

        cout << "Enter Name: ";
        string name;
        cin.ignore();
        getline(cin, name);

        cout << "Enter pin (5-digit): ";
        string pin;
        cin >> pin;

        cout << "Reconfirm pin: ";
        string confirmPin;
        cin >> confirmPin;

        int confirmedOtp;

        if (pin != confirmPin)
        {
            cout << "7. Pins must be same" << endl;
            return;
        }

        confirmedOtp = generateOTP();
        cout << "Enter OTP: ";
        int otpEntered;
        cin >> otpEntered;

        // Simulating OTP verification
        if (!verifyOTP(otpEntered, confirmedOtp))
        {
            cout << "5. OTP does NOT matched" << endl;
            return;
        }

        members.emplace_back(mobile, name, 0.0, pin);
        cout << "Registration is Successful" << endl;
    }

    bool login()
    {
        cout << "Enter Mobile No. (11-digit): ";
        cin >> mobile;

        if (!isMemberExists(mobile))
        {
            cout << "2. Member NOT exists" << endl;
            return false;
        }

        cout << "Enter pin: ";

        string pin;
        cin >> pin;

        Member &member = getMember(mobile);

        if (member.getPin() != pin)
        {
            cout << "8. Invalid login" << endl;
            return false;
        }

        cout << "Login is Successful" << endl;
        startSession();
        return true;
    }

    void updateMember()
    {
        if (isSessionExpired())
        {
            cout << "6. OTP time has expired. Please log in again." << endl;
            return;
        }
        try
        {
            Member &currentMember = getMember(mobile);

            cout << "Old Name: " << currentMember.getName() << endl;

            cout << "New Name (enter to ignore): ";
            string newName;
            cin.ignore();
            getline(cin, newName);

            cout << "Old pin: " << currentMember.getPin() << endl;

            cout << "New pin (enter to ignore): ";
            string newPin;
            cin >> newPin;

            if (!newName.empty())
            {
                currentMember.setName(newName);
            }

            if (!newPin.empty())
            {
                cout << "Confirm New pin: ";
                string confirmPin;
                cin >> confirmPin;

                if (newPin != confirmPin)
                {
                    cout << "7. Pins must be same" << endl;
                    return;
                }

                int confirmedOtp = generateOTP();
                cout << "Enter OTP: ";
                int otpEntered;
                cin >> otpEntered;

                // Simulating OTP verification
                if (!verifyOTP(otpEntered, confirmedOtp))
                {
                    cout << "5. OTP does NOT matched" << endl;
                    return;
                }

                currentMember.setPin(newPin);
            }

            cout << "Update is Successful" << endl;
        }
        catch (const logic_error &e)
        {
            cout << e.what() << endl;
        }
    }

    void removeMember()
    {
        int confirmedOtp = generateOTP();
        cout << "Enter OTP: ";
        int otpEntered;
        cin >> otpEntered;

        // Simulating OTP verification
        if (!verifyOTP(otpEntered, confirmedOtp))
        {
            cout << "5. OTP does NOT matched" << endl;
            return;
        }

        members.erase(std::remove_if(members.begin(), members.end(),
                                     [this](const Member &m)
                                     { return m.getMobile() == mobile; }),
                      members.end());

        cout << "Remove is Successful" << endl;
    }

    void sendMoney()
    {
        if (isSessionExpired())
        {
            cout << "6. OTP time has expired. Please log in again." << endl;
            return;
        }
        cout << "Enter Destination no. (11-digit): ";
        string destMobile;
        cin >> destMobile;

        if (!isMemberExists(destMobile))
        {
            cout << "9 Destination Mobile no. is invalid" << endl;
            return;
        }

        cout << "Enter Amount: ";
        double amount;
        cin >> amount;

        Member &sender = getMember(mobile);
        Member &receiver = getMember(destMobile);

        if (sender.getAmount() < amount)
        {
            cout << "3 Insufficient Fund" << endl;
            return;
        }

        cout << "Sending " << amount << " to " << destMobile << endl;
        cout << "Are you sure(Y/N)? ";
        char choice;
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            int confirmedOtp = generateOTP();
            cout << "Enter OTP: ";
            int otpEntered;
            cin >> otpEntered;

            // Simulating OTP verification
            if (!verifyOTP(otpEntered, confirmedOtp))
            {
                cout << "5. OTP does NOT matched" << endl;
                return;
            }

            sender.setAmount(sender.getAmount() - amount);
            receiver.setAmount(receiver.getAmount() + amount);

            history.addTransaction(rand() % 1000, "Send Money", amount, sender.getAmount());
            cout << "Send Money is Successful" << endl;
        }
        else
        {
            cout << "Send Money Cancelled" << endl;
        }
    }

    void cashIn()
    {
        cout << "Enter Amount: ";
        double amount;
        cin >> amount;

        Member &member = getMember(mobile);

        cout << "Cash-in " << amount << endl;
        cout << "11. Are you sure(Y/N)? ";
        char choice;
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            member.setAmount(member.getAmount() + amount);
            history.addTransaction(rand() % 1000, "Cash-in", amount, member.getAmount());
            cout << "Cash-in is Successful" << endl;
        }
        else
        {
            cout << "Cash-in Cancelled" << endl;
        }
    }

    void cashOut()
    {
        cout << "Enter Amount: ";
        double amount;
        cin >> amount;

        Member &member = getMember(mobile);

        int confirmedOtp = generateOTP();
        cout << "Enter OTP: ";
        int otpEntered;
        cin >> otpEntered;

        // Simulating OTP verification
        if (!verifyOTP(otpEntered, confirmedOtp))
        {
            cout << "5 OTP does NOT matched" << endl;
            return;
        }

        if (member.getAmount() < amount)
        {
            cout << "3 Insufficient Fund" << endl;
            return;
        }

        cout << "Cash-out " << amount << endl;
        cout << "11. Are you sure(Y/N)? ";
        char choice;
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            member.setAmount(member.getAmount() - amount);
            history.addTransaction(rand() % 1000, "Cash-out", amount, member.getAmount());
            cout << "Cash-out is Successful" << endl;
        }
        else
        {
            cout << "Cash-out Cancelled" << endl;
        }
    }

    void payBill()
    {
        cout << "Enter Bill Type (Gas/Electricity/Water/Internet-1/2/3/4): ";
        int billType;
        cin >> billType;

        cout << "Your ";
        switch (billType)
        {
        case 1:
            cout << "Gas";
            break;
        case 2:
            cout << "Electricity";
            break;
        case 3:
            cout << "Water";
            break;
        case 4:
            cout << "Internet";
            break;
        default:
            cout << "Invalid";
            break;
        }

        cout << " Bill: ";
        double billAmount;
        cin >> billAmount;

        cout << "11. Want to pay(Y/N)? ";
        char choice;
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            int confirmedOtp = generateOTP();
            cout << "Enter OTP: ";
            int otpEntered;
            cin >> otpEntered;

            // Simulating OTP verification
            if (!verifyOTP(otpEntered, confirmedOtp))
            {
                cout << "5 OTP does NOT matched" << endl;
                return;
            }

            Member &member = getMember(mobile);
            if (member.getAmount() < billAmount)
            {
                cout << "3 Insufficient Fund" << endl;
                return;
            }

            member.setAmount(member.getAmount() - billAmount);
            history.addTransaction(rand() % 1000, "Pay Bill", billAmount, member.getAmount());
            cout << "Bill Payment is Successful" << endl;
        }
        else
        {
            cout << "Bill Payment Cancelled" << endl;
        }
    }

    void checkBalance()
    {
        cout << "Balance: " << getMember(mobile).getAmount() << endl;
    }

    void displayHistory()
    {
        history.displayHistory();
    }
    ~MyCash()
    {
        saveDataToFile();
    }
};

int main()
{

    MyCash myCash;

    int option;

    do
    {
        cout << "\n*** MyCash Login Menu ***" << endl;
        cout << "1. Login\n2. Register\n3. Exit\nEnter Your Option: ";
        cin >> option;

        switch (option)
        {
        case 1:
            if (myCash.login())
            {
                do
                {
                    cout << "\n********** MyCash Menu ********" << endl;
                    cout << "1. Update Me\n2. Remove Me\n3. Send Money\n4. Cash-in\n5. Cash-out\n"
                         << "6. Pay Bill\n7. Check Balance\n8. History\n9. Logout\nEnter Your Option (1-9): ";
                    cin >> option;

                    switch (option)
                    {
                    case 1:
                        myCash.updateMember();
                        break;
                    case 2:
                        myCash.removeMember();
                        option = 9;
                        break;
                    case 3:
                        myCash.sendMoney();
                        break;
                    case 4:
                        myCash.cashIn();
                        break;
                    case 5:
                        myCash.cashOut();
                        break;
                    case 6:
                        myCash.payBill();
                        break;
                    case 7:
                        myCash.checkBalance();
                        break;
                    case 8:
                        myCash.displayHistory();
                        break;
                    case 9:
                        cout << "Logout Successful" << endl;
                        break;
                    default:
                        cout << "10 Invalid Option" << endl;
                    }
                } while (option != 9);
            }
            break;
        case 2:
            myCash.registerMember();
            break;
        case 3:
            cout << "Exiting MyCash Application" << endl;
            break;
        default:
            cout << "10.Invalid Option" << endl;
        }

    } while (option != 3);

    return 0;
}
