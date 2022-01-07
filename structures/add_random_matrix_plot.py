from structures.plot_tools import *

fig, ax = plt.subplots()

fname = 'build/structures/add_random_matrix.log'
plot_data(fname, ax, 'cs : 1023')

fname = 'build/structures/add_random_matrix_cs127.log'
plot_data(fname, ax, 'cs : 127')

fname = 'build/structures/add_random_matrix_cs2047.log'
plot_data(fname, ax, 'cs : 2047')

plt.tight_layout(pad=0.2)
ax.legend()

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, $\mu$s')
ax.set_title('Complexity of random adding elements to matrix array')
plt.show(block=False)
