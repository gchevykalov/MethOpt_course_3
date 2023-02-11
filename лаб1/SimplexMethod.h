#pragma once

//#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using std::vector;

#define EPS 0.00000000001

class SimplexMethod {
private:
    vector<vector<double>> A_c;   // ������� ��� ������ � ������������ �����
    int n_c, m_c;                 // ������� ������� A_c
    vector<double> b_c, c_c, x_c; // ������ �����, ������������ ������� ������� � ������ ������� ������ � ������������ �����
    vector<size_t> uv;            // ������ ��� ����� ��������������� ���������� ��� �������������� ����� ������ (�������� ������� (� 0) ��������� ��� ����������� �� ����)
    vector<int> B, N;             // ������� � ��������� �������� � ���������� ����������
    double coef;                  // �������������� ���������� ���� � ������� �������
    vector<double> x;             // ������ ������� ������ � �������� ����

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