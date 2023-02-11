#include "SimplexMethod.h"

using std::vector;

bool SimplexMethod::checkTask(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
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

void SimplexMethod::convertTaskFrom(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
    const vector<char>& signOfExpr, const vector<size_t>& noSignRestr) {
    b_c = b;
    uv = noSignRestr;
    c_c = c;
    for (auto u : uv) {
        c_c.push_back(-c[u]); // ��������� � ����� �������� ����������� ��-�� ������ xi = ui - vi
    }

    m_c = b.size(); // ����������� ������, �� ���������� � ������������� ���� ����� ������������� �������������� � ������������ ����� (����� �����)
    n_c = c.size(); // ����������� ������, �� �������� ����� ���������� (����� ��������)
    A_c = A;
    for (auto u : uv) // ��������� � ����� ������� ����������� ��-�� ������ xi = ui - vi
        for (int i = 0; i < m_c; ++i)
            A_c[i].push_back(-A_c[i][u]);

    n_c += uv.size(); // ����� �������� ����� ������ ����������
    int index = -1;
    for (auto s : signOfExpr) { // ��������� � ����� ������� ����������� ��-�� ���������� ���������� �� ����������
        ++index;
        if (s == '<')
            continue;
        if (s == '>') {
            b_c[index] *= -1;
            for (auto& a : A_c[index])
                a *= -1;
        }
        else {
            vector<double> tmp;
            for (auto a : A_c[index])
                tmp.push_back(-a);
            A_c.push_back(tmp);
            b_c.push_back(-b_c[index]);
            ++m_c;
        }
    }
    for (int i = 0; i < m_c; ++i)
        c_c.push_back(0); // ��������� 0 ���������� ��������������� ����������
}

void SimplexMethod::pivot(int l, int e) { // l - ���������, e - �������� -- ������������ ������� ������\������� � A_c ������ �������� ����������
    b_c[l] /= A_c[l][e];
    for (int j = 0; j < n_c; ++j) {
        if (j == e)
            continue;

        A_c[l][j] /= A_c[l][e];
    }
    A_c[l][e] = 1 / A_c[l][e];

    for (int i = 0; i < m_c; ++i) {
        if (i == l)
            continue;

        b_c[i] -= A_c[i][e] * b_c[l];
        for (int j = 0; j < n_c; ++j) {
            if (j == e)
                continue;

            A_c[i][j] -= A_c[i][e] * A_c[l][j];
        }
        A_c[i][e] = -A_c[i][e] * A_c[l][e];
    }

    coef += c_c[N[e]] * b_c[l];     // ����� ��� ������������ ������� ����������, � �� �����\��������, ������� N[e] � B[l]
    for (int j = 0; j < n_c; ++j) {
        if (j == e)
            continue;

        c_c[N[j]] -= c_c[N[e]] * A_c[l][j];
    }
    c_c[B[l]] = -c_c[N[e]] * A_c[l][e];
    c_c[N[e]] = 0;

    int tmp = N[e];
    N[e] = B[l];
    B[l] = tmp;
}

bool SimplexMethod::init() {
    int k = 0;
    for (int i = 0; i < m_c; ++i)
        if (b_c[i] < b_c[k])
            k = i;

    for (int i = 0; i < n_c; ++i)
        N.push_back(i);
    for (int i = n_c; i < n_c + m_c; ++i)
        B.push_back(i);
    coef = 0;
    if (b_c[k] >= 0) // ����������� ���������� �������� ������� ���������
        return true;

    vector<double> ctmp = c_c;
    c_c.clear();
    c_c.resize(n_c + m_c + 1, 0); //������� ������� ��������������� ������
    c_c[n_c + m_c] = 1;
    N.push_back(n_c + m_c); //���������� ���������� ��������������� ������
    for (auto& a : A_c) // ������� ��������������� ������
        a.push_back(-1.0);
    ++n_c;

    pivot(k, n_c - 1);

    vector<double> tmp;
    tmp.resize(m_c, 0);
    while (!isEnd()) {
        int e = -1;
        for (int i = 0; i < n_c; ++i) // �������� ���������� ������, ��� �������� ����������� ������� ������� �����������
            if (e == -1 && c_c[N[i]] < -EPS || c_c[N[i]] < -EPS && N[i] < N[e])
                e = i;

        for (int i = 0; i < m_c; ++i)
            if (A_c[i][e] > EPS)
                tmp[i] = b_c[i] / A_c[i][e];
            else
                tmp[i] = HUGE_VAL;

        int l = 0;
        for (int j = 0; j < m_c; ++j) // �������� ���������� ������, ��� �������� ��������� ����������
            if (tmp[j] == tmp[l] && B[j] < B[l] || tmp[j] < tmp[l])
                l = j;

        if (tmp[l] == HUGE_VAL) {
            x.push_back(HUGE_VAL);
            return true;
        }
        else
            pivot(l, e);
    }

    int index = 0;
    bool inB = false;
    for (int i = 0; i < m_c; ++i)
        if (B[i] == n_c + m_c - 1) {
            inB = true;
            index = i;
        }

    if (!inB || abs(b_c[index]) < EPS) {
        if (inB) {
            for (int j = 0; j < n_c; ++j)
                if (abs(A_c[index][j]) > EPS) {
                    pivot(index, j);
                    break;
                }
        }

        for (int j = 0; j < n_c; ++j) // ��������� ��������������� ����������
            if (N[j] == n_c + m_c - 1)
                index = j;
        N.erase(N.begin() + index);
        for (auto& a : A_c)
            a.erase(a.begin() + index);
        --n_c;

        c_c = ctmp; // ��������������� �������� ������� �������
        coef = 0;
        int chng = 0;
        for (auto b : B) {
            if (abs(c_c[b]) > EPS) {
                coef += c_c[b] * b_c[chng];
                for (int j = 0; j < n_c; ++j)
                    c_c[N[j]] -= c_c[b] * A_c[chng][j];
                c_c[b] = 0;
            }
            ++chng;
        }

        return true;
    }

    return false;
}

void SimplexMethod::convertSolution() {
    int n = n_c - uv.size(); // ����������� ������� ������� ������ � �������� �����

    x.insert(x.begin(), x_c.begin(), x_c.begin() + n);

    int i = n;
    for (auto u : uv) { // ������������ xi = ui - vi
        x[u] -= x_c[i++];
    }
}

vector<double>& SimplexMethod::Resolve(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c,
    const vector<char>& signOfExpr, const vector<size_t>& noSignRestr) {
    if (!checkTask(A, b, c, signOfExpr, noSignRestr))
        return x;

    convertTaskFrom(A, b, c, signOfExpr, noSignRestr);

    if (!init()) // ������� ����������, ���������� ������ ������
        return x;

    if (!x.empty()) // ������� ������� ������������, ���������� ������ � ����� HUGE_VAL
        return x;

    vector<double> tmp;
    tmp.resize(m_c, 0);
    while (!isEnd()) {
        int e = -1;
        for (int i = 0; i < n_c; ++i) // �������� ���������� ������, ��� �������� ����������� ������� ������� �����������
            if (e == -1 && c_c[N[i]] < -EPS || c_c[N[i]] < -EPS && N[i] < N[e])
                e = i;

        for (int i = 0; i < m_c; ++i)
            if (A_c[i][e] > EPS)
                tmp[i] = b_c[i] / A_c[i][e];
            else
                tmp[i] = HUGE_VAL;

        int l = 0;
        for (int j = 0; j < m_c; ++j) // �������� ���������� ������, ��� �������� ��������� ����������
            if (tmp[j] == tmp[l] && B[j] < B[l] || tmp[j] < tmp[l])
                l = j;

        if (tmp[l] == HUGE_VAL) { // ������� ������� ������������, ���������� ������ � ����� HUGE_VAL
            x.push_back(HUGE_VAL);
            return x;
        }
        else
            pivot(l, e);
    }
    x_c.resize(n_c, 0);
    int index = 0;
    for (auto b : B) {
        if (b < n_c)
            x_c[b] = b_c[index];
        ++index;
    }

    convertSolution();
    return x;
}