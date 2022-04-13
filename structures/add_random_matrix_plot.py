from plot_tools import *

fig, ax = plt.subplots()

fname = "build/structures/matrix/add_random_matrix_cs4095.log"
(n, t) = plot_data_f(fname, ax, 'matrixArray 4095')
part = n > 5e4
plot_fit_log_log(n[part], t[part], ax)
part = n < 5e3
plot_fit_log_log(n[part], t[part], ax)

fname = "build/structures/matrix/add_random_matrix_cs2047.log"
(n, t) = plot_data_f(fname, ax, 'matrixArray 2047')
part = n > 5e4
plot_fit_log_log(n[part], t[part], ax)

# fname = "build/structures/matrix/add_random_matrix_cs1023.log"
# (n, t) = plot_data_f(fname, ax, 'matrixArray 1023')
# part = n > 1e4
# plot_fit_log_log(n[part], t[part], ax)

fname = "build/structures/matrix/add_random_matrix_cs63.log"
(n, t) = plot_data_f(fname, ax, 'matrixArray 63')
part = n > 8e4
plot_fit_log_log(n[part], t[part], ax)

plt.tight_layout(pad=0.2)
ax.legend()

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, $\mu$s')
ax.set_title('Complexity of random adding elements to matrix array')
plt.show(block=False)
