#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

class configurations
{

public:
    configurations();
    int EthLineRate;
    int EthCaptureSizeMs;
    int EthMinNumOfIFGsPerPacket;
    string EthDestAddress;
    string EthSourceAddress;
    int EthMaxPacketSize;
    
    void readConfigurations(string configurationFile);
    void printConfigurations();
    int OranSCS;
    int OranMaxNrb;
    int OranNrbPerPacket;
    string OranPayloadType;
    string OranPayload;

private:
    int getNumberAfterEqual(const std::string &str);
    string getAddress(const std::string &str);
    string getString(const std::string &str);
};
#endif
