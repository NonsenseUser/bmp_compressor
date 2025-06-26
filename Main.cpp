#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct BMPHeader {
    // Structure of BMP header
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
    //Insert your input and output paths
    std::ifstream infile("F:/vm/PK_AOPKI_RPM/X/littleStar.bmp", std::ios::binary);
    std::ofstream outfile("F:/vm/PK_AOPKI_RPM/X/littleStar.bmp", std::ios::binary);

    setlocale(LC_ALL, "ru");

    if (!infile.is_open()) {
        std::cerr << "Input file cannot be opened" << std::endl;
        return 1;
    }

    if (!outfile.is_open()) {
        std::cerr << "Output file cannot be opened" << std::endl;
        return 1;
    }

    BMPHeader header;
    infile.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Ïðîâåðêà ñ÷èòàííûõ äàííûõ
    if (header.format[0]!='B' && header.format[1]!='M') {
        std::cerr << "Input file must have BMP format" << std::endl;
        return 1;
    }

   
    int width = header.width;
    int height = header.height;
    int bits_per_pixel = header.bits_per_pixel;
    int padding = (4 - (width * (bits_per_pixel / 8)) % 4) % 4; 
    
    int new_width = width / 2;
    int new_height = height / 2;
    int new_padding = (4 - (new_width * (bits_per_pixel / 8)) % 4) % 4; 
    
    header.width = new_width;
    header.height = new_height;

    int new_file_size = (new_width+new_padding) * new_height;
    header.file_size = new_file_size;

    outfile.write(reinterpret_cast<char*>(&header), sizeof(header));

    std::vector<char> row(width * (bits_per_pixel / 8)); 
    std::vector<char> new_row(new_width * (bits_per_pixel / 8));

    
    for (int y = 0; y < new_height; y++) {
        infile.seekg(width * (bits_per_pixel / 8) + padding, std::ios::cur); //A row and its padding are skipped
        infile.read(row.data(), width * (bits_per_pixel / 8)); // Next row is read
        infile.seekg(padding, std::ios::cur); // Padding is skipped
        for (int x = 0; x < new_width * (bits_per_pixel / 8); x += (bits_per_pixel / 8)) {
           new_row[x] = row[x * 2];        // Every second pixels of row is written into new compressed row
           new_row[x + 1] = row[x * 2 + 1];
           new_row[x + 2] = row[x * 2 + 2];
        }
        outfile.write(new_row.data(), new_width * (bits_per_pixel / 8)); // Write compressed row into new image
        for (int p = 0; p < new_padding; ++p){
           outfile.put(0); // put zeros at the end of the row for padding
        }
        
        
    }
    infile.close();
    outfile.close();

    std::cout << "File has been successfully compressed" << std::endl;

    return 0;
}
