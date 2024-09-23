#include "ECPRI.h"
ECPRI::ECPRI()
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