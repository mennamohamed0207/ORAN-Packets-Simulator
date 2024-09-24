#include "ECPRI.h"
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
    // this->ORANPacket = oranpacket;
}
int ECPRI::seqId = 0;

string ECPRI::getECPRI()
{
    return this->version + this->reserved + this->contcatention + this->message + this->payload + this->pId+to_string(seqId)+this->ORANPacket.getORAN();
}