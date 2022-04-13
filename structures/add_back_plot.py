from structures.plot_tools import *

def model_array_iteration(n_vals) :
    time = 0
    alloc_data = "build/structures/blocks/allocating_mu.log"
    (n, t) = get_data(alloc_data)
    # time to allocate n + 1 elements
    alloc_times = np.interp(n_vals + 1, n, t)
    move_data = "build/structures/blocks/moving.log"
    (n, t) = get_data(move_data)
    # time to allocate n elements to new location
    move_times = np.interp(n_vals, n, t)
    assignment_time = 1         # ns, just average value
    return alloc_times + move_times + assignment_time

fig, ax = plt.subplots()

# singleArray
fname = "build/structures/add_back_single.log"
(n, t) = plot_data_f(fname, ax, 'SingleArray')
part = n < 3e3
# plot_fit_log_log(n[part], t[part], ax)
n_p = n[part]
ax.plot(n_p, 0.08*n_p**2 + 70*n_p, '-', label='$0.08*N^2 + 70*N$')
part = n > 1.3e4
plot_fit_log_log(n[part], t[part], ax)

# VectorArray
fname = "build/structures/vectorAlloc/add_back_vector_100.log"
(n, t) = plot_data_f(fname, ax, '+100')
part = n > 2e4
plot_fit_log_log(n[part], t[part], ax)

# std::list
fname = "build/structures/add_back_wrapList.log"
plot_all(fname, ax, 'std::list')

# Factor Array
fname = "build/structures/add_back_factor2.log"
(n, t) = plot_data_f(fname, ax, ' x2')
part = n > 3e4
plot_fit_log_log(n[part], t[part], ax)

# std::vector
# fname = "build/structures/add_back_wrapVector.log"
# plot_data_f(fname, ax, 'std::vector')

# std::deque
fname = "build/structures/add_back_wrapDeque.log"
(n, t) = plot_data_f(fname, ax, 'std::deque')
part = n > 1e6
plot_fit_log_log(n[part], t[part], ax)
part = n < 3e4
plot_fit_log_log(n[part], t[part], ax)

# MatrixArray
# fname = "build/structures/matrix/add_back_matrix_cs2047.log"
# plot_data_f(fname, ax, 'matrixArray')

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Complexity of back adding elements to dynamic array: \ndifferent reallocation strategies')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
# plt.savefig('structures/add_back_array.pdf')
