#include <cstdio>
#include <vector>
#include <complex>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>

typedef std::complex<double> Number;

void FFT(std::vector<Number>& polynomial, bool forward = true) {
    auto len = polynomial.size(), half = len >> 1u;
    if (len == 1) return;

    std::vector<Number> evenPolynomial(half), unevenPolynomial(half);
    for (size_t i = 0; i < half; ++i) {
        evenPolynomial[i] = polynomial[2 * i];
        unevenPolynomial[i] = polynomial[2 * i + 1];
    }
    FFT(evenPolynomial, forward);
    FFT(unevenPolynomial, forward);

    auto angle_0 = 2 * M_PI / len;
    if (!forward) angle_0 = -angle_0;
    Number pow(1), mainRoot(cos(angle_0), sin(angle_0));
    for (size_t i = 0; i < half; ++i) {
        polynomial[i] = evenPolynomial[i] + pow * unevenPolynomial[i];
        polynomial[i + half] = evenPolynomial[i] - pow * unevenPolynomial[i];
        if (!forward) {
            polynomial[i] /= 2;
            polynomial[i + half] /= 2;
        }
        pow *= mainRoot;
    }
}

struct WAVHEADER {
    char chunkId[4];
    unsigned int chunkSize;
    char format[4];
    char subchunk1Id[4];
    unsigned int subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char subchunk2Id[4];
    unsigned int subchunk2Size;
};

int main() {
    // Interacting with user
    auto audioPATH = new char[4096], resultPATH = new char[4096];  // PATH_MAX = 4096
    printf("Please enter absolute path to the audio file:\n");
    scanf("%s", audioPATH);
    printf("Please enter absolute path to the result file:\n");
    scanf("%s", resultPATH);
    int compressionCoefficient;
    printf("Please enter compression coefficient in percents:\n");
    scanf("%d", &compressionCoefficient);

    // Opening the audio file
    auto fileIn = fopen(audioPATH, "r");
    delete[] audioPATH;
    if (!fileIn) {
        printf("ERROR 0: Failed to open file.\n");
        return 0;
    }

    // Getting header of audio file
    WAVHEADER header{};
    fread(&header, sizeof(WAVHEADER), 1, fileIn);

    // Getting main data of audio file
    auto rawData = new unsigned char[header.subchunk2Size];
    fread(rawData, header.subchunk2Size, 1, fileIn);
    fclose(fileIn);

    // dataSize is the next power 2 after numSamples (needed to FFT)
    size_t numSamples = header.subchunk2Size / header.blockAlign,
           dataSize   = 1;
    while (dataSize < numSamples) dataSize <<= 1u;

    // Converting blocks of header.blockAlign of bytes into size_t
    std::vector<Number> data(dataSize, 0);
    for (size_t i = 0; i < numSamples; ++i) {
        size_t num = 0, pow = 1;
        for (unsigned short j = 0; j < header.blockAlign; ++j) {
            num += pow * rawData[i * header.blockAlign + j];
            pow <<= 8u;
        }
        data[i] = num;
    }
    delete[] rawData;

    // Main part of program, compressing
    FFT(data);
    size_t mainPartLen = (100 - compressionCoefficient) * dataSize / 100;
    for (size_t i = mainPartLen; i < data.size(); ++i)
        data[i] = 0;
    FFT(data, false);

    // Converting back to bigEndian
    auto resultData = new unsigned char[header.subchunk2Size];
    for (size_t i = 0; i < data.size(); ++i) {
        size_t num = data[i].real();
        for (unsigned short j = 0; j < header.blockAlign; ++j) {
            resultData[i * header.blockAlign + j] = num % 256;
            num >>= 8u;
        }
    }

    // Opening of resulting file
    int fileOut = open(resultPATH, O_WRONLY | O_CREAT, 0640);
    if(fileOut == -1) {
        printf("ERROR 4: Cannot create or open resulting file.");
        return 0;
    }

    // Writing all the data in fileOut
    int written = 0, currentlyWrote;
    while ((currentlyWrote = write(fileOut, &header + written, sizeof(WAVHEADER) - written)) > 0)
        written += currentlyWrote;
    written = 0;
    while ((currentlyWrote = write(fileOut, resultData + written, header.subchunk2Size - written)) > 0)
        written += currentlyWrote;

    close(fileOut);
    printf("File successfully compressed.\n");
    return 0;
}