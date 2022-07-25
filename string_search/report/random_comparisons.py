from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/random_comp.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/random_comp_imp_1.log"
(n, t) = plot_data_f(fname, ax, 'KMP', '*')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bm_random_comp.log"
(n, t) = plot_data_f(fname, ax, 'BM', 's')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

fname = "build/string_search/22_boyer_moore/bmh_random_comp.log"
(n, t) = plot_data_f(fname, ax, 'BMH', 'o')
part = n > 1e2
plot_fit_log_log(n[part], t[part], ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('pattern length (|P|)')
ax.set_ylabel('number of comparisons')
ax.set_title('Average number of comparisons made by different algorithms'
             '\n(random text $T$ and pattern $P$). $|T| = 100*|P|$')

plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)

plt.savefig("string_search/report/random_comp.png")
