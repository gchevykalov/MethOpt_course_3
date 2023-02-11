import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D

import numpy as np
import csv

import task as t
import StepDiv as gwfs
import BFGS as gbfgs

def taskPlot():
    x = np.arange(-1, 1, 0.01)
    y = np.arange(-1, 1, 0.01)
    xgrid, ygrid = np.meshgrid(x, y)
    zgrid = 2 * (xgrid ** 2) + 3 * (ygrid ** 2) + np.exp((xgrid ** 2) + (ygrid ** 2))

    fig = plt.figure(figsize=(7, 4))
    ax_3d = Axes3D(fig)

    ax_3d.plot_surface(xgrid, ygrid, zgrid, rstride=5, cstride=5, cmap=cm.jet)
    plt.show()
    return

def printResult(steps : list, iters : int, fres : float, eps : float, methodName : str, n : int):
    print(methodName)
    print("Eps: " + str(eps))
    print("Res: " + str(steps[len(steps) - 1]))
    print("f(Res): " + str(fres))
    print("Iters: " + str(iters))
    print("Function calls: " + str(n))
    return

def plotSteps(steps : list):
    xCoord = np.arange(-1, 1.2, 0.01)
    yCoord = np.arange(-1, 1.2, 0.01)
    x, y = np.meshgrid(xCoord, yCoord)
    z = 2 * (x ** 2) + 3 * (y ** 2) + np.exp((x ** 2) + (y ** 2))

    fig, ax = plt.subplots()

    fig.set_figwidth(8)     #  ширина и
    fig.set_figheight(8)    #  высота "Figure"

    xSteps = [step[0] for step in steps]
    ySteps = [step[1] for step in steps]
    plt.plot(xSteps, ySteps, "bo--")
    cs = plt.contour(x, y, z)
    plt.clabel(cs)
    plt.show()
    return

def writeSteps(method, steps: list, eps, task: t.Task):
    with open(method + '_' + str(eps).replace('.', '_') + '.csv', 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            for point in steps:
                writer.writerow([point[0], point[1], task.f(point)])

def main():
    EPS = [0.01]
    currentTask = t.Task()

    taskPlot()
    gradWFragStep = gwfs.GradWFragStep(currentTask)
    grafBFGS = gbfgs.GrafBFGS(currentTask)

    for ep in EPS:
        currentTask.n = 0
        steps, iters = gradWFragStep.gradWFragStepSolver(ep)
        printResult(steps, iters, currentTask.f(steps[len(steps) - 1]), ep, "GRADIENT WITH FRAG STEP", currentTask.n)
        writeSteps('StepDiv', steps, ep, currentTask)
        if (ep == EPS[len(EPS) - 1]):
            plotSteps(steps)

    for ep in EPS:
        currentTask.n = 0
        steps, iters = grafBFGS.grafBFGSSolver(ep)
        printResult(steps, iters, currentTask.f(steps[len(steps) - 1]), ep, "BFGS METHOD", currentTask.n)
        writeSteps('BFGS', steps, ep, currentTask)
        if (ep == EPS[len(EPS) - 1]):
            plotSteps(steps)

main()