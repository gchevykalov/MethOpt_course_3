import numpy as np
from general import *


import enum
import copy


EMPTY = -1


class fromWhere(enum.Enum):
    START = -1
    FROM_LEFT = 0
    FROM_BOTTOM = 1
    FROM_RIGHT = 2
    FROM_TOP = 3


def north_west_corner_rule(n : int, m : int, a : list, b : list):
    solution = [[EMPTY for j in range(m)] for i in range(n)]
    northest_row = 0
    a_tmp = [a[i] for i in range(n)]
    b_tmp = [b[i] for i in range(m)]

    for i in range(m):
        for j in range(northest_row, n):
            count = min(a_tmp[j], b_tmp[i])
            if count > 0:
                solution[j][i] = count
                a_tmp[j] = a_tmp[j] - count
                b_tmp[i] = b_tmp[i] - count
                if a_tmp[j] == 0 and b_tmp[i] != 0:
                    northest_row = j + 1
                elif a_tmp[j] != 0 and b_tmp[i] == 0:
                    break
                elif a_tmp[j] == 0 and b_tmp[i] == 0 and (j != n - 1 or i != m - 1):
                    if i != m - 1:
                        solution[j][i + 1] = 0
                    elif j != n - 1:
                        solution[j + 1][i] = 0

    return solution


def print_loop(loop : list, plan : list, iter : int):
    print("Попытка улучшить план №" + str(iter + 1))

    plus = True
    loop_str = ""
    for i in range(len(loop)):
        if plus:
            sign = "+"
        else:
            sign = "-"
        loop_str += "(" + str(loop[i][0]) + ", " + str(loop[i][1]) + ")[" + sign + "] -> "
        plus = not plus
    loop_str += "(" + str(loop[0][0]) + ", " + str(loop[0][1]) + ")"
    print(loop_str)

    for i in range(len(plan)):
        row = [str(plan[i][j]) if plan[i][j] != EMPTY else "*" for j in range(len(plan[0]))]
        print(row)
    return


def look_all_pos_from_right(m : int, loopMap : list, pos_row : int,  pos_column : int):
    result = list()

    for j in range(pos_column + 1, m):
        if loopMap[pos_row][j][0] != EMPTY and loopMap[pos_row][j][1] == -1:
            result.append([pos_row, j])

    return result


def look_all_pos_from_left(loopMap : list, pos_row : int,  pos_column : int):
    result = list()

    for j in range(0, pos_column):
        if loopMap[pos_row][j][0] != EMPTY and loopMap[pos_row][j][1] == -1:
            result.append([pos_row, j])

    return result


def look_all_pos_from_bottom(n : int, loopMap : list, pos_row : int,  pos_column : int):
    result = list()

    for i in range(pos_row + 1, n):
        if loopMap[i][pos_column][0] != EMPTY and loopMap[i][pos_column][1] == -1:
            result.append([i, pos_column])

    return result


def look_all_pos_from_top(loopMap : list, pos_row : int,  pos_column : int):
    result = list()

    for i in range(0, pos_row):
        if loopMap[i][pos_column][0] != EMPTY and loopMap[i][pos_column][1] == -1:
            result.append([i, pos_column])

    return result


def isLoopEndNear(n : int, m : int, loopMap : list, pos_row : int, pos_column : int, recursionLvl : int):
    if recursionLvl < 3:
        return False

    for i in range(m):
        if loopMap[pos_row][i][1] == 0 and i != pos_column:
            return True

    for i in range(n):
        if loopMap[i][pos_column][1] == 0 and i != pos_row:
            return True

    return False


