from plot_tools import *

fig, ax = plt.subplots()

# ordered numbers insertion
fname = "build/6_simple_sort/bubble_v1.log"
(n, t) = plot_data_f(fname, ax, 'bubble v1')
plot_fit_log_log(n, t, ax)

fname = "build/6_simple_sort/bubble_v2.log"
(n, t) = plot_data_f(fname, ax, 'bubble v2 (adaptive)')
plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('array size (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Complexity of bubble sort algorithm (random data)')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
