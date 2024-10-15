#include "ns3/core-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("HelloSimulator");

int main(int argc, char* argv[])
{
    std::cout << "yoo";
    NS_LOG_UNCOND("Hello Simulator");
    return 0;
}
