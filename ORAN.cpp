#include "ORAN.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>


ORAN::ORAN(int frameId, int subframeId, int slotId, int symbolId,string IQSamplesFileName,int payloadSize)
{
    this->dataDirection + this->payloadVersion + this->filterIndex = "00";
    this->frameId = putIdIntoStringOneByte(frameId);
    this->symbolId=putIdIntoStringSixbits(symbolId);
    this->subframeId=putIdIntoStringOneDigit(subframeId);
    this->slotId=putIdIntoStringSixbits(slotId);
    this->payloadSize=payloadSize;
    this->iqSamples=fillIQSamples(IQSamplesFileName);
}
string ORAN::fillIQSamples(string IQSamplesFileName)
{
    string iqsamples="";
    std::ifstream file(IQSamplesFileName);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file " << IQSamplesFileName << std::endl;
        return "Error";
    }
    std::string line;
    int sample=0;
    while (sample<this->payloadSize&&std::getline(file, line))
    {
        
        iqsamples+=iqSamplesToHexa(line);

        sample++;
    }
    return iqsamples;
    // file.close();


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
    return this->dataDirection + this->payloadVersion + this->filterIndex + 
    (this->frameId) + 
    (this->subframeId) + 
    (this->slotId)  + 
    (this->symbolId) + this->sectionId + this->restOfSectionId + this->rb + this->symInc + this->startPrbu + this->restOfStartPrbu + this->numPrbu + this->iqSamples;
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
    if(id < 0 || id > 9)
    {
        throw std::out_of_range("Number must be between 0 and 9.");
    }
    std::stringstream stream;
    stream << std::setw(1) << std::setfill('0') << std::hex << std::uppercase << id;
    return stream.str();
}
