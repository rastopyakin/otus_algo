from plot_tools import *

fig, ax = plt.subplots()

fname = "build/structures/add_random_factor2.log"
(n, t) = plot_data_f(fname, ax, 'FactorArray x2')
part = n > 1e4
plot_fit_log_log(n[part], t[part], ax)

fname = "build/structures/add_random_single.log"
(n, t) = plot_data_f(fname, ax, 'SingleArray')
part = n > 0.5e4
plot_fit_log_log(n[part], t[part], ax)

# std::vector
# fname = "build/structures/add_random_wrapVector.log"
# (n, t) = plot_data_f(fname, ax, 'std::vector')
# part = n > 1e4
# plot_fit_log_log(n[part], t[part], ax)

# std::list
fname = "build/structures/add_random_wrapList.log"
plot_all(fname, ax, 'std::list')

# std::deque
fname = "build/structures/add_random_wrapDeque.log"
(n, t) = plot_data_f(fname, ax, 'std::deque')
part = n > 1e4
plot_fit_log_log(n[part], t[part], ax)


# matrix
fname = "build/structures/matrix/add_random_matrix_cs64.log"
(n, t) = plot_data_f(fname, ax, 'matrixArray 1023')
part = n > 1e4
plot_fit_log_log(n[part], t[part], ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Complexity of random adding elements to dynamic array')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
# plt.savefig('structures/add_random.pdf')
