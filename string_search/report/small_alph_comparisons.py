from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/small_alph_comp.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/small_alph_comp_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'KMP', '*')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bmh_small_alph_comp.log"
(n, t) = plot_data_f(fname, ax, 'BMH', 'o')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bm_small_alph_comp.log"
(n, t) = plot_data_f(fname, ax, 'BM', 's')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Average number of comparisons made by different algorithms'
             '\nrandom text $T$ and pattern $P$, [a,b]-alphabet). $|T| = 100*|P|$')

plt.tight_layout(pad=0.1)
ax.legend()
plt.show(block=False)

plt.savefig("string_search/report/small_alphabet_comp.png")
