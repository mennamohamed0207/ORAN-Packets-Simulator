#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;
#include "Program.h"

void Program::generatePackets(const std::string &outputFile)
{
    std::ofstream out(outputFile);

    int numberOfFrames = config.EthCaptureSizeMs / 10;
    int packetsPerSymbol = ceil(config.OranMaxNrb * 1.0 / config.OranNrbPerPacket);
    int symbolPerSlot = 14; // Assum normal cyclic prefix
    int packetsPerSlot = packetsPerSymbol * symbolPerSlot;
    int mu = log2(config.OranSCS / 15.0);
    int slotsPerSubframe = pow(2, mu);
    int packetsPerSecond = packetsPerSlot * slotsPerSubframe * (1000 / config.EthCaptureSizeMs);
    int bitsPerPacket = packetsPerSecond * 12 * config.OranNrbPerPacket * 16 * 2;
    int bytesPerPacket = bitsPerPacket / 8;
    int packetsPerSubframe = packetsPerSlot * slotsPerSubframe;
    int packetsPerFrame = packetsPerSubframe * 10;
    if (true||bytesPerPacket < config.EthMaxPacketSize) // No fragmentation
    {
        cout << "No fragmentation " << endl;
        cout << bytesPerPacket << endl;
        int numberOfPackets = packetsPerSecond * (1000 / config.EthCaptureSizeMs);

        int frameId = 0;
        int subframeId = 0;
        int slotId = 0;
        int symbolId = 0;

        for (int i = 0; i < numberOfPackets; ++i)
        {
            string destAddress = (config.EthDestAddress);
            string srcAddress = (config.EthSourceAddress);
            string ethernetType = "AEFE";
            if (i % packetsPerSlot == 0)
            {
                slotId++;
            }
            if (i % packetsPerSymbol == 0)
            {
                symbolId++;
            }
            if (i % packetsPerSubframe == 0)
            {
                subframeId++;
            }
            if (i % packetsPerFrame == 0)
            {
                frameId++;
            }
            if(i==30) break;
            ORAN oran(frameId, subframeId, slotId, symbolId,config.OranPayload);
            ECPRI ecpri(oran);
            Packet p(destAddress, srcAddress, "AEFE", ecpri.getECPRI());
            out<<p.getPacket() << endl;
        }
    }
    else
    { // Fragmentation
        cout << "Fragmentation" << endl;
        cout << bytesPerPacket << endl;
    }
    out.close();
}

long long Program::calculateNumberOfBursts()
{

    return -1;
}
long long Program::handleIFGs()
{

    return 0;
}
bool Program::isAligned(int packetSize)
{
    return packetSize % 4 == 0;
}

void Program::addIFGs(Packet &packet)
{

    int padding = 4 - (packet.getPacket().size() % 4);
    // string packet=packet.getPacket();
    for (int i = 0; i < padding; ++i)
    {
        packet.getPacket() += "07";
    }
}
void Program::dumpPacketsToFile(const std::string &outputFile)
{
    std::ofstream out(outputFile);
    for (Packet &packet : packets)
    {
        out << packet.getPacket() << std::endl;
    }
    out.close();
}

void Program::setConfigurationFile(const std::string &configurationFile)
{

    if (!fileExists(configurationFile))
    {
        throw std::runtime_error("File not found");
    }

    this->configurationFile = configurationFile;
    readConfig();
}

void Program::setOutputFile(const std::string &outputFile)
{
    this->outputFile = outputFile;
}

void Program::setPackets(vector<Packet> &p)
{
    this->packets = p;
}

void Program::readConfig()
{
    config.readConfigurations(configurationFile);
}

bool Program::fileExists(const std::string &fileName)
{
    std::ifstream file(fileName);
    return file.is_open();
}

Program::Program(string configFile, string outputFile)
{
    setConfigurationFile(configFile);
    generatePackets(outputFile);
}
configurations Program::getConfig()
{
    return this->config;
}