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
        return;
    }
    std::string line;
    int sample=0;
    while (sample>this->payloadSize)
    {
        
        iqsamples+=iqSamplesToHexa(line)
        sample++;
    }
    return iqsamples;
    // file.close();


}
string ORAN::iqSamplesToHexa(string line)
{
    int c=0;
    string i="";
    string q="";
    while(line[c]!=" ")
    {
        i+=line[c];
        c++;
    }
    c++;
    q=line.substr(c,line.length()-1);
    int i_integer=stoi(i);
    std::stringstream stream;
    stream << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << i_integer;
    i=stream.str();
    int q_integer=stoi(q);
    std::stringstream streamq;
    stream << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << q_integer;
    q=streamq.str();
    return i+q;

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
