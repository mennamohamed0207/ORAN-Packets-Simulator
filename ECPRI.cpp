#include "ECPRI.h"
#include <iomanip>
#include <sstream>
#include <iostream>
ECPRI::ECPRI(ORAN oranpacket)
: ORANPacket(oranpacket)
{
    this->version = "0";
    this->reservedAndContcatention = "0";
    this->message = "00";
    this->pId = "0000";
    this->payload=putIdIntoStringTwoByte((oranpacket.payloadSize)*2);
    // std::cout<<"Size is "<<(oranpacket.payloadSize)*2<<"and hexa is "<<this->payload<<endl;
    seqId++;
    if (seqId == 255)
        seqId = 0;
}
int ECPRI::seqId = 0;

string ECPRI::getECPRI()
{
    return this->version + this->reservedAndContcatention + this->message + this->payload + this->pId+putIdIntoStringTwoByte(seqId)+this->ORANPacket.getORAN();
}

string ECPRI::putIdIntoStringTwoByte(int id)
{
    if (id < 0 || id > 65535)
    {
        throw std::out_of_range("Number must be between 0 and 65535");
    }
    std::stringstream stream;
    stream << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << id;
    return stream.str();
}