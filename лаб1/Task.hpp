#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "EPEM.h"
#include "SimplexMethod.h"

class Task {
private:
    std::vector<std::vector<double>> matrix;
    std::vector<double> vectorC;
    std::vector<double> vectorB;
    std::vector<char> signOfExpr;
    std::vector<size_t> noSignRestr;
    std::vector<double> x;
    bool isMax;
    double functionValue;

    Task(const std::vector<std::vector<double>>& matrix, const std::vector<double>& C, const std::vector<double>& B,
            const std::vector<char>& signOfExpr, const std::vector<size_t>& noSignRestr, bool isMax) :
        matrix(matrix), vectorB(B), vectorC(C), signOfExpr(signOfExpr), noSignRestr(noSignRestr), isMax(isMax), functionValue(0) {}

    void FunctionCalc() {
        functionValue = 0;
        for (size_t i = 0; i < x.size(); ++i)
            functionValue += (vectorC[i] * x[i]);
        if (isMax)
            functionValue *= -1;
    }
public:
    static Task LoadTask(std::string filename) {
        vector<vector<double>> a;
        vector<double> c, b;
        vector<char> h;
        vector<size_t> s;
        bool findMax = false;
        std::ifstream file(filename);
        std::string line;
        
        std::getline(file, line);
        {
            std::vector<std::string> result;
            std::istringstream iss(line);
            
            for (std::string s; iss >> s; )
                result.push_back(s);
            
            for (int i = 1; i < result.size(); ++i)
                c.push_back(std::stod(result[i]));
            
            if (result[0] == "max") {
                for (auto& el : c)
                    el *= -1;
                findMax = true;
            }
        }
        
        std::vector<bool> restr;
        restr.resize(c.size(), false);
        std::vector<size_t> rev;
        std::getline(file, line);
        {
            std::istringstream iss(line);
            
            for (std::string s; iss >> s; )
                restr[std::stoi(s)] = true;
        }
        
        std::getline(file, line);
        {
            std::istringstream iss(line);
            
            for (std::string s; iss >> s; ) {
                restr[std::stoi(s)] = true;
                rev.push_back(std::stoi(s));
            }
        }
        
        for (int i = 0; i < restr.size(); ++i)
            if (!restr[i])
                s.push_back(i);
        
        while (std::getline(file, line)) {
            std::vector<std::string> result;
            std::vector<double> row;
            std::istringstream iss(line);
            bool nextIsB = false;
            
            for (std::string s; iss >> s; )
                result.push_back(s);
            
            for (auto& r : result) {
                if (nextIsB) {
                    b.push_back(std::stod(r));
                    nextIsB = false;
                    continue;
                }
                if (r == "=") {
                    h.push_back('=');
                    nextIsB = true;
                }
                else if (r == ">=") {
                    h.push_back('>');
                    nextIsB = true;
                }
                else if (r == "<=") {
                    h.push_back('<');
                    nextIsB = true;
                }
                else
                    row.push_back(std::stod(r));
            }
            a.push_back(row);
        }
        
        for (auto r : rev) {
            for (auto& row : a)
                row[r] *= -1;
            c[r] *= -1;
        }
        
        return Task(a, c, b, h, s, findMax);
    }

