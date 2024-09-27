#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;
#include "Program.h"

void Program::generatePackets(const std::string &outputFile)
{
    std::ofstream out(outputFile);

    long long packetsPerSymbol = ceil(config.OranMaxNrb * 1.0 / config.OranNrbPerPacket);
    long long symbolPerSlot = 14; // Assuming normal cyclic prefix
    long long packetsPerSlot = packetsPerSymbol * symbolPerSlot;
    long long mu = log2(config.OranSCS / 15.0);
    long long slotsPerSubframe = pow(2, mu);
    long long slotsPerFrame = slotsPerSubframe * 10;
    long long framesPerSecond=100;
    long long packetsPerSecond = packetsPerSlot * slotsPerFrame*framesPerSecond; 
    long long packetsPerCaptureSize=packetsPerSecond*config.EthCaptureSizeMs*pow(10,-3);
    long long bitsPerPacket =  12 * config.OranNrbPerPacket * 16 * 2;
    
    long long ethernetHeaderSize = 26;
    long long ecpriHeaderSize = 8;
    long long oranHeaderSize = 8;
    long long bytesPerPacket = (bitsPerPacket / 8) + ethernetHeaderSize + ecpriHeaderSize + oranHeaderSize;
    long long packetsPerSubframe = packetsPerSlot * slotsPerSubframe;
    long long packetsPerFrame = packetsPerSubframe * 10;
    int numberOfPackets = packetsPerCaptureSize;
    long long indexOfSamples = 0;
    cout<<"Number of packets "<<numberOfPackets<<endl;
        // Calculate IFGs sent
        long long totalTransmittedBits = numberOfPackets * bytesPerPacket * 8;
        long long timeOfPacketsMs = ceil(totalTransmittedBits * 1.0 / (config.EthLineRate * pow(10, 6)));
        long long timeOfIFGsMs = (config.EthCaptureSizeMs - timeOfPacketsMs)*1000;
        long long timePerIFG = (8 / config.EthLineRate) + 1; // Because of precision in 8/lineRate it will be zero so I add 1 as I ceil it
        long long numberOfIFGs = timeOfIFGsMs / timePerIFG;
        cout << "Number of IFGs " << numberOfIFGs << endl;
        cout << "Total transmitted bits " << totalTransmittedBits << endl;
        cout << "Time of packets " << timeOfPacketsMs << endl;

    if (bytesPerPacket < config.EthMaxPacketSize) // No fragmentation
    {
        cout << "No fragmentation " << endl;
        cout << bytesPerPacket << endl;

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
            //    cout<<endl<<i<<endl;

                symbolId++;
                symbolId=symbolId%14;
            }
            if (i % packetsPerSubframe == 0)
            {
                subframeId++;
            }
            if (i % packetsPerFrame == 0)
            {
                frameId++;
            }

            int payloadSize = (bytesPerPacket - (ethernetHeaderSize + ecpriHeaderSize + oranHeaderSize));
            ORAN oran(frameId, subframeId, slotId, symbolId, config.OranPayload, payloadSize, indexOfSamples);
            ECPRI ecpri(oran);
            Packet p(destAddress, srcAddress, "AEFE", ecpri.getECPRI());
            indexOfSamples += payloadSize;
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
            out << endl;

        }
            int count = numberOfIFGs;

            while (count != 0)
            {
                out << "07";

                count--;
            }
            
    }
    else
    { // Fragmentation logic
        cout << "Fragmentation" << endl;
        cout << bytesPerPacket << endl;
        

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
                slotId++;
            if (i % packetsPerSymbol == 0)
                symbolId++;
            if (i % packetsPerSubframe == 0)
                subframeId++;
            if (i % packetsPerFrame == 0)
                frameId++;
            if(i==3) break;

            // Fragmentation logic begins here
            int maxPayloadSize = (config.EthMaxPacketSize) - (ethernetHeaderSize + ecpriHeaderSize + oranHeaderSize);
            int totalPayloadSize = bytesPerPacket - (ethernetHeaderSize + ecpriHeaderSize + oranHeaderSize);
            int fragments = ceil(totalPayloadSize * 1.0 / maxPayloadSize);
            cout << "Fragments " << fragments << endl;
            // return;
            for (int f = 0; f < fragments; ++f)
            {
                int fragmentPayloadSize = std::min(maxPayloadSize, totalPayloadSize - (f * maxPayloadSize));
                // cout<<"Frangment Payload "<<fragmentPayloadSize<<endl;
                // Create fragment
                ORAN fragmentOran(frameId, subframeId, slotId, symbolId, config.OranPayload, fragmentPayloadSize, indexOfSamples);
                indexOfSamples += fragmentPayloadSize;
                ECPRI fragmentEcpri(fragmentOran);
                Packet fragmentPacket(destAddress, srcAddress, ethernetType, fragmentEcpri.getECPRI());

                // Alignment and IFG handling remains the same
                int IFGs = config.EthMinNumOfIFGsPerPacket;
                string AddedIFG = "";
                while (IFGs != 0)
                {
                    AddedIFG += "07";
                    IFGs--;
                }
                if (!isAligned(fragmentPacket.getPacket().size()))
                {
                    int padding = addIFGs(fragmentPacket) / 2;
                    while (padding != 0)
                    {
                        AddedIFG += "07";
                        padding--;
                    }
                }
                fragmentPacket.setIFG(AddedIFG);
                out << fragmentPacket.getPacket();

                // Add the IFGs between packets
                int count = numberOfIFGs;
                
                while (count > 0)
                {
                    out << "07";
                    count--;
                }
                out << endl;
            }
            indexOfSamples=0;
        }
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