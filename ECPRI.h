#include <string>
#include "ORAN.h"
using namespace std;
class ECPRI{
    public:
        // Octet 1
        string version;
        string reserved;
        string contcatention;
        //Octet 2
        string message;
        //Octet 3-4
        string payload;
        //Octet 5-6
        string pId;
        //Octet 7
        string seqId;
        //Octet 9-16
        ORAN ORANPacket;

};