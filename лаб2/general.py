def to_close(n : int, m : int, a : list, b : list, cost : list):
    sum_supply = sum(a)
    sum_demand = sum(b)
    
    if (sum_demand < sum_supply):
        print("Избыток -> добавляем фиктивного потребителя")
        for cost_row in cost:
            cost_row.append(0)
        m = m + 1
        b.append(sum_supply - sum_demand)
    elif (sum_demand > sum_supply):
        print("Недостаток -> добавляем фиктивного поставщика")
        cost.append([0 for i in range(m)])
        n = n + 1
        a.append(sum_demand - sum_supply)
    else:
        print("Введенная задача является закрытой")
    return n, m, a, b, cost


def print_task(n : int, m : int, a : list, b : list, cost : list):
    print("Поставленная задача:")
    matr = [list() for i in range(n + 2)]

    for i in range(len(matr)):
        if i == 0 or i == n + 1:
            matr[i].append('   ')
        else:
            spaces = ''
            while len(str(i - 1) + spaces) < 2:
                spaces += ' '
            matr[i].append('a' + str(i - 1) + spaces)

    for i in range(m):
        spaces = ''
        while len(str(i) + spaces) < 2:
                spaces += ' '
        matr[0].append('b' + str(i) + spaces)

    for i in range(len(matr) - 2):
        for j in range(len(matr[0]) - 1):
            spaces = ''
            while len(str(cost[i][j]) + spaces) < 3:
                spaces += ' '
            matr[i + 1].append(str(cost[i][j]) + spaces)

    for i in range(len(b)):
        spaces = ''
        while len(str(b[i]) + spaces) < 3:
            spaces += ' '
        matr[n + 1].append(str(b[i]) + spaces)

    for i in range(len(matr)):
        if i == 0 or i == n + 1:
            matr[i].append('   ')
        else:
            spaces = ''
            while len(str(a[i - 1]) + spaces) < 3:
                spaces += ' '
            matr[i].append(str(a[i - 1]) + spaces)

    for i in range(len(matr)):
        print(matr[i])

    return


def print_result(plan : list, cost : list):
    print("Оптимальный план")
    for i in range(len(plan)):
        rowToPrint = [str(plan[i][j]) for j in range(len(plan[0]))]
        print(rowToPrint)

    res = 0
    for i in range(len(plan)):
        for j in range(len(plan[0])):
            res += plan[i][j] * cost[i][j]
    print("Итоговая стоимость: " + str(res))
    return


if __name__ == "__main__":
    pass