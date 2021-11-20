from structures.plot_tools import *

fig, ax = plt.subplots()

# singleArray
fname = "build/structures/add_back_single_copy.log"
plot_all(fname, ax, 'single')

# VectorArray
fname = "build/structures/add_back_vector.log"
plot_all(fname, ax, 'vector +100')

fname = "build/structures/add_back_factor1.5.log"
plot_all(fname, ax, 'factor x1.5')

fname = "build/structures/add_back_factor10.log"
plot_all(fname, ax, 'factor x10')

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, $\mu$s')
ax.set_title('Complexity of back adding elements to dynamic array: \ndifferent reallocation strategies')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
plt.savefig('structures/add_back.pdf')
