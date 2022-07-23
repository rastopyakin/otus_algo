from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/degenerate_comp.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bmh_degenerate_comp.log"
(n, t) = plot_data_f(fname, ax, 'BMH', 'o')
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/degenerate_comp_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'KMP', '*')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bm_degenerate_comp.log"
(n, t) = plot_data_f(fname, ax, 'BM', 's')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Number of comparisons made by different algorithms'
             '\n(degenerate "aa.." text $T$ and pattern $P$). $|T| = 100*|P|$')

plt.tight_layout(pad=0.1)
ax.legend()
plt.show(block=False)

plt.savefig("string_search/report/degenerate_comp.png")
