# test.py
# Alex Hergenhan 2024

import numpy as np
import matplotlib.pyplot as plt 
import scipy

layer = np.array([0,1,2,3,4,5,6,7])
wire = np.array([2,2,2,2,2,2,2,2])
tdc = np.array([9,190,59,121,129,51,198,17])
DIVIDER = 900

def conv_hit_to_coords(l : np.array, w : np.array):
    # if l is odd, add 0.5 to it
    x = l
    y = w+ 0.5*(l%2)
    return (x, y)


def get_basic_plot():
    fig = plt.figure(figsize=(8,8))
    ax = fig.add_subplot(111)

    for i in range(8):
        for j in range(8):
            x, y = conv_hit_to_coords(i,j)
            ax.plot(x, y, 'k.')

    x, y = conv_hit_to_coords(layer, wire)
    ax.plot(x, y, 'rx')
    ax.set(xlabel='Layer', ylabel='Wire',)
    return ax

def plot_TDC_circles(ax, v=1):
    
    for i in range(8):
        x, y = conv_hit_to_coords(layer[i], wire[i])
        ax.add_artist(plt.Circle((x,y), tdc[i] / DIVIDER * v, color='b', fill=False))
    
    return ax


# ax + bY + c = 0
class line:
    def __init__(self, a , b, c):
        self.a = a
        self.b = b
        self.c = c
    
    def plot(self, ax, xmin, xmax):
        x = np.linspace(xmin, xmax, 100)
        y = (-self.a * x - self.c) / self.b
        ax.plot(x, y)
        return ax

class circle:
    def __init__(self, x, y, r):
        self.x = x
        self.y = y
        self.r = r
    
    def plot(self, ax):
        ax.add_artist(plt.Circle((self.x, self.y), self.r, color='b', fill=False))
        return ax

def get_tangents_to_two_circles(c1, c2):
    ls = []

    # four solutions
    for i in [-1, 1]:
        for j in [-1, 1]:
            r1 = c1.r * i
            r2 = c2.r * j

            dr = r2 - r1

            x = c2.x - c1.x
            y = c2.y - c1.y

            z = x**2 + y**2
            d = np.sqrt( np.abs(z - dr**2) )

            a = (x * dr + y*d)/z
            b = (y * dr - x*d)/z
            c = r1 - c1.x * a - c1.y * b

            l = line(a, b, c)  
            ls.append(l) 

    return ls



# now do minimisation until all 8 circles touch ONE of these tangent lines
from scipy.optimize import minimize

# check if a cirlce is touching a line
def min_circle_line_dist(c, l):
    return np.abs(l.a * c.x + l.b * c.y + l.c) / np.sqrt(l.a**2 + l.b**2) - c.r

def f(v):
    # drifttime * v = radius

    c1 = circle(conv_hit_to_coords(layer[0], wire[0])[0], conv_hit_to_coords(layer[0], wire[0])[1], tdc[0] / DIVIDER * v)
    c2 = circle(conv_hit_to_coords(layer[7], wire[7])[0], conv_hit_to_coords(layer[7], wire[7])[1], tdc[7] / DIVIDER * v)
    ls = get_tangents_to_two_circles(c1, c2)
    return check_all_lines(ls, v)


def check_all_lines(ls, v):
    dist = np.array([])
    
    for l in ls:

        for i in range(1,6): # dont need to check the first and last circle
            # check if the circle is touching the line
            c = circle(conv_hit_to_coords(layer[i], wire[i])[0], conv_hit_to_coords(layer[i], wire[i])[1], tdc[i] / DIVIDER * v)
            dist = np.append(dist, min_circle_line_dist(c, l))
        
        # we want to minimise it such that all distances are < 0.001
    total_dist = np.sum(np.abs(dist))
    return total_dist # wanna minimise this!




res = minimize(f, 1, method='Nelder-Mead')
print(res.x)

v = res.x / DIVIDER
print(v)



ax = plot_TDC_circles(get_basic_plot(), v*DIVIDER)
# get the tangent to the first and last circle
c1 = circle(conv_hit_to_coords(layer[0], wire[0])[0], conv_hit_to_coords(layer[0], wire[0])[1], tdc[0] / DIVIDER * v)
c2 = circle(conv_hit_to_coords(layer[7], wire[7])[0], conv_hit_to_coords(layer[7], wire[7])[1], tdc[7] / DIVIDER * v)
ls = get_tangents_to_two_circles(c1, c2)
for l in ls:
    ax = l.plot(ax, 0, 8)

plt.show()


# end test.py
