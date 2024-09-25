#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;
#include "Program.h"

void Program::generatePackets(const std::string &outputFile)
{
    std::ofstream out(outputFile);

    // int numberOfFrames = config.EthCaptureSizeMs / 10;
    long long packetsPerSymbol = ceil(config.OranMaxNrb * 1.0 / config.OranNrbPerPacket);
    long long symbolPerSlot = 14; // Assum normal cyclic prefix
    long long packetsPerSlot = packetsPerSymbol * symbolPerSlot;
    long long mu = log2(config.OranSCS / 15.0);
    long long slotsPerSubframe = pow(2, mu);
    long long packetsPerSecond = packetsPerSlot * slotsPerSubframe * (1000 / config.EthCaptureSizeMs);
    long long bitsPerPacket = packetsPerSecond * 12 * config.OranNrbPerPacket * 16 * 2;
    long long ethernetHeaderSize = 7 + 1 + 6 + 6 + 2 + 4;
    long long ecpriHeaderSize = 8;
    long long oranHeaderSize = 4;
    long long bytesPerPacket = (bitsPerPacket / 8) + ethernetHeaderSize + ecpriHeaderSize + oranHeaderSize;
    long long packetsPerSubframe = packetsPerSlot * slotsPerSubframe;
    long long packetsPerFrame = packetsPerSubframe * 10;

    if (true || bytesPerPacket < config.EthMaxPacketSize) // No fragmentation
    {
        cout << "No fragmentation " << endl;
        cout << bytesPerPacket << endl;
        int numberOfPackets = packetsPerSecond * (1000 / config.EthCaptureSizeMs);

        int frameId = 0;
        int subframeId = 0;
        int slotId = 0;
        int symbolId = 0;
        // Calculate IFGs sent
        long long totalTransmittedBits = numberOfPackets * bytesPerPacket * 8;
        long long timeOfPacketsMs = ceil(totalTransmittedBits * 1.0 / (config.EthLineRate * pow(10, 6)));
        long long timeOfIFGsMs = config.EthCaptureSizeMs - timeOfPacketsMs;
        long long timePerIFG = (8 / config.EthLineRate) + 1; // Because of precision in 8/lineRate it will be zero so I add 1 as I ceil it
        long long numberOfIFGs = timeOfIFGsMs / timePerIFG;
        cout << "Number of IFGs " << numberOfIFGs << endl;
        cout << "Total transmitted bits " << totalTransmittedBits << endl;
        cout << "Time of packets " << timeOfPacketsMs << endl;

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
            if (i == 30)
                break;
            int payloadSize = config.OranNrbPerPacket * 12;
            ORAN oran(frameId, subframeId, slotId, symbolId, config.OranPayload, payloadSize);
            ECPRI ecpri(oran);
            Packet p(destAddress, srcAddress, "AEFE", ecpri.getECPRI());
            // Alignment
            int IFGs = config.EthMinNumOfIFGsPerPacket;
            string AddedIFG = "";
            while (IFGs != 0)
            {
                AddedIFG += "07";
                IFGs--;
            }
            if (!isAligned(p.getPacket().size()))
            {
                int padding = addIFGs(p) / 2;
                while (padding != 0)
                {
                    AddedIFG += "07";
                    padding--;
                }
            }
            p.setIFG(AddedIFG);
            out << p.getPacket();

            int count = numberOfIFGs;

            while (count != 0)
            {
                out << "07";

                count--;
            }
            out<<endl;
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

int Program::addIFGs(Packet &packet)
{

    int padding = 4 - (packet.getPacket().size() % 4);
    return padding;
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