#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include <string>

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

class WAVFILE {
public:
    WAVFILE(FILE* file) {
        // Getting header of audio file
        fread(&header, sizeof(WAVHEADER), 1, file);

        // Getting main data of audio file
        rawData = new unsigned char[header.subchunk2Size];
        fread(rawData, header.subchunk2Size, 1, file);
        fclose(file);
    }

    ~WAVFILE() {
        delete[] rawData;
    }

    void compress(int compressionCoefficient) {
        // Main part of program, compressing
        std::vector<Number> data;
        getReadyToFFT(data);
        FFT(data);
        size_t mainPartLen = (100 - compressionCoefficient) * dataSize / 100;
        for (size_t i = mainPartLen; i < data.size(); ++i)
            data[i] = 0;
        FFT(data, false);

        // Converting back to bigEndian
        for (size_t i = 0; i < numSamples; ++i) {
            size_t num = data[i].real();
            for (unsigned short j = 0; j < header.blockAlign; ++j) {
                rawData[i * header.blockAlign + j] = num % 256;
                num >>= 8u;
            }
        }
    }

    void writeAudiofile(int fd) {
        // Writing all the data in fileOut
        int written = 0, currentlyWrote;
        while ((currentlyWrote = write(fd, &header + written, sizeof(WAVHEADER) - written)) > 0)
            written += currentlyWrote;
        written = 0;
        while ((currentlyWrote = write(fd, rawData + written, header.subchunk2Size - written)) > 0)
            written += currentlyWrote;

        close(fd);
    }

private:
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

    WAVHEADER header;
    unsigned char* rawData;
    size_t numSamples, dataSize;

    void getReadyToFFT(std::vector<Number>& data) {
        // dataSize is the next power 2 after numSamples (needed to FFT)
        numSamples = header.subchunk2Size / header.blockAlign;
        dataSize   = 1;
        while (dataSize < numSamples) dataSize <<= 1u;

        // Converting blocks of header.blockAlign of bytes into size_t
        data.resize(dataSize, 0);
        for (size_t i = 0; i < numSamples; ++i) {
            size_t num = 0, pow = 1;
            for (unsigned short j = 0; j < header.blockAlign; ++j) {
                num += pow * rawData[i * header.blockAlign + j];
                pow <<= 8u;
            }
            data[i] = num;
        }
    }
};

int main() {
    // Interacting with user
    std::string audioPATH, resultPATH;
    std::cout << "Please enter absolute path to the audio file:\n";
    std::cin >> audioPATH;
    std::cout << "Please enter absolute path to the result file:\n";
    std::cin >> resultPATH;
    int compressionCoefficient;
    std::cout << "Please enter compression coefficient in percents:\n";
    std::cin >> compressionCoefficient;

    WAVFILE file(fopen(audioPATH.c_str(), "r"));
    file.compress(compressionCoefficient);
    file.writeAudiofile(open(resultPATH.c_str(), O_WRONLY | O_CREAT, 0640));

    printf("File successfully compressed.\n");
    return 0;
}