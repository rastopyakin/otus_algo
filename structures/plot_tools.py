import numpy as np
import matplotlib.pyplot as plt

def plot_data(fname, ax, name) :
    data  = np.loadtxt(fname, usecols=[0,1])
    N = np.split(data, 2, axis=1)[0].ravel()
    times = np.split(data, 2, axis=1)[1].ravel()
    ax.plot(N, times, ls='dashed', lw=0.5, marker='^', label=name)
    return (N, times)

def plot_fit_log(x, y, ax) :
    a = np.polyfit(np.log(x), np.log(y), 1)
    fit = np.poly1d(a)
    ax.plot(x, np.exp(fit(np.log(x))), label='$N^{' + str(a[0])[0:4] +'}$')

def plot_all(fname, ax, name) :
    (n, t) = plot_data(fname, ax, name)
    plot_fit_log(n, t, ax)
