from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/24_kmp/random_imp_2.log"
(n, t) = plot_data_f(fname, ax, 'Knutt-Morris-Pratt 2', '*')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/random_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'Knutt-Morris-Pratt 1', '*')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/small_alph_imp_2.log"
(n, t) = plot_data_f(fname, ax, 'Knutt-Morris-Pratt 2 [a,b]-alphabet', 's')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/small_alph_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'Knutt-Morris-Pratt 1 [a, b]-alphabet', 's')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Execution time of different KMP implementations \n'
             '(random text $T$ and pattern $P$). $|T| = 100*|P|$')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)

plt.savefig("string_search/report/kmp_versions.png")
