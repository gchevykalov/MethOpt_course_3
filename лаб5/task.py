from math import exp

class Task:
    def __init__(self):
        self.dimention = 3
        self.answer = [0, 0, 0]

        # limits with '<= 0' sign
        self.limits = [
            lambda x : x[0] ** 2 + x[1] ** 2 + x[2] ** 2 - 4,
            lambda x : - x[2] + x[0] ** 2 + x[1] ** 2 - 1,
            lambda x : x[1] ** 2 - 0.49,
        ]

        # gradients for limits with '<= 0' sign
        self.d_limits = [
            lambda x : [2 * x[0],
                        2 * x[1],
                        2 * x[2]],
            lambda x : [2 * x[0],
                        2 * x[1],
                        -1],
            lambda x : [0,
                        2 * x[1],
                        0]
        ]

        # limits with '= b' sign
        self.A = [[1, 1, -1]]
        self.b = [-1] 
        return

    # INITS BY USER
    def f(self, x : list) -> float:
        # RETURN VALUE OF FUNC
        x1 = x[0]
        x2 = x[1]
        x3 = x[2]
        return 2 * (x1 ** 2) + 3 * (x2 ** 2) + exp((x1 ** 2) + (x2 ** 2) + ((x3 - 1) ** 2))
        
    # INITS BY USER
    def grad_f(self, x : list) -> list:
        # RETURN VEC OF DIFFS.
        x1 = x[0]
        x2 = x[1]
        x3 = x[2]

        df_1 = 2 * x1 * (exp((x1 ** 2) + (x2 ** 2) + ((x3 - 1) ** 2)) + 2)
        df_2 = 2 * x2 * (exp((x1 ** 2) + (x2 ** 2) + ((x3 - 1) ** 2)) + 3)
        df_3 = 2 * (x3 - 1) * exp((x1 ** 2) + (x2 ** 2) + ((x3 - 1) ** 2))
        return [df_1, df_2, df_3]