    Task GoToDual() {
        Task newTask = *this;

        //—мена направлени€ задачи
        newTask.isMax = !isMax;

        //—начала нужно свести еЄ так чтобы неравенства были в одну сторону
        if (!isMax) {
            for (size_t i = 0; i < newTask.signOfExpr.size(); ++i)
                if (newTask.signOfExpr[i] == '<') {
                    for (int j = 0; j < newTask.matrix[i].size(); ++j)
                        newTask.matrix[i][j] *= -1;
                    newTask.vectorB[i] *= -1;
                    newTask.signOfExpr[i] = '>';
                }
        }
        else {
            for (size_t i = 0; i < newTask.signOfExpr.size(); ++i)
                if (newTask.signOfExpr[i] == '>') {
                    for (int j = 0; j < newTask.matrix[i].size(); ++j)
                        newTask.matrix[i][j] *= -1;
                    newTask.vectorB[i] *= -1;
                    newTask.signOfExpr[i] = '<';
                }
        }

        //«атем помен€ть b и c рол€ми 
        std::swap(newTask.vectorB, newTask.vectorC);

        //» поскольку в двойственной нужно искать уже другое направление, в новой с нужно всЄ на - 1 умножить
        if (!isMax)
            for (size_t i = 0; i < newTask.vectorC.size(); ++i)
                newTask.vectorC[i] *= -1;
        else 
            for (size_t i = 0; i < newTask.vectorB.size(); ++i)
                newTask.vectorB[i] *= -1;

        //ћатрица транспонируетс€
        std::vector<std::vector<double>> tmp;
        tmp.resize(newTask.matrix[0].size());
        for (int i = 0; i < newTask.matrix.size(); ++i)
            for (int j = 0; j < newTask.matrix[0].size(); ++j)
                tmp[j].push_back(newTask.matrix[i][j]);
        newTask.matrix = tmp;
        
        //ќграничени€ на знак получают переменные, индексы которых совпадают с индексами старых неравенств
        std::vector<size_t> newNoSignRestr;
        for (size_t i = 0; i < signOfExpr.size(); ++i)
            if (signOfExpr[i] == '=')
                newNoSignRestr.push_back(i);

        //–авенствами станов€тс€ строки с индексами старых ограничений на знак
        //ј остальные станов€тс€ со знаком неравенства

        //ѕостроение вспомогательного вектора
        std::vector<bool> logicVector(newTask.matrix.size(), true);
        for (auto i : newTask.noSignRestr)
            logicVector[i] = false;

        signOfExpr.resize(newTask.matrix.size());
        for (size_t i = 0; i < logicVector.size(); ++i)
            if (logicVector[i])
                if (!isMax)
                    newTask.signOfExpr[i] = '<';
                else 
                    newTask.signOfExpr[i] = '>';
            else
                newTask.signOfExpr[i] = '=';

        newTask.noSignRestr = newNoSignRestr;

        return newTask;
    }

    void Print() {
        if(!isMax)
            std::cout << "min: ";
        else
            std::cout << "max: ";
        if(!isMax)
            std::cout << vectorC[0] << " * x0";
        else
            std::cout << -vectorC[0] << " * x0";
        for (size_t i = 1; i < matrix.size(); ++i) {
            if (!isMax) {
                if (vectorC[i] >= 0)
                    std::cout << " + " << vectorC[i] << " * x" << i;
                else
                    std::cout << " - " << -vectorC[i] << " * x" << i;
            }
            else {
                if (vectorC[i] <= 0)
                    std::cout << " + " << -vectorC[i] << " * x" << i;
                else
                    std::cout << " - " << vectorC[i] << " * x" << i;
            }
        }
        std::cout << std::endl;
        
        for (size_t i = 0; i < matrix.size(); ++i) {
            if (matrix[i][0] >= 0)
                std::cout << "  " << matrix[i][0] << " * x0";
            else
                std::cout << " " << matrix[i][0] << " * x0";
            for (size_t j = 1; j < matrix[i].size(); ++j)
                if (matrix[i][j] >= 0)
                    std::cout << " + " << matrix[i][j] << " * x" << j;
                else
                    std::cout << " - " << -matrix[i][j] << " * x" << j;
            if(signOfExpr[i] == '=')
                std::cout << " " << signOfExpr[i] << "  " << vectorB[i] << std::endl;
            else
                std::cout << " " << signOfExpr[i] << "= " << vectorB[i] << std::endl;
        }
        vector<bool> s(vectorC.size(), true);
        for (auto i : noSignRestr)
            s[i] = false;
        for(int i = 0; i< vectorC.size(); ++i)
            if(s[i])
                std::cout << "  " << "x" << i << " >= 0" << std::endl;
        std::cout << std::endl;
    }

    void SolveEPEM(){
        EPEM epem;
        x = epem.Resolve(matrix, vectorB, vectorC, signOfExpr, noSignRestr);
        FunctionCalc();
    }

    void SolveSimplex() {
        SimplexMethod simplex;
        x = simplex.Resolve(matrix, vectorB, vectorC, signOfExpr, noSignRestr);
        FunctionCalc();
    }

    void PrintSolve() {
        if (x.empty())
            std::cout << "The problem is unsolvable" << std::endl << std::endl;
        else if (x[0] == HUGE_VAL)
            std::cout << "The goal function is unlimited" << std::endl << std::endl;
        else {
            std::cout << "x = [";
            for (auto xi : x)
                std::cout << std::setprecision(15) << xi << "; ";
            std::cout << "]" << std::endl;
            
            std::cout << "c' * x = " << functionValue << std::endl << std::endl;
        }
    }
};