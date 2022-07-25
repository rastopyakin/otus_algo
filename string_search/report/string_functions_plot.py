from plot_tools import *

fig, ax = plt.subplots()

fname = "build/string_search/22_boyer_moore/z_function_naive_v0.log"
(n, t) = plot_data_f(fname, ax, 'naive Z-function', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/pi_function_naive.log"
(n, t) = plot_data_f(fname, ax, 'naive $\pi$-function', 's')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/z_function_v0.log"
(n, t) = plot_data_f(fname, ax, 'Z-function (v0)', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/z_function.log"
(n, t) = plot_data_f(fname, ax, 'Z-function', 'v')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/24_kmp/pi_function.log"
(n, t) = plot_data_f(fname, ax, '$\pi$-function', 's')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/suffix_function_naive.log"
(n, t) = plot_data_f(fname, ax, 'BM suffix-function (naive)', 'o')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/suffix_function_v0.log"
(n, t) = plot_data_f(fname, ax, 'BM suffix-function (v0)', 'o')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

fname = "build/string_search/22_boyer_moore/suffix_function.log"
(n, t) = plot_data_f(fname, ax, 'BM suffix-function', 'o')
# part = n > 1e3
plot_fit_log_log(n, t, ax)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel('string length (n)')
ax.set_ylabel('execution time, ns')
ax.set_title('Complexity of $\pi(S, n)$, $Z(S, n)$ and Boyer-Moore suffix- \n'
             'string functions: $S=a..ab$ (for $\pi(S, n)$ and $Z(S, n)$) and \n'
             '$S=ba..a$ (for BM suffix)')
plt.tight_layout(pad=0.2)
ax.legend()
plt.show(block=False)
