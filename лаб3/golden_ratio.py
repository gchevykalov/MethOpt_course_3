import math as m
import task as t

class GoldenRatioSolver:
    def __init__(self, task):
        self.task = task
        self.GOLDEN_RATIO_CONST = (3 - m.sqrt(5)) / 2
        return

    def goldenRatioSolver(self, eps : float):
        lBorder = self.task.getLeftBorder()
        rBorder = self.task.getRightBorder()
        isLess = None

        while abs(lBorder - rBorder) > eps:
            if isLess is None:
                x1 = lBorder + self.GOLDEN_RATIO_CONST * (rBorder - lBorder)
                x2 = lBorder + (1 - self.GOLDEN_RATIO_CONST) * (rBorder - lBorder)
                y1 = self.task.f(x1)
                y2 = self.task.f(x2)
            elif isLess:
                x2 = x1
                x1 = lBorder + self.GOLDEN_RATIO_CONST * (rBorder - lBorder)
                y2 = y1
                y1 = self.task.f(x1)
            else:
                x1 = x2
                x2 = lBorder + (1 - self.GOLDEN_RATIO_CONST) * (rBorder - lBorder)
                y1 = y2
                y2 = self.task.f(x2)
            
            if y1 >= y2:
                lBorder = x1
                isLess = False
            else:
                rBorder = x2
                isLess = True

        return (lBorder + rBorder) / 2