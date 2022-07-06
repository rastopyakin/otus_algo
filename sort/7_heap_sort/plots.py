from plot_tools import *

fig, ax = plt.subplots()

# ordered numbers insertion
fname = "build/sort/7_heap_sort/selection_sort.log"
(n, t) = plot_data_f(fname, ax, 'selection sort', 's')
plot_fit_log_log(n, t, ax)

fname = "build/sort/7_heap_sort/heapify.log"
(n, t) = plot_data_f(fname, ax, 'turn array into a heap')
plot_fit_log_log(n, t, ax)

fname = "build/sort/7_heap_sort/heap_sort.log"
(n, t) = plot_data_f(fname, ax, 'heap sort', 'v')
# plot_fit_log_log(n, t, ax)

x = np.linspace(n[0], n[n.size - 1])
plt.plot(x, 140*x*np.log(x), label="$N\cdot log(N)$")

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('array size (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Complexity of different algorithms related to Heap Sort (random data)')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
