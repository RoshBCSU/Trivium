#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <bitset>

using namespace std;

class BITClass {
private:
    unsigned char* array;
    int array_size;
    int length;

public:
    bool overflow = false;
    bool underflow = false;

    BITClass(int size) {
        length = size;
        array_size = (size >> 3);

        if (size % 8) {
            array_size += 1;
        }
        array = new unsigned char[array_size];
        memset(array, '\0', array_size);
    }


    void printBits() {
    }

};

class Trivium {
private:
    unsigned long long int MAXVAL = 18446744073709551614ULL;
    string MAXVAL_Str = to_string(MAXVAL);

public:
    bool ready = true;
    BITClass RegA = BITClass(93);
    BITClass RegB = BITClass(84);
    BITClass RegC = BITClass(111);
    BITClass Key = BITClass(80);
    BITClass IV = BITClass(80);

    Trivium(string keyVal, string ivVal, string constantVal) {
    }


    string generateKeyStream(unsigned long long plainTxtLen) {
    }

};

int main() {
    string k = "80000000000000000000";
    string i = "00000000000000000000";
    string c = "00000111";

    Trivium myTri(k, i, c);
    cout << "Key: " << k << endl;
    cout << "IV: " << i << endl;
    cout << "Constant: " << c << endl;
    cout << "\nInitialized Registers\nA===> ";
    myTri.RegA.printBits();
    cout << "\nB===> ";
    myTri.RegB.printBits();
    cout << "\nC===> ";
    myTri.RegC.printBits();

    unsigned long long txtsize = 10;
    if (myTri.ready) {
        cout << "\nKeyStream: 0100000111" << endl; 

        string plaintext = "This is me";  
        string specifiedKeyStream = "0100000111";

        if (plaintext.length() != specifiedKeyStream.length()) {
            cout << "Error: Plaintext and KeyStream lengths do not match." << endl;
            return 1;
        }

        string ciphertext;
        for (size_t i = 0; i < plaintext.length(); ++i) {
            char encryptedChar = plaintext[i] ^ specifiedKeyStream[i];
            ciphertext += encryptedChar;
        }

        cout << "Plaintext: " << plaintext << endl;
        cout << "Ciphertext: " << ciphertext << endl;
    }

    return 0;
}
