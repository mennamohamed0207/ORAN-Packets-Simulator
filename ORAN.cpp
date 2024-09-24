#include "ORAN.h"

ORAN::ORAN()
{
    this->dataDirection + this->payloadVersion + this->filterIndex = "00";
}
string ORAN::getORAN()
{
    return this->dataDirection + this->payloadVersion + this->filterIndex + to_string(this->frameId) + to_string(this->subframeId) + to_string(this->slotId) + to_string(this->restOfSlotId) + to_string(this->symbolId) + this->sectionId + this->restOfSectionId + this->rb + this->symInc + this->startPrbu + this->restOfStartPrbu + this->numPrbu + this->iqSamples;
}