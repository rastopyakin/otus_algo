from plot_tools import *

fig, ax = plt.subplots()

# ordered numbers insertion
fname = "build/6_simple_sort/insertion_v1.log"
(n, t) = plot_data_f(fname, ax, 'v1')
plot_fit_log_log(n, t, ax)

fname = "build/6_simple_sort/insertion_v1_5.log"
(n, t) = plot_data_f(fname, ax, 'v1_5')
plot_fit_log_log(n, t, ax)

fname = "build/6_simple_sort/insertion_v2.log"
(n, t) = plot_data_f(fname, ax, 'v2 (shifting instead swapping)')
plot_fit_log_log(n, t, ax)

fname = "build/6_simple_sort/insertion_v3.log"
(n, t) = plot_data_f(fname, ax, 'v3 (shifting instead swapping,\nbinsearch of where to insert)')
part = n > 6e3
plot_fit_log_log(n[part], t[part], ax)


ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('array size (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Complexity of insertion sort algorithm (random data)')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
