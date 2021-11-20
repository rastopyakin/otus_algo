from structures.plot_tools import *

fig, ax = plt.subplots()

# singleArray
fname = "build/structures/add_random_factor1.5_A.log"
plot_all(fname, ax, 'random, x1.5 (A)')

fname = "build/structures/add_random_vector_A.log"
plot_all(fname, ax, '+100, random')

fname = "build/structures/add_random_single_A.log"
plot_all(fname, ax, '+1, random (A)')

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('additions number (N)')
ax.set_ylabel('execution time, $\mu$s')
ax.set_title('Complexity of random adding elements to dynamic array: \ndifferent reallocation strategies')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
plt.savefig('structures/add_random.pdf')
