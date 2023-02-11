#include "Task.hpp"

int main(void) {
    try {
        Task task = Task::LoadTask("task.txt");
        task.Print();
        task.SolveEPEM();
        std::cout << "EPEM:" << std::endl;
        task.PrintSolve();
        task.SolveSimplex();
        std::cout << "Simplex:" << std::endl;
        task.PrintSolve();
        
        task = task.GoToDual();
        task.Print();
        task.SolveEPEM();
        std::cout << "EPEM:" << std::endl;
        task.PrintSolve();
        task.SolveSimplex();
        std::cout << "Simplex:" << std::endl;
        task.PrintSolve();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}