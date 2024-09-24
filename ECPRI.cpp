#include "ECPRI.h"
#include <iomanip>
#include <sstream>
ECPRI::ECPRI(ORAN oranpacket)
: ORANPacket(oranpacket)
{
    this->version = "0";
    this->reserved + this->contcatention = "0";
    this->message = "00";
    this->pId = "0000";
    seqId++;
    if (seqId == 255)
        seqId = 0;
}
int ECPRI::seqId = 0;

string ECPRI::getECPRI()
{
    return this->version + this->reserved + this->contcatention + this->message + this->payload + this->pId+putIdIntoStringTwoByte(seqId)+this->ORANPacket.getORAN();
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