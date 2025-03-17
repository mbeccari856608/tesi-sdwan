#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "LinearStrategy.h"
#include "SDWanApplication.h"
#include "SDWanStaticApplication.h"
#include "ISPInterface.h"
#include "Utils.h"
#include "absl/flags/flag.h"
#include "absl/log/flags.h"
#include "ortools/base/init_google.h"
#include "ortools/base/logging.h"
#include "ortools/linear_solver/linear_solver.h"

LinearStrategy::LinearStrategy(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces) : Strategy(applications, availableInterfaces)
{
}

void LinearStrategy::Compute()
{
    if (this->hasBeenComputed)
    {
        return;
    }
    using namespace operations_research;
    // At each step we want to minimize the packet cost for the overall transmission.

    uint32_t amountOfApplications = this->applications->size();
    uint32_t amountOfInterfaces = this->availableInterfaces->size();
    using namespace operations_research;

    std::unique_ptr<operations_research::MPSolver> solver(operations_research::MPSolver::CreateSolver("CBC_MIXED_INTEGER_PROGRAMMING"));
    const double infinity = solver->infinity();
    std::vector<operations_research::MPVariable *> amountOfPacketForApplicationOverInterface;

    for (unsigned short i = 0; const std::shared_ptr<ISPInterface> &interface : *(this->availableInterfaces))
    {
        for (size_t j = 0; j < this->applications->size(); j++)
        {
            std::string interfaceApplicationName = "Interfaccia " + std::to_string(i) + " Applicazione " + std::to_string(j);
            amountOfPacketForApplicationOverInterface.push_back(solver->MakeIntVar(0.0, infinity, interfaceApplicationName));
            /* code */
        }

        i++;
    }

    std::vector<MPConstraint *> constraints;

    // Vincoli sulla banda
    for (std::size_t j = 0; j < this->applications->size(); ++j)
    {
        auto currentApplication = this->applications->at(j);
        auto requiredBitRate = currentApplication->getRequiredDataRate().GetBitRate();
        auto requiredPacketRate =  requiredBitRate/ Utils::PacketSizeBit;
        constraints.push_back(
            solver->MakeRowConstraint(requiredBitRate, infinity));
        auto totalPackagesForApplication = currentApplication->pendingpackets.size();

        for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
        {
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(i);
            double bitRate = currentInterface->getDataBitRate();
            double bandwidthCoefficient = totalPackagesForApplication > 0 ? bitRate / totalPackagesForApplication : 0;

            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], bandwidthCoefficient);
        }
    }


    // Vincoli sul delay
    for (std::size_t j = 0; j < this->applications->size(); ++j)
    {
        auto currentApplication = this->applications->at(j);
        constraints.push_back(
            solver->MakeRowConstraint(0, currentApplication->requiredDelay));
        auto totalPackagesForApplication = currentApplication->pendingpackets.size();

        for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
        {
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(i);
            double milliSecondsInterfaceDelay = ((double)currentInterface->getDelayInMilliseconds());
            double delayCoefficient = totalPackagesForApplication > 0 ? milliSecondsInterfaceDelay / totalPackagesForApplication : 0;

            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], delayCoefficient);
        }
    }

    // // Vincolo sulla percentuale di errore
    for (std::size_t j = 0; j < this->applications->size(); ++j)
    {
        auto currentApplication = this->applications->at(j);
        constraints.push_back(
            solver->MakeRowConstraint(0, currentApplication->errorRate));
        auto totalPackagesForApplication = currentApplication->pendingpackets.size();

        for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
        {
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(i);
            double errorRate = currentInterface->getErrorRate();
            double errorCoefficient = totalPackagesForApplication > 0 ? errorRate / totalPackagesForApplication : 0;

            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], errorCoefficient);
        }
    }


    for (std::size_t j = 0; j < this->applications->size(); ++j)
    {
        auto currentApplication = this->applications->at(j);
        auto currentPendingPackages = currentApplication->pendingpackets.size();
        constraints.push_back(
            solver->MakeRowConstraint(currentPendingPackages, currentPendingPackages));

        for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
        {

            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], 1);
        }
    }

    MPObjective *const objective = solver->MutableObjective();
    for (std::size_t j = 0; j < this->applications->size(); ++j)
    {
        for (size_t i = 0; i < this->availableInterfaces->size(); ++i)
        {

            objective->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], this->availableInterfaces->at(i)->cost);
        }
    }
    objective->SetMinimization();

    const MPSolver::ResultStatus result_status = solver->Solve();

    // Check that the problem has an optimal solution.
    if (result_status != MPSolver::OPTIMAL)
    {
        throw std::runtime_error("Soluzione non trovata");
    }


    std::vector<std::vector<uint32_t>> solutionValues;
    for (std::size_t j = 0; j < this->applications->size(); ++j)
    {
        auto currentApplication = this->applications->at(j);
        std::vector<uint32_t> currentApplicationValues;
        for (size_t i = 0; i < this->availableInterfaces->size(); ++i)
        {

            double value = amountOfPacketForApplicationOverInterface.at(j + (i * amountOfApplications))->solution_value();
            uint32_t flooredValue = static_cast<uint32_t>(std::floor(value));
            currentApplicationValues.push_back(flooredValue);
        }
        solutionValues.push_back(currentApplicationValues);
    }

    for (uint32_t i = 0; i < solutionValues.size(); i++)
    {
        auto currentApplication = this->applications->at(i);
        std::vector<uint32_t> packetsForApplication = solutionValues.at(i);
        for (uint32_t j = 0; j < this->availableInterfaces->size(); j++)
        {
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(j);
            auto packetsForApplicationAndInterface = packetsForApplication.at(j);
            while (packetsForApplicationAndInterface > 0)
            {
                currentApplication->pendingpackets.pop();
                ns3::Time currentTime = ns3::Simulator::Now();
                uint32_t applicationId = currentApplication->applicationId;

                SendPacketInfo packetInfo;

                packetInfo.dateEnqueued = currentTime;
                packetInfo.originatedFrom = applicationId;
                currentInterface->enqueuePacket(packetInfo);
                packetsForApplicationAndInterface--;
            }
        }
    }

    this->hasBeenComputed = true;

    return;
}