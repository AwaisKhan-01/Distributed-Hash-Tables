#pragma once
#pragma once
#include "SHA1.h"

using namespace std;
const string calculateStringHash(string input) {
    SHA1 checksum;
    checksum.update(input);
    const string hash = checksum.final();

    return hash;
}
const string calculateTextHash(string filePath) {
    ifstream fileContent(filePath, ios::binary);
    if (!fileContent.is_open()) {
        cout << "Error opening file: " << filePath << endl;
        return "";
    }
    string input;
    while (!fileContent.eof()) {
        fileContent >> input;
    }
    fileContent.close();
    SHA1 checksum;
    checksum.update(input);
    const string hash = checksum.final();
    return hash;
}

const string calculateAudioHash(const std::string& fileName) {
    std::ifstream audioFile(fileName, std::ios::binary);
    if (!audioFile.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return "";
    }

    audioFile.seekg(0, std::ios::end);
    int fileSize = audioFile.tellg();
    audioFile.seekg(0, std::ios::beg);

    char* buffer = new char[fileSize];

    audioFile.read(buffer, fileSize);

    audioFile.close();

    SHA1 checksum;
    checksum.update(buffer);
    const string hash = checksum.final();

    return hash;
}

