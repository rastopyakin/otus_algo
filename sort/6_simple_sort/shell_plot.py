from plot_tools import *

fig, ax = plt.subplots()

fname = "build/6_simple_sort/shell_v1.log"
(n, t) = plot_data_f(fname, ax, 'v1 ([$N/2^k]$ gaps)', '^')
part = n > 1e3
plot_fit_log_log(n[part], t[part], ax)

fname = "build/6_simple_sort/shell_v2.log"
(n, t) = plot_data_f(fname, ax, 'v2 ($2^k + 1$ gaps)', 'x')
part = n > 1e3
plot_fit_log_log(n[part], t[part], ax)

fname = "build/6_simple_sort/shell_v3.log"
(n, t) = plot_data_f(fname, ax, 'v3 ($4^k + 3*2^{k-1} + 1$ gaps)', 'v')
part = n > 1e3
plot_fit_log_log(n[part], t[part], ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('array size (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Complexity of Shell sort algorithm with \ndifferent gaps sequences (random data)')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
