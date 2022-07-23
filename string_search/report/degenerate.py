from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/degenerate.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bmh_degenerate.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore-Horspool', 'o')
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bm_degenerate.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore', 's')
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/degenerate_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'Knutt-Morris-Pratt', '*')
plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Execution time of search algorithms on degenerate \"aa..aa\" strings \n (text '
             '$T$ and pattern $P$). $|T| = 100*|P|$')

plt.tight_layout(pad=0.1)
ax.legend()
plt.show(block=False)

plt.savefig("string_search/report/degenerate.png")
