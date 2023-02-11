#pragma once

#include <iostream>
//#include <iomanip> // ������ ����� �� ����� ����� EPEM::print
#include <vector>

using std::vector;

class EPEM {  // ������� ������� �����
private:
    vector<vector<double>> A_c;   // ������� ��� ������ � ������������ �����
    int n_c, m;                   // ������� ������� A_c
    vector<double> b_c, c_c, x_c; // ������ �����, ������������ ������� ������� � ������ ������� ������ � ������������ �����
    vector<size_t> uv;            // ������ ��� ����� ��������������� ���������� ��� �������������� ����� ������ (�������� ������� (� 0) ��������� ��� ����������� �� ����)
    int wcount;                   // ���������� ��������������� ����������, ����������� ��� �������������� '>='->'='
    vector<double> x;             // ������ ������� ������ � �������� ����
    vector<vector<double>> Am_m;  // ���������� �������, �� ������� ������ ������� ������
    double C_x_c;                 // �������� ������� ����

    bool checkTask(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr);

    // ��� ������� A �������������� ���������� ����������� �������� - ������� ������, ����� �������
    void convertTaskFrom(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr); // �������������� A->A_c, c->c_c

    inline int grayCode(int n) {
        return n ^ (n >> 1);
    }

    inline int countBits(int n) {
        int res = 0;
        for (; n; n >>= 1)
            res += n & 1;
        return res;
    }

    bool gauss(vector<double>& res); // ����� ������ ������� ����

    void convertSolution(); // �������������� x_c -> x
public:
    EPEM() : m(0), n_c(0), wcount(0), C_x_c(0) {};

    vector<double>& Resolve(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
        const vector<char>& signOfExpr, const vector<size_t>& noSignRestr); // �������� �����
};