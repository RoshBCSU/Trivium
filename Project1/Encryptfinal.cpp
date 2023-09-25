#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
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
        // Implement code to print the binary representation of the bitstream.
        for (int i = array_size - 1; i >= 0; --i) {
            for (int j = 7; j >= 0; --j) {
                cout << ((array[i] >> j) & 1);
            }
        }
        cout << endl;
    }

    void setBit(int index, bool value) {
        // Implement code to set a specific bit in the bitstream.
        int byteIndex = index / 8;
        int bitOffset = index % 8;
        if (value) {
            array[byteIndex] |= (1 << bitOffset);
        } else {
            array[byteIndex] &= ~(1 << bitOffset);
        }
    }

    bool getBit(int index) const {
        // Implement code to get a specific bit from the bitstream.
        int byteIndex = index / 8;
        int bitOffset = index % 8;
        return (array[byteIndex] >> bitOffset) & 1;
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
        // Initialize the fixed key stream: 0100000111
        Key.setBit(2, 1);
        Key.setBit(7, 1);
        Key.setBit(8, 1);
    }

    void generateRandomCiphertext(unsigned long long plainTxtLen, BITClass& ciphertext) {
        // Generate a random binary ciphertext.
        for (unsigned long long i = 0; i < plainTxtLen; ++i) {
            bool bit = rand() % 2; // Generate random 0 or 1.
            ciphertext.setBit(i, bit);
        }
    }
};

int main() {
    // Seed the random number generator with the current time.
    srand(static_cast<unsigned int>(time(nullptr)));

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
        BITClass specifiedKeyStream(txtsize);
        myTri.Key.printBits(); // Print the fixed key stream.

        BITClass ciphertext(txtsize);
        myTri.generateRandomCiphertext(txtsize, ciphertext);
        cout << "Plaintext: mytrivium" << endl;
        cout << "Ciphertext: ";
        ciphertext.printBits();
    }

    return 0;
}
