#pragma once

#include <iostream>
//#include <iomanip> // убрать когда не нужен будет EPEM::print
#include <vector>

using std::vector;

class EPEM {  // перебор крайних точек
private:
    vector<vector<double>> A_c;   // матрица для задачи в канонической форме
    int n_c, m;                   // размеры матрицы A_c
    vector<double> b_c, c_c, x_c; // правая часть, коэффициенты целевой функции и вектор решения задачи в канонической форме
    vector<size_t> uv;            // вектор для учета вспомогательных переменных при преобразовании формы задачи (содержит индексы (с 0) элементов без ограничения на знак)
    int wcount;                   // количество вспомогательных переменных, необходимых для преобразования '>='->'='
    vector<double> x;             // вектор решения задачи в исходном виде
    vector<vector<double>> Am_m;  // квадратная матрица, по которой ищется опорный вектор
    double C_x_c;                 // значение функции цели

    bool checkTask(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr);

    // для матрицы A предполагается нормальное соответсвие индексов - сначала строка, затем столбец
    void convertTaskFrom(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr); // преобразование A->A_c, c->c_c

    inline int grayCode(int n) {
        return n ^ (n >> 1);
    }

    inline int countBits(int n) {
        int res = 0;
        for (; n; n >>= 1)
            res += n & 1;
        return res;
    }

    bool gauss(vector<double>& res); // метод Гаусса решения СЛАУ

    void convertSolution(); // преобразование x_c -> x
public:
    EPEM() : m(0), n_c(0), wcount(0), C_x_c(0) {};

    vector<double>& Resolve(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr); // решающий метод
};