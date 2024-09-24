#include <string>
using namespace std;
class ORAN{
    public:
    //Octet 9
    string dataDirection;
    string payloadVersion;
    string filterIndex;
    //Octet 10
    int  frameId;
    //Octet 11
    int  subframeId;
    int slotId;
    //Octet 12
    int restOfSlotId;
    int symbolId;
    //Octet 13
    string sectionId;
    //Octet 14
    string restOfSectionId;
    string rb;
    string symInc;
    string startPrbu;
    //Octet 15
    string restOfStartPrbu;
    //Octet 16
    string numPrbu;
    //rest of packet
    string iqSamples;

    ORAN();
    ORAN(int frameId, int subframeId, int slotId, int symbolId);
    string getORAN();



};