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

void ReactiveStrategy::Compute()
{
    ComputeOptimization(this->applications, this->availableInterfaces);
}

void ReactiveStrategy::ComputeOptimization(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> currentApplications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> currentInterfaces)
{
    // At each step we want to minimize the packet cost for the overall transmission.

    using namespace operations_research;

    std::unique_ptr<operations_research::MPSolver> solver(operations_research::MPSolver::CreateSolver("CBC_MIXED_INTEGER_PROGRAMMING"));
    const double infinity = solver->infinity();
    std::vector<operations_research::MPVariable *> amountOfPacketForInterface;
    for (unsigned short i = 0; const std::shared_ptr<ISPInterface> &interface : *(currentInterfaces))
    {
        std::string interfaceName = "Interfaccia " + std::to_string(i);
        amountOfPacketForInterface.push_back(solver->MakeIntVar(0.0, infinity, interfaceName));
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
            constraints.back()->SetCoefficient(amountOfPacketForInterface[j], 1);
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


    for (std::size_t i = 0; i < currentInterfaces->size(); ++i)
    {
        for (std::size_t j = 0; j < currentApplications->size(); ++j)
        {
            auto requiredDelay = currentApplications->at(j)->requiredDelay;
            auto currentDelay = delays.at(i).at(j);
            constraints.push_back(
                solver->MakeRowConstraint(currentDelay, requiredDelay));
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

    // Vincolo sul numero totale di pacchetti.
    for (std::size_t i = 0; i < currentApplications->size(); ++i)
    {
        auto currentApplication = currentApplications->at(i);
        constraints.push_back(
            solver->MakeRowConstraint(currentApplication->pendingpackets.size(), infinity));
        for (std::size_t j = 0; j < availableInterfaces->size(); ++j)
        {
            constraints.back()->SetCoefficient(amountOfPacketForInterface[j], 1);
        }
    }

    MPObjective *const objective = solver->MutableObjective();
    for (size_t i = 0; i < amountOfPacketForInterface.size(); ++i)
    {
        objective->SetCoefficient(amountOfPacketForInterface[i], this->availableInterfaces->at(i)->cost);
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
}