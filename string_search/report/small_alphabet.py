from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/small_alph.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

fname = "build/string_search/22_boyer_moore/bmh_small_alph.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore-Horspoole', 'o')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

fname = "build/string_search/24_kmp/small_alph_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'KMP', '*')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

fname = "build/string_search/22_boyer_moore/bm_small_alph.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore', 's')
part = n > 3e2
plot_fit_log_log(n[part], t[part], ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Execution time of search algorithms on random\n'
             'text $T$ and pattern $P$ of [a,b]-alphabet ($|T| = 100*|P|$)).')
plt.tight_layout(pad=0.1)
ax.legend()
plt.show(block=False)

plt.savefig("string_search/report/small_alphabet.png")
