from task import *
from example1 import *
from example2 import *

from general import *
from potencial_method import *
from extreme_point import *

n, m, a, b, cost = to_close(n, m, a, b, cost)
print_task(n, m, a, b, cost)
#print("Метод потенциалов")
#solution = potencial_solution(n, m, a, b, cost)
#print_result(solution, cost)

print("Метод перебора крайних точек")
min_task, A_matr, b_vec = to_canon(n, m, a, b, cost)
A_matr.pop(len(A_matr) - 8)
b_vec.pop(len(b_vec) - 8)
solution = solve_extreme_point(A_matr, b_vec, min_task)
solution = np.reshape(solution, (n, m))
print_result(solution, cost)

#print("\nПример приведения задачи с избытком к закрытому виду")
#print_task(n1, m1, a1, b1, cost1)
#n1, m1, a1, b1, cost1 = to_close(n1, m1, a1, b1, cost1)
#print_task(n1, m1, a1, b1, cost1)

#print("\nПример приведения задачи с недостатком к закрытому виду")
#print_task(n2, m2, a2, b2, cost2)
#n2, m2, a2, b2, cost2 = to_close(n2, m2, a2, b2, cost2)
#print_task(n2, m2, a2, b2, cost2)