def loop_recursion(n, m, loopMap, last_row, last_col, from_where, recursionLvl, loop, currentWay):
    loopMap[last_row][last_col][1] = recursionLvl
    currentWay.append([last_row, last_col])

    # если оказались рядом с началом -> возвращаем цикл
    if isLoopEndNear(n, m, loopMap, last_row, last_col, recursionLvl):
        loop.append(copy.deepcopy(currentWay))
        return
    else:
        # ищем цикл справа
        if from_where != fromWhere.FROM_RIGHT and from_where != fromWhere.FROM_LEFT:
            waysFromRight = look_all_pos_from_right(m, loopMap, last_row, last_col)
            for way in waysFromRight:
                loop_recursion(n, m, loopMap, way[0], way[1], fromWhere.FROM_RIGHT, recursionLvl + 1, loop, currentWay)
         # ищем цикл сверху
        if from_where != fromWhere.FROM_TOP and from_where != fromWhere.FROM_BOTTOM:
            waysFromTop = look_all_pos_from_top(loopMap, last_row, last_col)
            for way in waysFromTop:
                loop_recursion(n, m, loopMap, way[0], way[1], fromWhere.FROM_TOP, recursionLvl + 1, loop, currentWay)
        # ищем цикл слева
        if from_where != fromWhere.FROM_RIGHT and from_where != fromWhere.FROM_LEFT:
            waysFromLeft = look_all_pos_from_left(loopMap, last_row, last_col)
            for way in waysFromLeft:
                loop_recursion(n, m, loopMap, way[0], way[1], fromWhere.FROM_LEFT, recursionLvl + 1, loop, currentWay)
        # ищем цикл снизу
        if from_where != fromWhere.FROM_TOP and from_where != fromWhere.FROM_BOTTOM:
            waysFromBottom = look_all_pos_from_bottom(n, loopMap, last_row, last_col)
            for way in waysFromBottom:
                loop_recursion(n, m, loopMap, way[0], way[1], fromWhere.FROM_BOTTOM, recursionLvl + 1, loop, currentWay)

    # все поиски закончены -> зачищаем свое наличие в карте и в текущем пути
    loopMap[last_row][last_col][1] = -1
    currentWay.pop(len(currentWay) - 1)
    return


def find_loop(n : int, m : int, plan : list, start_row : int, start_col : int):
    # создаем двумерны массив пар вида (клетка плана, номер шага в рассматриваемом пути)
    loopMap = [[[plan[i][j], -1] for j in range(m)] for i in range(n)]
    loop = list()

    way = list()
    loop_recursion(n, m, loopMap, start_row, start_col, fromWhere.START, 0, loop, way)

    return loop[0]


def minimum_in_loop(plan : list, loop : list):
    min = 1e+15
    for i in range(len(loop)):
        if plan[loop[i][0]][loop[i][1]] < min and plan[loop[i][0]][loop[i][1]] != EMPTY and i % 2 == 1:
            min = plan[loop[i][0]][loop[i][1]]
    return min


def rewrite_plan(plan : list, loop : list):
    sign = 1
    index = 0
    min = minimum_in_loop(plan, loop)

    wasNewZero = False
    while index != len(loop):
        if plan[loop[index][0]][loop[index][1]] == EMPTY:
            plan[loop[index][0]][loop[index][1]] = min
        else:
            plan[loop[index][0]][loop[index][1]] = plan[loop[index][0]][loop[index][1]] + sign * min
            if plan[loop[index][0]][loop[index][1]] == 0 and wasNewZero == False:
                wasNewZero = True
                plan[loop[index][0]][loop[index][1]] = EMPTY
        sign = sign * -1
        index = index + 1

    return plan


def get_potencials(n : int, m : int, plan : list, cost : list):
    # составляем СЛАУ и решаем при помоще np.solve
    A = [[0 for j in range(m + n)] for i in range(m + n)]
    b = [0 for i in range(m + n)]

    A[0][0] = 1
    b[0] = 0
    var = 1
    for i in range(n):
        for j in range(m):
            if plan[i][j] != EMPTY:
                A[var][i] = -1
                A[var][n + j] = 1
                b[var] = cost[i][j]
                var = var + 1
    x = np.linalg.solve(A, b)
    
    u = x[:n]
    v = x[n:]

    return u, v

# Проверка на оптимальность
# если возвращается пустой массив - то решение оптимально
# иначе - координаты точки (row, column), с которой нужно будет строить цикл
def check_plan_optimal(n : int, m : int, cost : list, plan : list):
    u, v = get_potencials(n, m, plan, cost)

    max_delta = -1
    new_point = []
    for i in range(n):
        for j in range(m):
            if plan[i][j] == EMPTY and v[j] - u[i] > cost[i][j]:
                if v[j] - u[i] - cost[i][j] > max_delta:
                    new_point = [i, j]
                    max_delta = v[j] - u[i] - cost[i][j]

    return new_point


def replace_empty_with_zero(n : int, m : int, plan : list):
    for i in range(n):
        for j in range(m):
            if plan[i][j] == EMPTY:
                plan[i][j] = 0
    return plan


def potencial_solution(n : int, m : int, a : list, b : list, cost : list):
    plan = north_west_corner_rule(n, m, a, b)

    new_point = check_plan_optimal(n, m, cost, plan)
    iter = 0
    while len(new_point) != 0:
        loop = find_loop(n, m, plan, new_point[0], new_point[1])
        print_loop(loop, plan, iter)
        iter += 1
        plan = rewrite_plan(plan, loop)
        new_point = check_plan_optimal(n, m, cost, plan)

    return replace_empty_with_zero(n, m, plan)

if __name__ == "__main__":
    pass