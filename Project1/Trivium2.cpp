#include <iostream>
#include <string.h>
#include <vector>
#include <bitset>

using namespace std;

class BITClass
{
private:
    unsigned char *array;
    int array_size;
    int length;

public:
    bool overflow = false;
    bool underflow = false;

    BITClass(int size)
    {
        length = size;
        array_size = (size >> 3);

        if (size % 8)
        {
            array_size += 1;
        }
        array = new unsigned char[array_size];
        memset(array, '\0', array_size);
    }

    // get bit value at location bit b
    double getBitAt(int b)
    {
        if (b < length)
        {
            // b >> 3 will divide b by 8
            return (array[(b >> 3)] & (char)(1 << (b % 8))) >> (b % 8);
        }
        else
        {
            return 0;
        }
        // return array[b >> 3]  &= (1 << b % 8);
    }

    void clearBitAt(int b)
    {
        if (b < length)
        {
            array[(b >> 3)] &= (char)(~(1 << (b % 8)));
        }
    }

    void setBitAt(int b)
    {
        if (b < length)
        {
            array[(b >> 3)] |= (1 << (b % 8));
        }
        else
        {
            cout << "SetBitAt out of bound: max bit len is " << length << endl;
        }
    }

    void setByteAt(int index, unsigned char v)
    {
        if (index < array_size)
        {
            array[index] = v;
        }
        else
        {
            cout << "SetByteAt out of bound: max byte len is " << array_size << endl;
        }
    }

    // byte is an unsigned char in c++
    const unsigned char getByteAt(int index)
    {
        if (index < array_size)
        {
            return array[index];
        }
        else
        {
            return 0;
        }
    }

    BITClass shiftToLeft(const int offset)
    {
        BITClass result(length);
        for (int i = 0; i < array_size; i++)
        {
            result.setByteAt(i, getByteAt(i));
        }
        int carry = 0;
        for (int j = 0; j < offset; j++)
        {
            for (int i = 0; i < array_size; i++)
            {
                int x = result.getByteAt(i);
                int y = x << 1;
                if (carry)
                {
                    y = y + 1;
                }
                result.setByteAt(i, y);
                carry = x & 128;
            }
        }
        return result;
    }

    BITClass shiftToRight(const int offset)
    {
        BITClass result(length);
        for (int i = 0; i < array_size; i++)
        {
            result.setByteAt(i, getByteAt(i));
        }

        int carry = 0;
        for (int j = 0; j < offset; j++)
        {
            for (int i = array_size - 1; i >= 0; i--)
            {
                int x = result.getByteAt(i);
                int y = (x >> 1) | (carry << 7); // Right shift x and OR it with the carry shifted to the leftmost bit
                result.setByteAt(i, y);
                carry = x & 1; // Set carry to the rightmost bit of x
            }
        }

        return result;
    }

    void printBits()
    {
        int unusedBit;
        unusedBit = (8 - (length % 8)) % 8;

        for (int i = array_size - 1; i >= 0; i--)
        {
            cout << "\033[1;34m|\033[0;30m";

            int x = array[i];
            cout << x << ": ";

            int v;

            for (int j = 7; j >= 0; j--)
            {
                v = (x & 128);

                if ((i == array_size - 1) && (unusedBit != 0))
                {
                    cout << "\033[1;31m-\033[0;30m";
                }
                else
                {
                    cout << (v >> 7);
                }

                if (unusedBit != 0)
                {
                    unusedBit--;
                }

                x = x << 1;
            }
        }
        cout << "\033[1;34m|\033[0;30m";
        cout << "\033[0m";
    }

    void reset()
    {
        memset(array, '\0', array_size);
    }
};

class Trivium
{
private:
    unsigned long long int MAXVAL = 18446744073709551614;
    string MAXVAL_Str = to_string(MAXVAL);

public:
    bool ready = true;
    BITClass RegA = BITClass(93);
    BITClass RegB = BITClass(84);
    BITClass RegC = BITClass(111);
    BITClass Key = BITClass(80);
    BITClass IV = BITClass(80);

    Trivium(string keyVal, string ivVal, string constantVal)
    {
        string KeyBin = ConvertToBin(keyVal);
        string IVBin = ConvertToBin(ivVal);

        initiateRegisterAB(KeyBin, RegA, 'a');
        initiateRegisterAB(IVBin, RegB, 'b');
        initiateRegisterC(constantVal, RegC);
    }

    void initiateRegisterAB(string k, BITClass &Reg, char RegName)
    {
        int len = k.length();
        int RegSize = 0;
        if (RegName == 'a')
        {
            RegSize = 93;
        }
        else if (RegName == 'b')
        {
            RegSize = 84;
        }
        else
        {
            cout << "Error happened while registering bits to the registers" << endl;
            ready = false;
            return;
        }
        for (int i = 0; i < len; i++)
        {
            if (k.at(i) == '1')
            {
                Reg.setBitAt(RegSize - i - 1);
            }
        }
    }

