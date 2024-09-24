#include "ORAN.h"
#include <iomanip>
#include <sstream>

ORAN::ORAN(int frameId, int subframeId, int slotId, int symbolId,string IQSamplesFileName)
{
    this->dataDirection + this->payloadVersion + this->filterIndex = "00";
    this->frameId = putIdIntoStringOneByte(frameId);
    this->symbolId=putIdIntoStringSixbits(symbolId);
    this->subframeId=putIdIntoStringOneDigit(subframeId);
    this->slotId=putIdIntoStringSixbits(slotId);
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
