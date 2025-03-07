#include "ReactiveStrategy.h"
#include "absl/flags/flag.h"
#include "Utils.h"
#include "absl/log/flags.h"
#include "ortools/base/init_google.h"
#include "ortools/base/logging.h"
#include "ortools/linear_solver/linear_solver.h"

ReactiveStrategy::ReactiveStrategy(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces)
    : Strategy(applications, availableInterfaces), applicationToInterfacesMap()
{
}

void ReactiveStrategy::Compute()
{

    uint32_t amountOfInterfaces = this->availableInterfaces->size();
    uint32_t counter = Utils::maxIntWithNBits(amountOfInterfaces);
    std::vector<std::vector<std::shared_ptr<ISPInterface>>> interfaceSet;
    for (uint32_t i = 1; i <= counter; i++)
    {
        std::vector<bool> interfacesToUse = Utils::getBoolVectorFromInt(i, amountOfInterfaces);
        std::vector<std::shared_ptr<ISPInterface>> currentSet;
        for (uint32_t j = 0; j < interfacesToUse.size(); j++)
        {
            bool currentValue = interfacesToUse.at(j);
            if (currentValue || true)
            {
                currentSet.push_back(this->availableInterfaces->at(j));
            }
        }
        interfaceSet.push_back(currentSet);
    }

    std::vector<std::shared_ptr<ComputeOptimizationResult>> results;

    for (uint32_t i = 0; i < interfaceSet.size(); i++)
    {
        auto data = std::make_shared<std::vector<std::shared_ptr<ISPInterface>>>(interfaceSet.at(i));
        auto result = ComputeOptimization(this->applications, data);

        results.push_back(result);
    }

    std::shared_ptr<ComputeOptimizationResult> bestSolution = nullptr;

    auto it = std::max_element(results.begin(), results.end(), [](const std::shared_ptr<ComputeOptimizationResult> &a, const std::shared_ptr<ComputeOptimizationResult> &b)
                               {
        if (!a->getIsFeasible()) return true;
        if (!b->getIsFeasible()) return false;
        return a->getTotalCost() > b->getTotalCost(); });

    uint32_t currentPendingPackages = 0;
    for (size_t i = 0; i < this->applications->size(); i++)
    {
        currentPendingPackages += this->applications->at(i)->pendingpackets.size();
    }

    if (it != results.end() && (*it)->getIsFeasible())
    {
        bestSolution = (*it);
        uint32_t currentCost = bestSolution->getTotalCost();
        uint32_t totalCost = strategyData.size() == 0 ? currentCost : strategyData.back().totalCost + currentCost;
        uint32_t totalAmountOfPackets = strategyData.size() == 0 ? currentPendingPackages : strategyData.back().totalAmountOfPackets + currentPendingPackages;
        StrategyDataPoint dataPoint = StrategyDataPoint(totalCost, currentCost, totalAmountOfPackets, currentPendingPackages);
        this->strategyData.push_back(dataPoint);
    }
    else
    {
        throw new std::runtime_error("Nessuna soluzione trovata");
    }

    auto &solutionValues = bestSolution->getSolutionValues();
    for (uint32_t i = 0; i < solutionValues.size(); i++)
    {
        auto currentApplication = this->applications->at(i);
        std::vector<uint32_t> packetsForApplication = solutionValues.at(i);
        for (uint32_t j = 0; j < bestSolution->interfaces->size(); j++)
        {
            std::shared_ptr<ISPInterface> currentInteface = bestSolution->interfaces->at(j);
            auto packetsForApplicationAndInterface = packetsForApplication.at(j);
            while (packetsForApplicationAndInterface > 0)
            {
                if (currentApplication->pendingpackets.size() == 0)
                {
                    throw new std::runtime_error("Nessun pacchetto");
                }
                currentApplication->pendingpackets.pop();
                ns3::Time currentTime = ns3::Simulator::Now();
                uint32_t applicationId = currentApplication->applicationId;

                SendPacketInfo packetInfo;

                packetInfo.dateEnqueued = currentTime;
                packetInfo.originatedFrom = applicationId;
                currentInteface->enqueuePacket(packetInfo);
                packetsForApplicationAndInterface--;
            }
        }
    }

    if (this->getAllDataHasBeenSent())
    {
        std::string fileName = "outputReactive.csv";
        Utils::printResultsToFile(fileName, this->strategyData);
    }
}

