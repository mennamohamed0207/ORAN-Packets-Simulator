#include "configurations.h"

configurations::configurations()
{
    EthLineRate = 0;
    EthCaptureSizeMs = 0;
    EthMinNumOfIFGsPerPacket = 0;
    EthDestAddress = "";
    EthSourceAddress = "";
    EthMaxPacketSize = 0;
}
// Function to trim leading and trailing whitespace characters
string configurations::trim(const std::string& str) {
    // Find the position of the first non-whitespace character
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });

    // Find the position of the last non-whitespace character
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
        return std::isspace(c);
    }).base();

    // Check if the entire string is whitespace
    if (start >= end) {
        return "";
    }

    // Return the trimmed string
    return std::string(start, end);
}
// Function to remove comments from a line
string configurations::removeComments(const std::string& line) {
    size_t commentPos = line.find("//");
    if (commentPos != std::string::npos) {
        return line.substr(0, commentPos);  // Return the part before the comment
    }
    return line;  // No comment found, return the original line
}
void configurations::printConfigurations()
{
    std::cout << "EthLineRate: " << EthLineRate << std::endl;
    std::cout << "EthCaptureSizeMs: " << EthCaptureSizeMs << std::endl;
    std::cout << "EthMinNumOfIFGsPerPacket: " << EthMinNumOfIFGsPerPacket << std::endl;
    std::cout << "EthDestAddress: " << EthDestAddress << std::endl;
    std::cout << "EthSourceAddress: " << EthSourceAddress << std::endl;
    std::cout << "EthMaxPacketSize: " << EthMaxPacketSize << std::endl;

    std::cout << "OranSCS: " << OranSCS << std::endl;
    std::cout << "OranMaxNrb: " << OranMaxNrb << std::endl;
    std::cout << "OranNrbPerPacket: " << OranNrbPerPacket << std::endl;
    std::cout << "OranPayloadType: " << OranPayloadType << std::endl;
    std::cout << "OranPayload: " << OranPayload << std::endl;
}
int configurations::getNumberAfterEqual(const std::string &str)
{
    size_t pos = str.find('=');
    if (pos != std::string::npos)
    {
        std::string numberStr = str.substr(pos + 1);
        return std::stoi(numberStr);
    }
    else
    {
        std::cerr << "Error: '=' not found in the string!" << std::endl;
        return -1;
    }
}
string configurations::getAddress(const std::string &str)
{
    size_t pos = str.find('=');
    if (pos != std::string::npos)
    {
        size_t address = str.find('x');
        if (address != std::string::npos)
        {

            std::string numberStr = str.substr(address + 1);
            return numberStr;
        }
        return "";
    }
    else
    {
        std::cerr << "Error: '=' not found in the string!" << std::endl;
        return "";
    }
}
string configurations::getString(const std::string &str)
{
    size_t pos = str.find('=');

    if (pos != std::string::npos)
    {
        std::string text = str.substr(pos + 1);
        return text;
    }
    else
    {
        std::cerr << "Error: '=' not found in the string!" << std::endl;
        return "";
    }
}
void configurations::readConfigurations(string configurationFile)
{
    std::ifstream file(configurationFile);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file " << configurationFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // cout << line << endl;
        line =this->removeComments(line);
        if (line.find("Eth.LineRate") != std::string::npos)
        {
            EthLineRate = getNumberAfterEqual(line);
        }
        else if (line.find("Eth.CaptureSizeMs") != std::string::npos)
        {
            EthCaptureSizeMs = getNumberAfterEqual(line);
        }
        else if (line.find("Eth.MinNumOfIFGsPerPacket") != std::string::npos)
        {
            EthMinNumOfIFGsPerPacket = getNumberAfterEqual(line);
        }
        else if (line.find("Eth.DestAddress") != std::string::npos)
        {
            EthDestAddress = getAddress(line);
        }
        else if (line.find("Eth.SourceAddress") != std::string::npos)
        {
            EthSourceAddress = getAddress(line);
        }
        else if (line.find("Eth.MaxPacketSize") != std::string::npos)
        {
            EthMaxPacketSize = getNumberAfterEqual(line);
        }
        else if (line.find("Oran.SCS") != std::string::npos)
        {
            OranSCS = getNumberAfterEqual(line);
        }
        else if (line.find("Oran.MaxNrb") != std::string::npos)
        {
            OranMaxNrb = getNumberAfterEqual(line);
            if (OranMaxNrb == 0)
                OranMaxNrb = 273;
        }

        else if (line.find("Oran.NrbPerPacket") != std::string::npos)
        {
            OranNrbPerPacket = getNumberAfterEqual(line);
        }
        else if (line.find("Oran.PayloadType") != std::string::npos)
        {
            OranPayloadType = getString(line);
        }
        else if (line.find("Oran.Payload") != std::string::npos)
        {
            OranPayload = this->trim(getString(line));
        }
    }
    // printConfigurations();

    file.close();
}