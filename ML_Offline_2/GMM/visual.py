import math
import matplotlib.pyplot as plt
import numpy as np
import time

def draw_ellipse(mu, cov):
    cx = mu[0]
    cy = mu[1]
    a = cov[0][0]
    b = cov[1][1]

    t = np.linspace(0, 2 * math.pi, 100)
    plt.plot(cx + a * np.cos(t), cy + b * np.sin(t)) #draw the elipse

    plt.plot(cx, cy, '+', mew=5, ms=10)

def plot_image(filename, xs, ys, labels, mulist, covlist, k, sleep_time = 0):
    fig = plt.figure()
    plt.title(filename)
    plt.scatter(xs, ys, c= labels, marker='x')
    for j in range(k):
        draw_ellipse(mulist[j], covlist[j])
    fig.savefig(filename)
    plt.show()
    time.sleep(sleep_time)

def ioff():
    plt.ioff()

def ion():
    plt.ion()