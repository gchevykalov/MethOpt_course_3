from math import exp

class Task:
    def __init__(self):
        self.dimention = 2
        self.answer = [0, 0]
        self.n = 0
        return

    # INITS BY USER
    def f(self, x : list) -> float:
        # RETURN VALUE OF FUNC
        x1 = x[0]
        x2 = x[1]
        self.n += 1
 
        return 2 * (x1 ** 2) + 3 * (x2 ** 2) + exp((x1 ** 2) + (x2 ** 2))

    # INITS BY USER
    def grad_f(self, x : list) -> list:
        # RETURN VEC OF DIFFS.
        x1 = x[0]
        x2 = x[1]

        df_1 = 2 * x1 * (exp((x1 ** 2) + (x2 ** 2)) + 2)
        df_2 = 2 * x2 * (exp((x1 ** 2) + (x2 ** 2)) + 3)
        return [df_1, df_2]

    # INITS BY USER
    def secGrad_f(self, x : list) -> list:
        # RETURN VEC OF SECOND DIFFS.
        x1 = x[0]
        x2 = x[1]

        df_11 = 4 * (x1 ** 2) * exp((x1 ** 2) + (x2 ** 2)) + 2 * exp((x1 ** 2) + (x2 ** 2)) + 4
        df_12 = 4 * x1 * x2 * exp((x1 ** 2) + (x2 ** 2))
        df_21 = 4 * x1 * x2 * exp((x1 ** 2) + (x2 ** 2))
        df_22 = 4 * (x2 ** 2) * exp((x1 ** 2) + (x2 ** 2)) + 2 * exp((x1 ** 2) + (x2 ** 2)) + 6
        return [[df_11, df_12], [df_21, df_22]]