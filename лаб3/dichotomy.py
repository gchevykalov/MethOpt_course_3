import task as t

class DichotomySolver:
    def __init__(self, task : t.Task):
        self.task = task
        return

    def dichotomySolver(self, eps : float):
        lBorder = self.task.getLeftBorder()
        rBorder = self.task.getRightBorder()
        delta = eps / 100

        while abs(lBorder - rBorder) > eps:
            m = (lBorder + rBorder) / 2
            y1 = self.task.f(m - delta)
            y2 = self.task.f(m + delta)

            if y1 >= y2:
                lBorder = m - delta
            else:
                rBorder = m + delta

        return (lBorder + rBorder) / 2