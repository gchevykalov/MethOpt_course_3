from itertools import combinations
import numpy as np


def to_canon(n : int, m : int, a : list, b : list, cost : list):
    min_task = list()
    for i in range(n):
        for j in range(m):
            min_task.append(cost[i][j])

    b_vec = [a[i] if i < n else b[i - n] for i in range(n + m)]
    A_matr = [[0 for i in range(n * m)] for i in range(n + m)]

    for i in range(n):
        for j in range(m):
            A_matr[i][i * m + j] = 1

    for i in range(m):
        for j in range(n):
            A_matr[n + i][i + j * m] = 1

    return min_task, A_matr, b_vec


def get_basis_matrs(A : np.ndarray):
    basis_matrs = []
    combinations_indexes = []
    all_indexes = [i for i in range(A.shape[1])]

    for i in combinations(all_indexes, A.shape[0]):
        basis_matr = A[:, i]
        if np.linalg.det(basis_matr) != 0:
            basis_matrs.append(basis_matr)
            combinations_indexes.append(i)
     
    return basis_matrs, combinations_indexes


def get_vectors(A : list, b : list):
    vectors = []

    if len(A) >= len(A[0]):
        return vectors

    basis_matrs, combinations_indexes = get_basis_matrs(np.array(A))

    for i in range(len(basis_matrs)):
        result = np.linalg.solve(basis_matrs[i], b)
        if (len(result[result < 0]) != 0) or (len(result[result > 1e+15]) != 0):
            continue

        vec = [0 for i in range(len(A[0]))]
        for j in range(len(combinations_indexes[i])):
            vec[combinations_indexes[i][j]] = result[j]
        vectors.append(vec)
    return vectors


def solve_extreme_point(A : list, b : list, c : list):
    vectors = get_vectors(A, b)
    if len(vectors) == 0:
        return []

    solution = vectors[0]
    target_min = np.dot(solution, c)

    for vec in vectors:
        if np.dot(vec, c) <= target_min:
            target_min = np.dot(vec, c)
            solution = vec

    return solution


if __name__ == "__main__":
    pass