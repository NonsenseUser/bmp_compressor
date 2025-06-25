#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct BMPHeader {
    // Заголовок BMP-файла 
    char format[2];
    int32_t file_size;
    char skip1[12];
    int32_t width;
    int32_t height;
    char planes[2];
    int32_t bits_per_pixel;
    char skip2[24];
};
#pragma pack(pop)

int main() {
    std::ifstream infile("F:/vm/PK_AOPKI_RPM/X/littleStar.bmp", std::ios::binary);
    std::ofstream outfile("F:/vm/PK_AOPKI_RPM/X/littleStar.bmp", std::ios::binary);

    setlocale(LC_ALL, "ru");

    if (!infile.is_open()) {
        std::cerr << "Ошибка открытия входного файла!" << std::endl;
        return 1;
    }

    if (!outfile.is_open()) {
        std::cerr << "Ошибка открытия выходного файла!" << std::endl;
        return 1;
    }

    BMPHeader header;
    infile.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Проверка считанных данных
    if (header.format[0]!='B' && header.format[1]!='M') {
        std::cerr << "Ошибка чтения заголовка BMP-файла!" << std::endl;
        return 1;
    }

    // Чтение размеров изображения
    int width = header.width;
    int height = header.height;
    int bits_per_pixel = header.bits_per_pixel;
    int padding = (4 - (width * (bits_per_pixel / 8)) % 4) % 4; // Вычисление паддинга

    // Уменьшение ширины и высоты в два раза
    int new_width = width / 2;
    int new_height = height / 2;
    int new_padding = (4 - (new_width * (bits_per_pixel / 8)) % 4) % 4; // Вычисление паддинга
    // Обновление ширины и высоты в заголовке
    header.width = new_width;
    header.height = new_height;

    // Обновление размера файла
    int new_file_size = (new_width+new_padding) * new_height;
    header.file_size = new_file_size;

    // Запись заголовка с изменениями в выходной файл
    outfile.write(reinterpret_cast<char*>(&header), sizeof(header));

    std::vector<char> row(width * (bits_per_pixel / 8)); // Размер строки в байтах
    std::vector<char> new_row(new_width * (bits_per_pixel / 8)); // Размер строки выходной картинки в байтах

    
    // Чтение и запись пикселей, оставляя только каждую вторую строку и столбец
    for (int y = 0; y < new_height; y++) {
        infile.seekg(width * (bits_per_pixel / 8) + padding, std::ios::cur); // Пропуск нечетных строк
        infile.read(row.data(), width * (bits_per_pixel / 8)); // Считывание четных
        infile.seekg(padding, std::ios::cur); // Пропуск паддинга
        for (int x = 0; x < new_width * (bits_per_pixel / 8); x += (bits_per_pixel / 8)) {
           new_row[x] = row[x * 2];        // Перенос в массив выходных значений каждого второго пикселя
           new_row[x + 1] = row[x * 2 + 1];
           new_row[x + 2] = row[x * 2 + 2];
        }
        outfile.write(new_row.data(), new_width * (bits_per_pixel / 8)); // Копирование каждого второго пикселя
        for (int p = 0; p < new_padding; ++p){
           outfile.put(0); // Запись нового паддинга
        }
        
        
    }
    //Массивы, вектор, нечетный размер, структура
    infile.close();
    outfile.close();

    std::cout << "Файл успешно сжат" << std::endl;

    return 0;
}
