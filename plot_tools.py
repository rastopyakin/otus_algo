import numpy as np
import matplotlib.pyplot as plt

def get_data(fname) :
    data  = np.loadtxt(fname, usecols=[0,1])
    N = np.split(data, 2, axis=1)[0].ravel()
    times = np.split(data, 2, axis=1)[1].ravel()
    return (N, times)

def plot_data_f(fname, ax, name, mkr='.') :
    data  = np.loadtxt(fname, usecols=[0,1])
    N = np.split(data, 2, axis=1)[0].ravel()
    times = np.split(data, 2, axis=1)[1].ravel()
    ax.plot(N, times, ls='dashed', lw=0.5, marker=mkr, label=name)
    return (N, times)

def plot_fit_log_log(x, y, ax) :
    a = np.polyfit(np.log(x), np.log(y), 1)
    fit = np.poly1d(a)
    degree = '%.3g' % a[0]
    multiplier = '%.2g' % np.exp(a[1])
    ax.plot(x, np.exp(fit(np.log(x))), label='$' + multiplier + '*N^{' + degree +'}$')

def plot_fit_logx(x, y, ax) :
    a = np.polyfit(np.log(x), y, 1)
    fit = np.poly1d(a)
    ax.plot(x, fit(np.log(x)), label = '$\log N*' + str(a[0])[0:4] + '$')

def plot_fit(x, y, ax) :
    a = np.polyfit(x, y, 1)
    fit = np.poly1d(a)
    multiplier = '%.2g' % a[0]
    constant = '%.2g' % a[1]
    ax.plot(x, fit(x), label='$' + multiplier +'\cdot N + ' + constant + '$', lw = 2.5)


def plot_all(fname, ax, name) :
    (n, t) = plot_data_f(fname, ax, name)
    plot_fit_log_log(n, t, ax)
