from plot_tools import *

fig, ax = plt.subplots()

# fname = "build/string_search/small_alph_v0.log"
# (n, t) = plot_data_f(fname, ax, 'v0::full_search', 'v')
part = n > 1e3
# plot_fit_log_log(n, t, ax)

fname = "build/string_search/small_alph.log"
(n, t) = plot_data_f(fname, ax, 'full_search', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bmh_small_alph.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore-Horspoole', 'o')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

# fname = "build/string_search/22_boyer_moore/bmh_small_alph_v0.log"
# (n, t) = plot_data_f(fname, ax, 'Boyer-Moore-Horspoole', 'o')
# part = n > 1e3
# plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/bm_small_alph.log"
(n, t) = plot_data_f(fname, ax, 'Boyer-Moore', 's')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

# fname = "build/string_search/22_boyer_moore/bm_small_alph_v0.log"
# (n, t) = plot_data_f(fname, ax, 'Boyer-Moore', 's')
# part = n > 1e3
# plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('text length (|T|)')
ax.set_ylabel('execution time, ns')
ax.set_title('Execution time of search algorithms on random\n'
             'text $T$ of [a,b] alphabet pattern $P = ababab$).')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
