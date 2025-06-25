#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct BMPHeader {
    // ��������� BMP-����� 
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
        std::cerr << "������ �������� �������� �����!" << std::endl;
        return 1;
    }

    if (!outfile.is_open()) {
        std::cerr << "������ �������� ��������� �����!" << std::endl;
        return 1;
    }

    BMPHeader header;
    infile.read(reinterpret_cast<char*>(&header), sizeof(header));

    // �������� ��������� ������
    if (header.format[0]!='B' && header.format[1]!='M') {
        std::cerr << "������ ������ ��������� BMP-�����!" << std::endl;
        return 1;
    }

    // ������ �������� �����������
    int width = header.width;
    int height = header.height;
    int bits_per_pixel = header.bits_per_pixel;
    int padding = (4 - (width * (bits_per_pixel / 8)) % 4) % 4; // ���������� ��������

    // ���������� ������ � ������ � ��� ����
    int new_width = width / 2;
    int new_height = height / 2;
    int new_padding = (4 - (new_width * (bits_per_pixel / 8)) % 4) % 4; // ���������� ��������
    // ���������� ������ � ������ � ���������
    header.width = new_width;
    header.height = new_height;

    // ���������� ������� �����
    int new_file_size = (new_width+new_padding) * new_height;
    header.file_size = new_file_size;

    // ������ ��������� � ����������� � �������� ����
    outfile.write(reinterpret_cast<char*>(&header), sizeof(header));

    std::vector<char> row(width * (bits_per_pixel / 8)); // ������ ������ � ������
    std::vector<char> new_row(new_width * (bits_per_pixel / 8)); // ������ ������ �������� �������� � ������

    
    // ������ � ������ ��������, �������� ������ ������ ������ ������ � �������
    for (int y = 0; y < new_height; y++) {
        infile.seekg(width * (bits_per_pixel / 8) + padding, std::ios::cur); // ������� �������� �����
        infile.read(row.data(), width * (bits_per_pixel / 8)); // ���������� ������
        infile.seekg(padding, std::ios::cur); // ������� ��������
        for (int x = 0; x < new_width * (bits_per_pixel / 8); x += (bits_per_pixel / 8)) {
           new_row[x] = row[x * 2];        // ������� � ������ �������� �������� ������� ������� �������
           new_row[x + 1] = row[x * 2 + 1];
           new_row[x + 2] = row[x * 2 + 2];
        }
        outfile.write(new_row.data(), new_width * (bits_per_pixel / 8)); // ����������� ������� ������� �������
        for (int p = 0; p < new_padding; ++p){
           outfile.put(0); // ������ ������ ��������
        }
        
        
    }
    //�������, ������, �������� ������, ���������
    infile.close();
    outfile.close();

    std::cout << "���� ������� ����" << std::endl;

    return 0;
}
