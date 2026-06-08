#include <iostream>
#include <bitset>
#include <string>
#include <limits>
#include <cmath>
#include <iomanip>
using namespace std;

// Функции для отображения двоичного представления

string toBinary(unsigned int value)
{
    return bitset < 32 >(value).to_string(); //32 бита для unsigned int
}

string toBinary(long double value)
{
    unsigned char* ptr = reinterpret_cast <unsigned char*> (&value);
    string binaryRepresentation;

    // Перебираем каждый байт
    for (int i = sizeof(long double) - 1; i >= 0; --i)
    {
        // Получаем двоичное представление каждого байта
        binaryRepresentation += bitset < 8 >(ptr[i]).to_string();
    }

    return binaryRepresentation;
}


// Функция для циклического сдвига  unsigned int
unsigned int circularShift(unsigned int value, int shift, int startBit, int bitCount, char direction)
{

    // Создадим маску для выделения нужной группы бит
    unsigned int mask = ((1U << bitCount) - 1) << startBit;
    unsigned int groupValue = (value & mask) >> startBit; // Извлекаем группу для сдвига

    shift = shift % bitCount;

    // Выполняем циклический сдвиг в нужную сторону
    if (direction == 'l')
    {
        return (value & ~mask) | (((groupValue << shift) | (groupValue >> (bitCount - shift))) & ((1U << bitCount) - 1)) << startBit;
    }
    else
    {
        return ((value & ~mask) | (((groupValue >> shift) | (groupValue << (bitCount - shift))) & ((1U << bitCount) - 1)) << startBit);
    }
}

// Функция для циклического сдвига long double
long double cyclicShift(long double value, int shift, int startBit, char direction) {
    // Получаем представление в виде 64-битного целого числа
    unsigned long long* ptr = reinterpret_cast <unsigned long long*> (&value);
    unsigned long long mask = 0xFFFFFFFFFFFFFFFF; // Маска для 64 бит

    // Определяем количество бит для сдвига в пределах 64 бит
    shift = shift % 64;

    unsigned long long groupBits = *ptr & mask;

    if (direction == 'r')
    { // Сдвиг вправо
        *ptr = (groupBits >> shift) | ((groupBits & ((1ULL << shift) - 1)) << (64 - shift));
    }
    else
    { // Сдвиг влево
        *ptr = (groupBits << shift) | ((groupBits >> (64 - shift)) & ((1ULL << (64 - shift)) - 1));
    }

    return value;
}

int main()
{
    // Выбор типа данных
    cout << "Enter the type: 1(unsigned int) or 2(long double):  ";
    int fl;
    cin >> fl;

    if (fl == 1)
    {
        // Ввод unsigned int
        unsigned int uintValue;
        cout << "Enter value (unsigned int): ";
        cin >> uintValue;
        cout << "Binary representation (unsigned int): \n" << toBinary(uintValue) << endl;

        // Циклический сдвиг
        int shiftValue, startBit, bitCount;
        char direction;

        cout << "Enter number of bits to shift: ";
        cin >> shiftValue;

        cout << "Enter number of smaller digit of start of group (0-31) ";
        cin >> startBit;

        cout << "Enter direction of shift (l - left, r - right): ";
        cin >> direction;

        if (shiftValue < 0 || startBit < 0 || startBit >= 32 || (direction != 'l' && direction != 'r'))
        {
            cerr << "Incorrect data, try again" << endl;
            return 1;
        }

        bitCount = 32 - startBit;
        if (shiftValue >= bitCount)
        {
            shiftValue = shiftValue % bitCount; // Обрабатываем ситуации, когда сдвиг больше размера группы
        }

        if (direction == 'l')
        {
            unsigned int shiftedValue = circularShift(uintValue, shiftValue, startBit, bitCount, 'l');
            cout << "Binary representation (unsigned int) after left shift: \n" << toBinary(shiftedValue) << endl;
            cout << "Decimal representation (unsigned int) after left shift: " << shiftedValue << endl;
        }

        if (direction == 'r')
        {
            unsigned int shiftedValue = circularShift(uintValue, shiftValue, startBit, bitCount, 'r');
            cout << "Binary representation (unsigned int) after right shift: \n" << toBinary(shiftedValue) << endl;
            cout << "Decimal representation (unsigned int) after right shift: " << shiftedValue << endl;
        }
    }

    else if (fl == 2)
    {
        // Ввод long double
        long double ldValue;
        cout << "Enter value (long double): ";
        cin >> ldValue;
        cout << "Binary representation (long double): \n" << toBinary(ldValue) << endl;

        // Циклический сдвиг
        int shiftValue, startBit, bitCount;
        char direction;
        long double shiftedValue;

        cout << "Enter number of bits to shift: ";
        cin >> shiftValue;

        cout << "Enter number of smaller digit of start of group ";
        cin >> startBit;

        cout << "Enter direction of shift (l - left, r - right): ";
        cin >> direction;

        if (shiftValue < 0 || startBit < 0 || startBit >= sizeof(long double) * 8 || (direction != 'l' && direction != 'r'))
        {
            cerr << "Incorrect data, try again" << endl;
            return 1;
        }

        if (direction == 'l')
        {
            shiftedValue = cyclicShift(ldValue, shiftValue, startBit, 'l');
            cout << "Binary representation (long double) after left shift: \n" << toBinary(shiftedValue) << endl;
            cout << "Decimal representation (long double) after left shift: " << setprecision(20) << shiftedValue << endl;
        }

        if (direction == 'r')
        {
            shiftedValue = cyclicShift(ldValue, shiftValue, startBit, 'r');
            cout << "Binary representation (long double) after right shift: \n" << toBinary(shiftedValue) << endl;
            cout << "Decimal representation (long double) after right shift: " << setprecision(20) << shiftedValue << endl;
        }
    }
    else
    {
        cout << "This type of data doesn't exist ";
    }

    return 0;
}
