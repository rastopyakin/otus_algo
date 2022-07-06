from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/degenerate.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

# fname = "build/string_search/degenerate_v0.log"
# (n, t) = plot_data_f(fname, ax, 'v0::full_search', 'v')
# # part = n > 1e3
# plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bm_degenerate.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore', 's')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

# fname = "build/string_search/22_boyer_moore/bm_degenerate_v0.log"
# (n, t) = plot_data_f(fname, ax, 'Boyer-Moore (v0)', 's')
# # part = n > 1e3
# plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bmh_degenerate.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore-Horspool', 'o')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/degenerate.log"
(n, t) = plot_data_f(fname, ax, 'Knutt-Morris-Pratt', '*')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Execution time of search algorithms on degenerate \"aa..aa\" strings \n (text'
             '$T$ and pattern $P$). $|T| = 100*|P|$')

plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
