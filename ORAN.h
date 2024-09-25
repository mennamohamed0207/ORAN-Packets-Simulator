#include <string>
using namespace std;
class ORAN{
    public:
    //Octet 9
    string dataDirection;
    string payloadVersion;
    string filterIndex;
    //Octet 10
    string  frameId;
    //Octet 11
    string  subframeId;
    string slotId;
    //Octet 12
    string symbolId;
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
    int payloadSize;

    // ORAN();
    ORAN(int frameId, int subframeId, int slotId, int symbolId,string IQSamplesFileName,int payloadSize,long long samplesIndex);
    string getORAN();
    string putIdIntoStringOneByte(int id);
    string putIdIntoStringSixbits(int id );
    string putIdIntoStringOneDigit(int id );
    string fillIQSamples(string IQSamplesFileName,long long samplesIndex);
    string iqSamplesToHexa(string line);



};