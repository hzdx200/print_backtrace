#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <dlfcn.h>
#include <cxxabi.h>

void print_backtrace (void)
{
    Dl_info dl_info;
    unw_cursor_t cursor;
    unw_context_t ctx;
    unsigned frame_n = 0;

    unw_getcontext (&ctx);
    unw_init_local (&cursor, &ctx);

    while (unw_step (&cursor) > 0) {
        unw_word_t offset;
        unw_proc_info_t p_info;
        const char *file_name;
        char *demangled_name;
        char func_name[256] = "";
        void *addr;
        int rc;

        if (unw_get_proc_info (&cursor, &p_info))
            break;

        addr = (void *)(p_info.start_ip + offset);

        if (dladdr (addr, &dl_info) && dl_info.dli_fname)
            file_name = dl_info.dli_fname;
        else
            file_name = "?";

        rc = unw_get_proc_name (&cursor, func_name, 256, &offset);
        if (rc == -UNW_ENOINFO)
            strcpy(func_name, "?");

        demangled_name = abi::__cxa_demangle (func_name, NULL, NULL, &rc);

        printf ("#%u  %p in %s (%s+0x%lx)\n", frame_n++, addr, file_name,
                rc ? func_name : demangled_name, (unsigned long) offset);
        free (demangled_name);
    }

    fflush (stdout);
}


int main(void)
{
    print_backtrace();
    return 0;
}
