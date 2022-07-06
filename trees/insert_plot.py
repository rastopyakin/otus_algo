from plot_tools import *

fig, ax = plt.subplots()

# ordered numbers insertion
fname = "build/b_tree/insert_ordered_bst.log"
(n, t) = plot_data_f(fname, ax, 'ordered BST')
plot_fit_log_log(n, t, ax)

fname = "build/b_tree/insert_ordered_avl.log"
(n, t) = plot_data_f(fname, ax, 'ordered AVL', 'v')
plot_fit_log_log(n, t, ax)

fname = "build/b_tree/insert_random_bst.log"
(n, t) = plot_data_f(fname, ax, 'random BST', '*')
part = n < 200000
plot_fit_log_log(n, t, ax)

fname = "build/b_tree/insert_random_avl.log"
(n, t) = plot_data_f(fname, ax, 'random AVL', '^')
plot_fit_log_log(n, t, ax)

# n_log_n = 9*n*np.log(n)
# ax.plot(n, n_log_n)
# plot_fit_log_log(n, n_log_n, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Insertion complexity (BST vs AVL): random and ordered keys')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
