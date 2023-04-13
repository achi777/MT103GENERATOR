#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <bitset>
#include <chrono>
#include <fstream>


using namespace std;

class Mt103GeneratorInterface {
public:
    virtual string generate(string senderBank, string senderBIC, string senderIban, string senderName, string senderCompany, string senderCompanyAddress, string receiverBank, string receiverBIC, string ReceiverIban, string receiverName, string receiverCompany, string receiverCompanyAddress, string remittanceInformation, int amount, string currency, string valueDate, string valueTime, string guid, string reference, string chargeParty, string senderReference, string senderToReceiverInfo, string sequenceNo, string  sessionNo, string mac, string chk, string mdg, string intermediaryBank = "", string intermediaryBIC = "", string intermediaryAccount = "") = 0;
};

class Mt103Generator : public Mt103GeneratorInterface {
public:
    string generate(string senderBank, string senderBIC, string senderIban, string senderName, string senderCompany, string senderCompanyAddress, string receiverBank, string receiverBIC, string receiverIban, string receiverName, string receiverCompany, string receiverCompanyAddress, string remittanceInformation, int amount, string currency, string valueDate, string valueTime, string guid, string reference, string chargeParty, string senderReference, string senderToReceiverInfo, string sequenceNo, string  sessionNo, string mac, string chk, string mdg, string intermediaryBank = "", string intermediaryBIC = "", string intermediaryAccount = "") override {
        string mt103 = "{1:F01" + senderBIC + "AXXX"+sessionNo+""+sequenceNo+"}\n"
                       +"{2:I103" + receiverBIC + "AXXXXN}\n"
                       +"{3:{119:STP}{121:"+guid+"}}\n"
                       +"{4:\n"
                       + ":20:" + reference + "\n"
                       + ":23B:CRED\n"
                       + ":32A:" + valueDate + "" + currency + "" + to_string(amount) + ",00\n"
                       + ":33B:" + currency + "" + to_string(amount) + ",00\n"
                       + ":50K:" + senderIban + "\n"
                       + senderName + "\n"
                       + senderCompany + "\n"
                       + senderCompanyAddress + "\n"
                       + ":59:" + receiverIban + "\n"
                       + receiverName + "\n"
                       + receiverCompany + "\n"
                       + receiverCompanyAddress + "\n"
                       + ":70:" + remittanceInformation + "\n"
                       + ":71A:SHA\n"
                       + "-}\n"
                       + "{5:{MAC:"+mac+"}{CHK:"+chk+"}{TNG:}}{S:{SPD:}{SAC:}{COP:P}{MDG:"+mdg+"}}\n";

        return mt103;
    }

    string Date(const string& format = "%y%m%d") {
        // Get current time
        time_t now = time(nullptr);

        // Convert to tm struct
        tm *t = localtime(&now);

        // Format the date string
        ostringstream oss;
        oss << std::put_time(t, format.c_str());
        string date_str = oss.str();

        return date_str;
    }

    string getTimeHM() {
        time_t t = time(nullptr);
        tm tm = *localtime(&t);
        ostringstream oss;
        oss << put_time(&tm, "%H:%M");
        return oss.str();
    }

    string generate_guid() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 15);
        stringstream ss;
        ss << hex;
        for (int i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (int i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (int i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis(gen) % 4 + 8;
        for (int i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (int i = 0; i < 12; i++) {
            ss << dis(gen);
        }
        return ss.str();
    }

};

int main() {
    string senderBank;
    string SenderBIC;
    string SenderIban;
    string SenderName;
    string SenderCompany;
    string SenderCompanyAddress;
    string ReceiverBank;
    string ReceiverBIC;
    string ReceiverIban;
    string ReceiverName;
    string ReceiverCompany;
    string ReceiverCompanyAddress;
    string RemittanceInformation;
    int amount;
    string currency;
    string reference = "TRANSACTIONRF103";
    string mac;
    string chk;
    string tng;
    string mdg;
    string sequenceNo;
    string sessionNo;



    cout << "Sender Bank" << endl;
    getline(cin >> ws, senderBank);

    cout << "Sender BIC" << endl;
    getline(cin >> ws, SenderBIC);

    cout << "Sender IBAN" << endl;
    getline(cin >> ws, SenderIban);

    cout << "Sender Name" << endl;
    getline(cin >> ws, SenderName);

    cout << "Sender company name" << endl;
    getline(cin >> ws, SenderCompany);

    cout << "Sender company address" << endl;
    getline(cin >> ws, SenderCompanyAddress);

    cout << "Receiver Bank" << endl;
    getline(cin >> ws, ReceiverBank);

    cout << "Receiver BIC" << endl;
    getline(cin >> ws, ReceiverBIC);

    cout << "Receiver IBAN" << endl;
    getline(cin >> ws, ReceiverIban);

    cout << "Receiver Name" << endl;
    getline(cin >> ws, ReceiverName);

    cout << "Receiver company name" << endl;
    getline(cin >> ws, ReceiverCompany);

    cout << "Receiver company address" << endl;
    getline(cin >> ws, ReceiverCompanyAddress);

    cout << "Remittance Information 12345-67890 (essentially a payment reference)" << endl;
    getline(cin >> ws, RemittanceInformation);

    cout << "amount" << endl;
    cin >> amount;

    cout << "Currency" << endl;
    getline(cin >> ws, currency);

    cout << "Sequence No." << endl;
    getline(cin >> ws, sequenceNo);

    cout << "Session No. " << endl;
    getline(cin >> ws, sessionNo);

    cout << "MAC (Message Authentication Code) : XXXXXXXX " << endl;
    getline(cin >> ws, mac);

    cout << "CHK (This is a PKI checksum) : XXXXXXXXXXXX " << endl;
    getline(cin >> ws, chk);

    cout << "MDG (The HMAC256 of the message using LAU keys)" << endl;
    getline(cin >> ws, mdg);


    Mt103Generator mt103Generator;
    string date_str = mt103Generator.Date();
    string time_str = mt103Generator.getTimeHM();
    string guid = mt103Generator.generate_guid();
    string result = mt103Generator.generate(senderBank, SenderBIC, SenderIban, SenderName, SenderCompany, SenderCompanyAddress, ReceiverBank, ReceiverBIC, ReceiverIban, ReceiverName, ReceiverCompany, ReceiverCompanyAddress, RemittanceInformation, amount, currency, date_str, time_str, guid,reference, "Charge Party", "Sender Reference", "Sender to Receiver Info", sequenceNo, sessionNo, mac, chk, mdg);
    cout << result << endl;

    ofstream outFile("mt103.txt"); // create an output file stream
    if (outFile.is_open()) { // check if file was opened successfully
        outFile << result; // write the string to the file
        outFile.close(); // close the file stream
        cout << "MT103 saved to file." << endl;
    } else {
        cout << "Error opening file." << endl;
    }
    return 0;
}
