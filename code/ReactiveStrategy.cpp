#include "ReactiveStrategy.h"
#include "absl/flags/flag.h"
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
static int count = 0;
void ReactiveStrategy::Compute()
{

    ComputeOptimization(this->applications, this->availableInterfaces);
    count++;
}

void ReactiveStrategy::ComputeOptimization(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> currentApplications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> currentInterfaces)
{
    // At each step we want to minimize the packet cost for the overall transmission.

    if ( ns3::Simulator::Now().GetSeconds() == 105){
        auto x = 4;
    }

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
            {
                constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[i * 3 + j], 1);
            }
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
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(i);
            double milliSecondsInterfaceDelay = ((double)currentInterface->getDelayInMilliseconds())  + delays.at(j).at(i);
            double delayCoefficient = totalPackagesForApplication > 0 ? milliSecondsInterfaceDelay / totalPackagesForApplication : 0;
            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[i + (j * 2) + j], delayCoefficient);
        }
    }

    // // Vincolo sulla percentuale di errore
    // for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
    // {
    //     constraints.push_back(
    //         solver->MakeRowConstraint(0, staticApplication->errorRate));
    //     for (std::size_t j = 0; j < this->availableInterfaces->size(); ++j)
    //     {
    //         std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(j);
    //         double errorRate = currentInterface->getErrorRate();
    //         double errorCoefficient = errorRate / staticApplication->amountOfPacketsToSend;
    //         constraints.back()->SetCoefficient(amountOfPacketForInterface[j], errorCoefficient);
    //     }
    // }

    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        auto currentApplication = currentApplications->at(j);
        auto currentPendingPackages = currentApplication->pendingpackets.size();
        constraints.push_back(
            solver->MakeRowConstraint(currentPendingPackages, infinity));

        for (std::size_t i = 0; i < currentInterfaces->size(); ++i)
        {
            constraints.back()->SetCoefficient(amountOfPacketForApplicationOverInterface[i + (j * 2) + j], 1);
        }
    }

    MPObjective *const objective = solver->MutableObjective();
    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        for (size_t i = 0; i < currentInterfaces->size(); ++i)
        {
            objective->SetCoefficient(amountOfPacketForApplicationOverInterface[i + (j * 2) + j], this->availableInterfaces->at(i)->cost);
        }
    }
    objective->SetMinimization();

    const MPSolver::ResultStatus result_status = solver->Solve();

    // Check that the problem has an optimal solution.
    if (result_status != MPSolver::OPTIMAL)
    {

        std::cout << "The problem does not have an optimal solution!" << "\n";
        std::cout << ns3::Simulator::Now().GetSeconds() << "\n";
        for (std::size_t j = 0; j < currentApplications->size(); ++j)
        {
            std::cout << "Pacchetti in coda: " << currentApplications->at(j)->pendingpackets.size() << "\n";
        }
        throw new std::exception();
        if (result_status == MPSolver::FEASIBLE)
        {
            std::cout << "A potentially suboptimal solution was found" << "\n";
            ;
        }
        else
        {
            std::cout << "The solver could not solve the problem." << "\n";
            return;
        }
    }

    for (size_t i = 0; i < currentInterfaces->size(); ++i)
    {
        for (std::size_t j = 0; j < currentApplications->size(); ++j)
        {
            auto currentApplication = currentApplications->at(j);
            double value = amountOfPacketForApplicationOverInterface.at(i + j)->solution_value();
            uint32_t flooredValue = static_cast<uint32_t>(std::floor(value));
            std::cout << "Pacchetti per l'applicazione " << j << " da mandare sull'interfaccia " << i << ": " << flooredValue << "\n ";
        }
    }

    for (std::size_t j = 0; j < currentApplications->size(); ++j)
    {
        auto currentApplication = currentApplications->at(j);
        for (size_t i = 0; i < currentInterfaces->size(); ++i)
        {

            double value = amountOfPacketForApplicationOverInterface.at(i + (j * 2) + j)->solution_value();
            uint32_t flooredValue = static_cast<uint32_t>(std::floor(value));
            while (flooredValue > 0)
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
                std::cout << "Pacchetto accodato sull'interfaccia " << i << "\n";
                this->availableInterfaces->at(i)->enqueuePacket(packetInfo);
                flooredValue--;
            }
        }
    }
}

uint32_t ReactiveStrategy::DelayEstimator(
    std::shared_ptr<SDWanApplication> application,
    std::shared_ptr<ISPInterface> interface)
{
    return interface->getAverageWaitingTimeInMilliseconds();
}