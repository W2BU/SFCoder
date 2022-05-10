#include <SFCoder.cpp>

int main() {
    std::string text;
    std::cout << "Enter text:\n";
    std::getline(std::cin, text);

    SFCoder mySFCode(text);
    std::cout << "\nOriginal size: " << mySFCode.get_orsize() << '\n';
    std::cout << "\nCompressed size: " << mySFCode.get_ensize() << '\n';
    std::cout << "\nCompression ratio: " << mySFCode.compression_ratio() << '\n';
    std::cout << "\nEncoded string:\n" << mySFCode.get_encoded();
    std::cout << "\nDecoded string:\n" << mySFCode.get_decoded();
    mySFCode.print_ftable();
    system("pause");
}