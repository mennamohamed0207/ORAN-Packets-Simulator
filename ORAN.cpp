#include "ORAN.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>


ORAN::ORAN(int frameId, int subframeId, int slotId, int symbolId,string IQSamplesFileName,int payloadSize,long long samplesIndex)
{
    this->dataDirectionAndpayloadVersionAndfilterIndex = "00";
    this->frameId = putIdIntoStringOneByte(frameId);
    this->symbolId=putIdIntoStringSixbits(symbolId);
    this->subframeId=putIdIntoStringOneDigit(subframeId);
    this->slotId=putIdIntoStringSixbits(slotId);
    this->payloadSize=payloadSize/2;
    this->iqSamples=fillIQSamples(IQSamplesFileName,samplesIndex);
    this->sectionId="00";
    this->restOfSectionId+this->rb+this->symInc+this->startPrbu="00";
    this->restOfStartPrbu="00";
    this->numPrbu="00";

}
string ORAN::fillIQSamples(string IQSamplesFileName, long long samplesIndex)
{
    string iqsamples = "";
    std::ifstream file(IQSamplesFileName);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << IQSamplesFileName << std::endl;
        return "Error";
    }

    std::string line;
    int sample = 0;

    // Skip to the starting index (samplesIndex)
    for (int i = 0; i < samplesIndex - 1 && std::getline(file, line); ++i) {
        // Skip the lines until we reach the sample index
    }

    // Loop to fill samples until payloadSize is reached
    while (sample < this->payloadSize)
    {
        // If the end of the file is reached, loop back to the beginning
        if (!std::getline(file, line)) {
            // Reset the file to the beginning
            file.clear(); // Clear eof and other flags
            file.seekg(0, std::ios::beg); // Move the cursor to the beginning of the file

            // Read the first line again after resetting
            if (!std::getline(file, line)) {
                std::cerr << "Error: Could not read the file again after resetting." << std::endl;
                return "Error";
            }
        }

        // Process the IQ sample line and convert to hexadecimal
        iqsamples += iqSamplesToHexa(line);

        sample++; // Increment the sample count
    }

    return iqsamples;
}

string ORAN::iqSamplesToHexa(string line)
{
    int c = 0;
    string i = "";
    string q = "";
    
    // Parse the I part from the input string
    while(line[c] != ' ')
    {
        i += line[c];
        c++;
    }
    c++;
    
    // Parse the Q part from the input string
    q = line.substr(c, line.length() - c);

    // Convert I to integer and handle two's complement for 16-bit signed value
    int i_integer = stoi(i);
    if (i_integer < 0)
        i_integer = (1 << 16) + i_integer;  // Two's complement for negative numbers

    std::stringstream stream;
    stream << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << (i_integer & 0xFFFF);  // Mask to 16-bit

    i = stream.str();

    // Convert Q to integer and handle two's complement for 16-bit signed value
    int q_integer = stoi(q);
    if (q_integer < 0)
        q_integer = (1 << 16) + q_integer;  // Two's complement for negative numbers

    std::stringstream streamq;
    streamq << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << (q_integer & 0xFFFF);  // Mask to 16-bit

    q = streamq.str();

    return i + q;
}


string ORAN::getORAN()
{
    return this->dataDirectionAndpayloadVersionAndfilterIndex + 
   (this->frameId) + 
     (this->subframeId) + 
    (this->slotId)  + 
    (this->symbolId)+ this->sectionId + this->restOfSectionId + this->rb + this->symInc + this->startPrbu + this->restOfStartPrbu + this->numPrbu + this->iqSamples;
}
string ORAN::putIdIntoStringOneByte(int id)
{
    if (id < 0 || id > 255)
    {
        throw std::out_of_range("Number must be between 0 and 255.");
    }
    std::stringstream stream;
    stream << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << id;
    return stream.str();
}
string ORAN::putIdIntoStringSixbits(int id)
{
    if(id < 0 || id > 63)
    {
        throw std::out_of_range("Number must be between 0 and 63.");
    }
    std::stringstream stream;
    stream << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << id;
    return stream.str();
}
string ORAN::putIdIntoStringOneDigit(int id)
{
    if(id < 0 || id > 15)
    {
        throw std::out_of_range("Number must be between 0 and 15.");
    }
    std::stringstream stream;
    stream << std::setw(1) << std::setfill('0') << std::hex << std::uppercase << id;
    return stream.str();
}
