#include <pantheios/implicit_link/core.h>
#include <pantheios/implicit_link/fe.simple.h>
#include <platformstl/platformstl.h>
#include <pantheios/implicit_link/be.file.h>

#if (   defined(UNIX) || \
        defined(unix))&& \
    (   defined(_WIN32) || \
        defined(_WIN64))
# include <unixem/implicit_link.h>
#endif /* _WIN32 || _WIN64 */