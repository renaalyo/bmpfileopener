#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

#pragma pack(push,1)
struct BMPFileHeader {
    unsigned short type{ 0x4D42 };
    unsigned int fileSize{ 0 };
    unsigned short reserved1{ 0 };
    unsigned short reserved2{ 0 };
    unsigned int offset{ 0 };
};


struct BMPInfoHeader {
    unsigned int size{ 0 };
    int width{ 0 };
    int height{ 0 };
    unsigned short planes{ 1 };
    unsigned short bitCount{ 0 };
    unsigned int compression{ 0 };
    unsigned int imageSize{ 0 };
    int xResolution{ 0 };
    int yResolution{ 0 };
    unsigned int colorsUsed{ 0 };
    unsigned int importantColors{ 0 };
};
#pragma pack(pop)

class BMPImage {
private:
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    std::vector<unsigned char> imageData;
    std::ifstream file;
       
public:
    BMPImage(){}

    void openBMP(const std::string& filename) {
        file.open(filename, std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Error: Unable to open file: " + filename);
        }

        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));

        if (fileHeader.type != 0x4D42) {
            throw std::runtime_error("Error: File is not a BMP file.");
        }

        file.seekg(fileHeader.offset - sizeof(BMPInfoHeader));
      
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

        if (infoHeader.bitCount != 24 && infoHeader.bitCount != 32) {
            throw std::runtime_error("Error: BMP file must be 24 or 32 bit.");
        }
        
        if (infoHeader.width <= 0 || infoHeader.height <= 0) {
            throw std::runtime_error("Error: Invalid image dimensions. Width and height must be greater than 0.");
        }
        
        int bytesPerRow = ((infoHeader.width * infoHeader.bitCount) / 8 + 3) & ~3;
        imageData.resize(infoHeader.height * bytesPerRow);
        
        file.read(reinterpret_cast<char*>(imageData.data()), imageData.size());
    }

    void closeBMP() {
        if (file.is_open()) {
            file.close();
        }
    }

    int getWidth() const {
        return infoHeader.width;
    }

    int getHeight() const {
        return infoHeader.height;
    }

    std::vector<unsigned char> getPixel(int x, int y) const {
        if (x >= 0 && x < getWidth() && y >= 0 && y < getHeight()) {
            
            //int bytesPerRow = (getWidth() * 3 + 3) & ~3;
            int bytesPerRow = ((infoHeader.width * infoHeader.bitCount) / 8 + 3) & ~3;

         
            //int pixelIndex = (y * bytesPerRow) + (x * 3);
            int pixelIndex = (y * bytesPerRow) + (x * (infoHeader.bitCount / 8));

            if (infoHeader.bitCount == 24) {
                return { imageData[pixelIndex], imageData[pixelIndex + 1], imageData[pixelIndex + 2] };
            }
            else if (infoHeader.bitCount == 32) {
                return { imageData[pixelIndex], imageData[pixelIndex + 1], imageData[pixelIndex + 2], imageData[pixelIndex + 3] };
            }
            else {
                std::cerr << "Error: Unsupported bit depth." << "\n";
                return { 0, 0, 0 }; 
            }
        }
        else {
            std::cerr << "Error: Pixel coordinates out of bounds." << "\n";
            return { 0, 0, 0 }; 
        }
    }

    
    void displayBMP() const {
        if (getWidth() <= 0 || getHeight() <= 0) {
            throw std::runtime_error("Error: Image cannot be displayed. Width or height is 0 or less.");
        }

        //for(int y = 0; y < getHeight(); ++y) mirrored images
        for (int y = getHeight() - 1; y >=0 ; --y) {

            for (int x = 0; x < getWidth(); ++x) {

                std::vector<unsigned char> pixel = getPixel(x, y);

                if (pixel[0] + pixel[1] + pixel[2] < 384) { 
                    std::cout << "# ";
                }
                else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
    }

    ~BMPImage() {
        closeBMP();
    }
};

//int main() {
//    std::string filename;
//    std::cout << "Enter the BMP file name: ";
//    std::cin >> filename;
//
//    try {
//        BMPImage image;
//
//        image.openBMP(filename);
//
//        image.displayBMP();
//
//    }
//    catch (const std::runtime_error& e) {
//        std::cerr << e.what() << "\n";
//        return 1;
//    }
//
//    return 0;
//}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: drawBmp.exe <bmp_file_path>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    try {
        // Create a BMPImage object
        BMPImage image;

        // Open the BMP file
        image.openBMP(filename);

        // Display the image in the console
        image.displayBMP();

    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1; // Exit with an error code
    }

    return 0;
}