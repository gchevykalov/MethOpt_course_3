import numpy as np

class Task:
    def __init__(self):
        self.a = 1
        self.b = 2

        self.counter = 0
        self.trueAnswer = 1.259921049894873
        return


    def f(self, x : float):
        someLittleEpsForX = 0.00001

        if (x < self.a - someLittleEpsForX) or (x > self.b + someLittleEpsForX):
            raise Exception("UNRESOLVED X")

        self.counter += 1

        return x + 1 / (x**2)

    def printResult(self, potential_min : float, current_eps : float):
        someLittleEpsForX = 0.00001
        if (potential_min  < self.a - someLittleEpsForX) or (potential_min > self.b + someLittleEpsForX):
            raise Exception("UNRESOLVED X")

        count = self.counter
        print("Eps = " + str(current_eps))
        print("True is " + str(self.trueAnswer))
        print("Potential answer is " + str(potential_min))
        print("Delta: " + str(abs(potential_min - self.trueAnswer)))
        print("f(x) = " + str(self.f(potential_min)))
        print("Delta f: " + str(abs(self.f(potential_min) - self.f(self.trueAnswer))))
        print("Counter: " + str(count))
        return

    def restartCounter(self):
        self.counter = 0
        return

    def getLeftBorder(self):
        return self.a

    def getRightBorder(self):
        return self.b