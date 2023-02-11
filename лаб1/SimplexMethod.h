#pragma once

//#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using std::vector;

#define EPS 0.00000000001

class SimplexMethod {
private:
    vector<vector<double>> A_c;   // матрица для задачи в канонической форме
    int n_c, m_c;                 // размеры матрицы A_c
    vector<double> b_c, c_c, x_c; // правая часть, коэффициенты целевой функции и вектор решения задачи в канонической форме
    vector<size_t> uv;            // вектор для учета вспомогательных переменных при преобразовании формы задачи (содержит индексы (с 0) элементов без ограничения на знак)
    vector<int> B, N;             // вектора с индексами базисных и небазисных переменных
    double coef;                  // необязательный постоянный член в целевой функции
    vector<double> x;             // вектор решения задачи в исходном виде

    bool checkTask(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr);

    void convertTaskFrom(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr);

    void pivot(int l, int e);

    inline bool isEnd() {
        for (auto n : N)
            if (c_c[n] < -EPS)
                return false;
        return true;
    }

    bool init();

    void convertSolution();
public:
    SimplexMethod() : m_c(0), n_c(0), coef(0) {}

    vector<double>& Resolve(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr);
};