std::shared_ptr<ComputeOptimizationResult> ReactiveStrategy::ComputeOptimization(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> currentApplications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> currentInterfaces)
{
    // At each step we want to minimize the packet cost for the overall transmission.

    uint32_t amountOfApplications = currentApplications->size();
    uint32_t amountOfInterfaces = currentInterfaces->size();
    using namespace operations_research;

    std::unique_ptr<operations_research::MPSolver> solver(operations_research::MPSolver::CreateSolver("CBC_MIXED_INTEGER_PROGRAMMING"));
    const double infinity = solver->infinity();
    std::vector<operations_research::MPVariable *> amountOfPacketForApplicationOverInterface;

    for (unsigned short i = 0; const std::shared_ptr<ISPInterface> &interface : *(currentInterfaces))
    {
        for (size_t j = 0; j < currentApplications->size(); j++)
        {
            std::string interfaceApplicationName = "Interfaccia " + std::to_string(i) + " Applicazione " + std::to_string(j);
            amountOfPacketForApplicationOverInterface.push_back(solver->MakeIntVar(0.0, infinity, interfaceApplicationName));
            /* code */
        }

        i++;
    }

    std::vector<MPConstraint *> constraints;
    // Constraint number two: the capacity of each interface must not be exceeded.

    for (std::size_t j = 0; j < currentInterfaces->size(); ++j)
    {
        auto currentInterface = currentInterfaces->at(j);
        constraints.push_back(
            solver->MakeRowConstraint(0, currentInterface->getPackageRate()));

        for (std::size_t i = 0; i < currentApplications->size(); ++i)
        {
            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[i + (j * amountOfApplications)], 1);
        }
    }

    // Constraint number three: we need to calculate the delay in milliseconds
    // for each application over each flow group

    std::vector<std::vector<uint32_t>> delays;
    for (std::size_t i = 0; i < currentApplications->size(); ++i)
    {
        delays.push_back(std::vector<uint32_t>());
        std::vector<uint32_t> &currentValues = delays.at(i);
        std::shared_ptr<SDWanApplication> currentApplication = currentApplications->at(i);
        for (size_t j = 0; j < currentInterfaces->size(); j++)
        {
            std::shared_ptr<ISPInterface> currentInterface = currentInterfaces->at(j);
            auto currentEstimation = DelayEstimator(currentApplication, currentInterface);
            currentValues.push_back(currentEstimation);
        }
    }

    // Vincoli sul delay
    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        auto currentApplication = currentApplications->at(j);
        constraints.push_back(
            solver->MakeRowConstraint(0, currentApplication->requiredDelay));
        auto totalPackagesForApplication = currentApplication->pendingpackets.size();

        for (std::size_t i = 0; i < currentInterfaces->size(); ++i)
        {
            std::shared_ptr<ISPInterface> currentInterface = currentInterfaces->at(i);
            double milliSecondsInterfaceDelay = ((double)currentInterface->getDelayInMilliseconds()) + delays.at(j).at(i);
            double delayCoefficient = totalPackagesForApplication > 0 ? milliSecondsInterfaceDelay / totalPackagesForApplication : 0;

            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], delayCoefficient);
        }
    }

    // // Vincolo sulla percentuale di errore
    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        auto currentApplication = currentApplications->at(j);
        constraints.push_back(
            solver->MakeRowConstraint(0, currentApplication->errorRate));
        auto totalPackagesForApplication = currentApplication->pendingpackets.size();

        for (std::size_t i = 0; i < currentInterfaces->size(); ++i)
        {
            std::shared_ptr<ISPInterface> currentInterface = currentInterfaces->at(i);
            double errorRate = currentInterface->getErrorRate();
            double errorCoefficient = totalPackagesForApplication > 0 ? errorRate / totalPackagesForApplication : 0;

            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], errorCoefficient);
        }
    }


    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        auto currentApplication = currentApplications->at(j);
        auto currentPendingPackages = currentApplication->pendingpackets.size();
        constraints.push_back(
            solver->MakeRowConstraint(currentPendingPackages, infinity));

        for (std::size_t i = 0; i < currentInterfaces->size(); ++i)
        {

            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], 1);
        }
    }

    MPObjective *const objective = solver->MutableObjective();
    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        for (size_t i = 0; i < currentInterfaces->size(); ++i)
        {

            objective->SetCoefficient(amountOfPacketForApplicationOverInterface[j + (i * amountOfApplications)], currentInterfaces->at(i)->cost);
        }
    }
    objective->SetMinimization();

    const MPSolver::ResultStatus result_status = solver->Solve();

    // Check that the problem has an optimal solution.
    if (result_status != MPSolver::OPTIMAL)
    {
        ComputeOptimizationResult result(false, std::vector<std::vector<uint32_t>>(), nullptr, 0);
        return std::make_shared<ComputeOptimizationResult>(result);
    }


    std::vector<std::vector<uint32_t>> solutionValues;
    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        auto currentApplication = currentApplications->at(j);
        std::vector<uint32_t> currentApplicationValues;
        for (size_t i = 0; i < currentInterfaces->size(); ++i)
        {

            double value = amountOfPacketForApplicationOverInterface.at(j + (i * amountOfApplications))->solution_value();
            uint32_t flooredValue = static_cast<uint32_t>(std::floor(value));
            currentApplicationValues.push_back(flooredValue);
        }
        solutionValues.push_back(currentApplicationValues);
    }

    ComputeOptimizationResult result(true, solutionValues, currentInterfaces, std::floor(objective->Value()));
    return std::make_shared<ComputeOptimizationResult>(result);
}

uint32_t ReactiveStrategy::DelayEstimator(
    std::shared_ptr<SDWanApplication> application,
    std::shared_ptr<ISPInterface> interface)
{
    return interface->getAverageWaitingTimeInMilliseconds();
}