    void initiateRegisterC(string c, BITClass &Reg)
    {
        int len = c.length();
        if (len <= 111)
        {
            for (int i = 0; i < 111 - len; i++)
            {
                c = '0' + c;
            }
            len = c.length();
            int RegSize = 111;
            for (int i = 0; i < len; i++)
            {
                if (c.at(i) == '1')
                {
                    Reg.setBitAt(RegSize - i - 1);
                }
            }
        }
        else
        {
            cout << "The length of Constant value should be shorter than 111" << endl;
            ready = false;
        }
    }

    bool isGreaterThanMaxValue(string &decimalStr)
    {
        try
        {
            uint64_t value = stoull(decimalStr);
            return value > MAXVAL;
        }
        catch (const out_of_range &e)
        {
            return true;
        }
    }

    string subtractDecimalStrings(string &decimalValue1, string &decimalValue2)
    {
        int carry = 0;
        int i = decimalValue1.length() - 1;
        int j = decimalValue2.length() - 1;
        string result = "";

        while (i >= 0 || j >= 0 || carry)
        {
            int num1 = (i >= 0) ? (decimalValue1[i] - '0') : 0;
            int num2 = (j >= 0) ? (decimalValue2[j] - '0') : 0;

            int diff = num1 - num2 - carry;

            if (diff < 0)
            {
                diff += 10;
                carry = 1;
            }
            else
            {
                carry = 0;
            }

            result = to_string(diff) + result;

            if (i >= 0)
                i--;
            if (j >= 0)
                j--;
        }

        size_t nonZeroIdx = result.find_first_not_of('0');
        if (nonZeroIdx != string::npos)
        {
            result = result.substr(nonZeroIdx);
        }
        else
        {
            result = "0";
        }

        return result;
    }

    string decimalToBinary80Bits(string &decimalValue)
    {
        unsigned long long int value = stoull(decimalValue);
        bitset<80> binary80Bits(value);

        return binary80Bits.to_string();
    }

    string binaryAddition(string &binary1, string &binary2)
    {
        if (binary1.length() != 80 || binary2.length() != 80)
        {
            return "Input length mismatch";
            ready = false;
        }

        string result;
        int carry = 0;

        for (int i = 79; i >= 0; i--)
        {
            int bit1 = binary1[i] - '0';
            int bit2 = binary2[i] - '0';

            int sum = bit1 + bit2 + carry;
            carry = sum / 2;
            result.insert(0, 1, '0' + (sum % 2));
        }

        if (carry > 0)
        {
            result.insert(0, 1, '1');
        }

        return result;
    }

    string ConvertToBin(string Val)
    {
        vector<string> intermediateValues;

        string Remainder = Val;
        string currBinary;
        while (isGreaterThanMaxValue(Val))
        {
            Remainder = subtractDecimalStrings(Val, MAXVAL_Str);
            currBinary = decimalToBinary80Bits(MAXVAL_Str);

            Val = Remainder;
            intermediateValues.push_back(currBinary);
        }
        currBinary = decimalToBinary80Bits(Remainder);
        intermediateValues.push_back(currBinary);

        string binAddRes;
        bitset<80> binary80BitsRes(binAddRes);
        binAddRes = binary80BitsRes.to_string();

        for (int i = 0; i < intermediateValues.size(); i++)
        {
            binAddRes = binaryAddition(binAddRes, intermediateValues[i]);
        }
        return binAddRes;
    }

    string generateKeyStream(unsigned long long plainTxtLen)
    {
        string keyStream = "";
        int currOutput;
        int t1;
        int t2;
        int t3;

        int newT1;
        int newT2;
        int newT3;

        for (int i = 0; i < plainTxtLen; i++)
        {
            t1 = (int)RegA.getBitAt(27) ^ (int)RegA.getBitAt(0);
            t2 = (int)RegB.getBitAt(15) ^ (int)RegB.getBitAt(0);
            t3 = (int)RegC.getBitAt(45) ^ (int)RegC.getBitAt(0);
            currOutput = t1 ^ t2 ^ t3;
            keyStream = to_string(currOutput) + keyStream;
            newT1 = t1 ^ ((int)RegA.getBitAt(2) & (int)RegA.getBitAt(1)) ^ (int)RegB.getBitAt(6);
            newT2 = t2 ^ ((int)RegB.getBitAt(2) & (int)RegB.getBitAt(1)) ^ (int)RegC.getBitAt(24);
            newT3 = t3 ^ ((int)RegC.getBitAt(2) & (int)RegC.getBitAt(1)) ^ (int)RegA.getBitAt(24);
            shiftToRight(newT1, newT2, newT3);
        }
        return keyStream;
    }

    void shiftToRight(int newT1, int newT2, int newT3)
    {
        RegA = RegA.shiftToRight(1);
        RegB = RegB.shiftToRight(1);
        RegC = RegC.shiftToRight(1);

        if (newT3 == 1)
        {
            RegA.setBitAt(92);
        }
        if (newT1 == 1)
        {
            RegB.setBitAt(83);
        }
        if (newT2 == 1)
        {
            RegC.setBitAt(110);
        }
    }
};

int main()
{
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
    if (myTri.ready)
    {
        cout << "\nKeyStream: " << myTri.generateKeyStream(txtsize) << endl;
    }
    return (0);
}
