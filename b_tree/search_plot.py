from plot_tools import *

fig, ax = plt.subplots()

# searching in ordered BST
fname = "build/b_tree/search_ordered_bst.log"
(n, t) = plot_data_f(fname, ax, 'ordered BST')
# part = n < 65000
plot_fit_log_log(n, t, ax)

fname = "build/b_tree/search_random_avl.log"
(n, t) = plot_data_f(fname, ax, 'random AVL', 's')
part = n > 65000
plot_fit_log_log(n[part], t[part], ax)

# searching in random BST
fname = "build/b_tree/search_random_bst.log"
(n, t) = plot_data_f(fname, ax, 'random BST', 'X')
part = n > 65000
plot_fit_log_log(n[part], t[part], ax)

fname = "build/b_tree/search_ordered_avl.log"
(n, t) = plot_data_f(fname, ax, 'ordered AVL', '^')
part = n > 1e4
plot_fit_log_log(n[part], t[part], ax)


ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('Searching N/10 elements in a tree (BST vs AVL) of size N')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
