from plot_tools import *

fig, ax = plt.subplots()

fname = "build/b_tree/insert_ordered_avl.log"
(n, t) = plot_data_f(fname, ax, 'ordered keys', 'v')
plot_fit_log_log(n, t, ax)

fname = "build/b_tree/insert_random_avl.log"
(n, t) = plot_data_f(fname, ax, 'random keys, big rotations as consecutive small ones', '^')
plot_fit_log_log(n, t, ax)

fname = "build/b_tree/insert_random_avl_opt.log"
(n, t) = plot_data_f(fname, ax, 'random keys, big rotations via direct assignments', 's')
plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, ns')
ax.set_title('AVL tree insertion complexity : different big rotations')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
