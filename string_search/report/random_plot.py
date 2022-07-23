from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/random.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)
fname = "build/string_search/24_kmp/random_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'Knutt-Morris-Pratt', '*')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bm_random.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore', 's')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

fname = "build/string_search/22_boyer_moore/bmh_random.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore-Horspoole', 's')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Execution time of search algorithms on random a...z strings \n'
             '(text $T$ and pattern $P$). $|T| = 100*|P|$')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)

plt.savefig("string_search/report/random.png")
