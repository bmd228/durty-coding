// durty-coding.cpp: определяет точку входа для приложения.
//

#include "durty-coding.h"
#include <iostream>
#include <fstream>
#include <iconv.h>
#include <cstring>
#include <cstdlib>
using namespace std;
unsigned char* readFile(const std::string& filePath, std::size_t& fileSize) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate); // Открываем файл в бинарном режиме
    if (!file) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return nullptr;
    }

    fileSize = file.tellg(); // Определяем размер файла
    file.seekg(0, std::ios::beg); // Возвращаемся к началу файла

    // Выделяем память под данные файла
    unsigned char*  buffer = new unsigned char[fileSize];
    if (!file.read(reinterpret_cast<char*>(buffer), fileSize)) {
        std::cerr << "Ошибка чтения файла!" << std::endl;
        delete[] buffer;
        return nullptr;
    }

    file.close(); // Закрываем файл
    return buffer; // Возвращаем указатель на данные
}

// Функция для сохранения данных в файл
bool writeFile(const std::string& filePath, char* data, std::size_t fileSize) {
    std::ofstream file(filePath, std::ios::binary); // Открываем файл в бинарном режиме
    if (!file) {
        std::cerr << "Ошибка создания файла!" << std::endl;
        return false;
    }

    if (!file.write(reinterpret_cast<const char*>(data), fileSize)) {
        std::cerr << "Ошибка записи в файл!" << std::endl;
        return false;
    }

    file.close(); // Закрываем файл
    return true;
}
char* convert(const char* fromCharset, const char* toCharset, const char* input,size_t& inBytesLeft) {
    // Создаем дескриптор iconv

    iconv_t conv = iconv_open(toCharset, fromCharset );
    if (conv == (iconv_t)-1) {
        std::cerr << "Error: cannot open iconv descriptor" << std::endl;
        return nullptr;
    }

    // Входной и выходной буферы
   // size_t inBytesLeft = strlen(input);
    
    size_t outBytesLeft = inBytesLeft * 4; // Задаем достаточный размер для выходного буфера
    size_t source_size = outBytesLeft;
    char* inBuf = const_cast<char*>(input);
    char* outBuf = (char*)malloc(outBytesLeft);
    char* outBufStart = outBuf; // Сохраняем начальное положение выходного буфера

    // Выполняем конвертацию
    if (iconv(conv, &inBuf, &inBytesLeft, &outBuf, &outBytesLeft) == (size_t)-1) {
        std::cerr << "Error: iconv conversion failed" << std::endl;
        iconv_close(conv);
        free(outBufStart);
        return nullptr;
    }

    // Выводим результат
    std::cout << "Converted string: " << outBufStart << std::endl;

    // Освобождаем ресурсы
    iconv_close(conv);
    inBytesLeft = source_size-outBytesLeft;
    return outBufStart;
    //free(outBufStart);
}
int main()
{
    std::string inputFilePath = "D:\\test\\1.txt";
    std::string outputFilePath = "D:\\test\\2.txt";

    std::size_t fileSize = 0;
    unsigned char* data = readFile(inputFilePath, fileSize);
    auto data_out=convert("CP1251", "UTF-8//TRANSLIT", reinterpret_cast<const char*>(data),fileSize);
    if (!writeFile(outputFilePath, data_out, fileSize))
        cout << "Err write" << endl;
    delete[] data;
	return 0;
}
