#include <LinkedList.cpp>
#include <MyMap.cpp>
#include <iostream>
#include <bitset>

//  Shannon - Fano encoder class
class SFCoder
{
    public:
        SFCoder(const std::string& originalText);
        ~SFCoder();

        void print_ftable();
        float compression_ratio();
        std::string get_encoded();
        std::string get_decoded();
        int get_orsize();
        int get_ensize();
        

    private:
        char* chars;
        int* frequency;
        size_t alphabetSize;
        MyMap<char, int> mapOfChars;

        int originalTextLength;
        int originalSize = 0;
        int encodedSize = 0;

        std::string* encodeKey;
        std::string* encodedText;
        std::string* decodedText;

        void quickSort(int* frequency, char* chars, size_t size);
        void createEncoding(int begin, int end);
        void encodeOriginalText(const std::string& originalText);
        void decodeEncodedText();
};


SFCoder::SFCoder(const std::string& originalText)
{
    originalTextLength = originalText.length();
    for (int i = 0; i < originalTextLength; i++) {
        if (!mapOfChars.has(originalText[i])) {
            mapOfChars.insert(originalText[i], 1);
        } else {
            mapOfChars.addToValue(originalText[i], 1);
        }
    }

    LinkedList<char> charsList = mapOfChars.get_keys();
    LinkedList<int> frequencyList = mapOfChars.get_values();

    alphabetSize = charsList.get_size();
    chars = new char[alphabetSize];
    frequency = new int[alphabetSize];
    encodeKey = new std::string[alphabetSize];
    encodedText = new std::string[originalTextLength];
    decodedText = new std::string[originalTextLength];

    for (int i = 0; i < alphabetSize; i++) {
        chars[i] = charsList.at(i);
        frequency[i] = frequencyList.at(i);
    }
    quickSort(frequency, chars, alphabetSize);
    createEncoding(0, alphabetSize-1);
    encodeOriginalText(originalText);
    decodeEncodedText();
}

SFCoder::~SFCoder()
{
    delete[] chars;
    delete[] frequency;
    delete[] encodeKey;
}

void SFCoder::createEncoding(int begin, int end)
{
    if (begin < end) {

		int left = begin, right = end;
		int sumLeft = 0, sumRight = 0;

		while (left <= right)
			if (sumLeft <= sumRight)
				sumLeft += frequency[left++];
			else
				sumRight += frequency[right--];

		for (int i = left; i <= end; i++) encodeKey[i] += "1";
		for (int i = begin; i < left; i++) encodeKey[i] += "0";

		// run the recursive algorithm to left and right subarrays
		createEncoding(left, end);
		createEncoding(begin, left - 1);
	}
}

void SFCoder::encodeOriginalText(const std::string& originalText)
{
    for (int i = 0; i < originalTextLength; i++)
		for (int j = 0; j < alphabetSize; j++)
			if (chars[j] == originalText[i]) {
				encodedText[i] = encodeKey[j];
                encodedSize += encodeKey[j].length();
				originalSize += 8;
				break;
			}
}

void SFCoder::decodeEncodedText()
{
    for (int i = 0; i < originalTextLength; i++)
		for (int j = 0; j < alphabetSize; j++)
			if (encodedText[i] == encodeKey[j]) {
				decodedText[i] = chars[j];
				break;
			}
}


float SFCoder::compression_ratio()
{
    return encodedSize * 1.f / originalSize;
}

void SFCoder::print_ftable()
{
    std::cout << "\nFano table:\n";
	for (int i = 0; i < alphabetSize; i++) 
        std::cout << chars[i] << " : " << frequency[i] << " : " << encodeKey[i] << '\n';
}

std::string SFCoder::get_encoded()
{
    std::string result;
    std::string nextChar;
    for (int i = 0; i < originalTextLength; i++) {
        nextChar = encodedText[i];
        result += nextChar;
    }
    return result;
}

std::string SFCoder::get_decoded()
{
    std::string result;
    std::string nextChar;
    for (int i = 0; i < originalTextLength; i++) {
        nextChar = decodedText[i];
        result += nextChar;
    }
    return result;
}

int SFCoder::get_ensize()
{
    return encodedSize;
}

int SFCoder::get_orsize()
{
    return originalSize;
}

//  Sorts relatively to Key-Value pair
void SFCoder::quickSort(int* frequency, char* chars, size_t size)
{
    if (size > 0) {
        int begin = 0; 
        int end = size - 1; 
        int pivot_value = frequency[rand() % size];
        do {
            while (frequency[begin] > pivot_value)
                begin++;
            while (frequency[end] < pivot_value)
                end--;
            if (begin <= end) {
                std::swap(frequency[begin], frequency[end]);
                std::swap(chars[begin], chars[end]);
                begin++;
                end--;
            }
        } while (begin <= end);

        if (begin < size) quickSort(frequency + begin, chars + begin, size - begin);
        if (end > 0) quickSort(frequency, chars, ++end);
    }
}

