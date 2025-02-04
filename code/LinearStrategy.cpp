#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "LinearStrategy.h"
#include "SDWanApplication.h"
#include "SDWanStaticApplication.h"
#include "ISPInterface.h"

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
    std::shared_ptr<SDWanApplication> firstApplication = this->applications->at(0);
    std::shared_ptr<SDWanStaticApplication> staticApplication = std::dynamic_pointer_cast<SDWanStaticApplication>(firstApplication);
    if (!staticApplication)
    {
        std::cout << "Applicazione non valida" << "\n";
        return;
    }
    std::unique_ptr<operations_research::MPSolver> solver(operations_research::MPSolver::CreateSolver("CBC_MIXED_INTEGER_PROGRAMMING"));
    const double infinity = solver->infinity();
    std::vector<operations_research::MPVariable *> interfaces;
    for (unsigned short i = 0; const std::shared_ptr<ISPInterface> &interface : *(this->availableInterfaces))
    {
        std::string interfaceName = "Interfaccia " + std::to_string(i);
        interfaces.push_back(solver->MakeIntVar(0.0, infinity, interfaceName));
        i++;
    }

    std::vector<MPConstraint *> constraints;
    // Vincoli sul delay
    for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
    {
        constraints.push_back(
            solver->MakeRowConstraint(0, staticApplication->requiredDelay));
        for (std::size_t j = 0; j < this->availableInterfaces->size(); ++j)
        {
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(j);
            double milliSecondsInterfaceDelay = ((double)currentInterface->getDelayInMilliseconds());
            double delayCoefficient = milliSecondsInterfaceDelay / staticApplication->amountOfPacketsToSend;
            constraints.back()->SetCoefficient(interfaces[j], delayCoefficient);
        }
    }

    // Vincoli sulla banda
    for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
    {
        // Convertiamo tutti i requisiti sulla banda in bps
        uint64_t applicationRequiredBitRate = staticApplication->getRequiredDataRate().GetBitRate();

        constraints.push_back(
            solver->MakeRowConstraint(applicationRequiredBitRate, infinity));
        for (std::size_t j = 0; j < this->availableInterfaces->size(); ++j)
        {
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(j);
            double interfaceBitRate = ((double)currentInterface->getDataBitRate());
            double bandWidthCoefficient = interfaceBitRate / staticApplication->amountOfPacketsToSend;
            constraints.back()->SetCoefficient(interfaces[j], bandWidthCoefficient);
        }
    }

    // Vincolo sulla percentuale di errore
    for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
    {
        constraints.push_back(
            solver->MakeRowConstraint(0, staticApplication->errorRate));
        for (std::size_t j = 0; j < this->availableInterfaces->size(); ++j)
        {
            std::shared_ptr<ISPInterface> currentInterface = this->availableInterfaces->at(j);
            double errorRate = currentInterface->getErrorRate();
            double errorCoefficient = errorRate / staticApplication->amountOfPacketsToSend;
            constraints.back()->SetCoefficient(interfaces[j], errorCoefficient);
        }
    }

    // Vincolo sul numero totale di pacchetti.
    for (std::size_t i = 0; i < this->availableInterfaces->size(); ++i)
    {
        constraints.push_back(
            solver->MakeRowConstraint(staticApplication->amountOfPacketsToSend, infinity));
        for (std::size_t j = 0; j < this->availableInterfaces->size(); ++j)
        {
            constraints.back()->SetCoefficient(interfaces[j], 1);
        }
    }

    MPObjective *const objective = solver->MutableObjective();
    for (size_t i = 0; i < interfaces.size(); ++i)
    {
        objective->SetCoefficient(interfaces[i], this->availableInterfaces->at(i)->cost);
    }
    objective->SetMinimization();

    const MPSolver::ResultStatus result_status = solver->Solve();

    // Check that the problem has an optimal solution.
    if (result_status != MPSolver::OPTIMAL)
    {
        std::cout << "The problem does not have an optimal solution!" << "\n";
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

    for (size_t i = 0; i < interfaces.size(); ++i)
    {
        double value = interfaces.at(i)->solution_value();
        uint32_t flooredValue = static_cast<uint32_t>(std::floor(value));
        std::cout << "Pacchetti per interfaccia " << i << ": " << flooredValue << "\n";
        for (size_t j = 0; j < flooredValue; j++)
        {
            if (!staticApplication->pendingpackets.empty())
            {
                staticApplication->pendingpackets.pop();
                ns3::Time currentTime = ns3::Simulator::Now();
                uint32_t applicationId = staticApplication->applicationId;

                SendPacketInfo packetInfo;

                packetInfo.dateEnqueued = currentTime;
                packetInfo.originatedFrom = applicationId;

                this->availableInterfaces->at(i)->enqueuePacket(packetInfo);
            }
        }
    }

    this->hasBeenComputed = true;

    return;
}