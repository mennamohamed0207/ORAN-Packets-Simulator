#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;
#include "Program.h"

void Program::generatePackets(const std::string &outputFile)
{
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