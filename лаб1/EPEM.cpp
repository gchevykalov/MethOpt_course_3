#include "EPEM.h"

using std::vector;

bool EPEM::checkTask(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
    const vector<char>& signOfExpr, const vector<size_t>& noSignRestr) {
    if (A.size() != b.size())
        return false;
    for (auto& a : A)
        if (a.size() != c.size())
            return false;
    if (signOfExpr.size() != b.size())
        return false;
    if (noSignRestr.size() > c.size())
        return false;

    return true;
}

void EPEM::convertTaskFrom(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
    const vector<char>& signOfExpr, const vector<size_t>& noSignRestr) {
    b_c = b; // ����� �� ����������

    uv = noSignRestr;
    c_c = c;
    for (auto u : uv) {
        c_c.push_back(-c[u]); // ��������� � ����� �������� ����������� ��-�� ������ xi = ui - vi
    }

    m = b.size();   // ����� ����� �������� �� �����
    n_c = c.size(); // ����������� ������, �� �������� ����� ���������� (����� ��������)
    A_c = A;
    for (auto u : uv) // ��������� � ����� ������� ����������� ��-�� ������ xi = ui - vi
        for (int i = 0; i < m; ++i)
            A_c[i].push_back(-A_c[i][u]);

    n_c += uv.size(); // ����� �������� ����� ������ ����������
    int index = -1;
    wcount = 0;
    for (auto s : signOfExpr) { // ��������� � ����� ������� ����������� ��-�� ���������� ���������� �� ����������
        int coef = -1;
        ++index;

        if (s == '=')
            continue;
        if (s == '<')
            coef = 1;

        for (int i = 0; i < m; ++i)
            if (i == index)
                A_c[i].push_back(coef);
            else
                A_c[i].push_back(0);

        ++wcount;
    }
    n_c += wcount;
    for (int i = 0; i < wcount; ++i)
        c_c.push_back(0); // ��������� 0 ���������� wi
}

void EPEM::convertSolution() {
    int n = n_c - wcount - uv.size(); // ����������� ������� ������� ������ � �������� �����

    x.insert(x.begin(), x_c.begin(), x_c.begin() + n);

    int i = n;
    for (auto u : uv) { // ������������ xi = ui - vi
        x[u] -= x_c[i++];
    }
}

bool EPEM::gauss(vector<double>& res) {
    double max;
    int k = 0, index;
    vector<vector<double>> Atmp = Am_m;
    vector<double> btmp = b_c;
    res.resize(m);

    while (k < m) {
        // ����� ������ � ������������ a[i][k]
        max = abs(Atmp[k][k]);
        index = k;
        for (int i = k + 1; i < m; ++i) {
            if (abs(Atmp[i][k]) > max) {
                max = abs(Atmp[i][k]);
                index = i;
            }
        }
        if (max == 0)
            return false;

        Atmp[k].swap(Atmp[index]);
        double tmp = btmp[k];
        btmp[k] = btmp[index];
        btmp[index] = tmp;
        // ������������ ���������
        for (int i = k; i < m; ++i) {
            double tmp = Atmp[i][k];
            if (tmp == 0)
                continue; // ��� �������� ������������ ����������
            for (int j = 0; j < m; ++j)
                Atmp[i][j] /= tmp;
            btmp[i] /= tmp;
            if (i == k)
                continue; // ��������� �� �������� ���� �� ����
            for (int j = 0; j < m; ++j)
                Atmp[i][j] -= Atmp[k][j];
            btmp[i] -= btmp[k];
        }
        k++;
    }
    // �������� �����������
    for (k = m - 1; k >= 0; --k) {
        res[k] = btmp[k];
        for (int i = 0; i < k; i++)
            btmp[i] = btmp[i] - Atmp[i][k] * res[k];
    }
    return true;
}

vector<double>& EPEM::Resolve(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
    const vector<char>& signOfExpr, const vector<size_t>& noSignRestr) {
    if (!checkTask(A, b, c, signOfExpr, noSignRestr))
        return x;

    convertTaskFrom(A, b, c, signOfExpr, noSignRestr);

    int prev = 0;
    vector<int> selected;
    for (int i = 0; i < (1 << n_c); ++i) { // ���������� ���������� ������� m*m, ��� ��� ���������� ��� �������� ����� ��������
        int cur = grayCode(i);
        if (countBits(cur) == m) { // ���������� �������
            if (selected.empty()) {
                for (int j = 0; j < n_c; ++j)
                    if (cur & (1 << j)) // ��������� ������ �������
                        selected.push_back(j);

                Am_m.resize(m);
                for (auto s : selected)
                    for (int k = 0; k < m; ++k)
                        Am_m[k].push_back(A_c[k][s]);
            }
            else {
                int dif = cur ^ prev;
                int k = -1, l = -1;
                for (int j = 0; j < n_c; ++j)
                    if (dif & (1 << j)) {
                        if (k == -1)
                            k = j;
                        else {
                            l = j;
                            break;
                        }
                    }
                for (int i = 0; i < selected.size(); ++i) {
                    if (selected[i] == k) {
                        selected[i] = l;
                        for (int j = 0; j < m; ++j)
                            Am_m[j][i] = A_c[j][l];
                        break;
                    }
                    if (selected[i] == l) {
                        selected[i] = k;
                        for (int j = 0; j < m; ++j)
                            Am_m[j][i] = A_c[j][k];
                        break;
                    }
                }
            }

            vector<double> res;
            if (gauss(res)) {
                bool adm = true;
                for (auto c : res)
                    if (c < 0)
                        adm = false;
                if (adm) {
                    if (x_c.empty()) {
                        x_c.resize(n_c, 0);
                        int i = 0;
                        for (auto s : selected) {
                            x_c[s] = res[i];
                            ++i;
                        }
                        for (int i = 0; i < n_c; ++i)
                            C_x_c += x_c[i] * c_c[i];
                    }
                    else {
                        vector<double> tmp;
                        tmp.resize(n_c, 0);
                        int i = 0;
                        for (auto s : selected) {
                            tmp[s] = res[i];
                            ++i;
                        }

                        double C_tmp = 0;
                        for (int i = 0; i < n_c; ++i)
                            C_tmp += tmp[i] * c_c[i];
                        if (C_tmp < C_x_c) {
                            C_x_c = C_tmp;
                            x_c = tmp;
                        }
                    }
                }
            }
            prev = cur;
        }
    }

    if (!x_c.empty())
        convertSolution();
    return x;
}