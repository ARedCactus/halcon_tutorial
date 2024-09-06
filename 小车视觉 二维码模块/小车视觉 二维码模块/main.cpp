#include "QR.h"

int main(int argc, char* argv[])
{
    int ret = 0;

    try
    {
#if defined(_WIN32)
        SetSystem("use_window_thread", "true");
#endif

        // file was stored with local-8-bit encoding
        //   -> set the interface encoding accordingly
        SetHcppInterfaceStringEncodingIsUtf8(false);

        // Default settings used in HDevelop (can be omitted)
        SetSystem("width", 512);
        SetSystem("height", 512);

#ifndef __APPLE__
        action();
#else
        ret = apple_main(argc, argv);
#endif
    }
    catch (HException& exception)
    {
        fprintf(stderr, "  Error #%u in %s: %s\n", exception.ErrorCode(),
            exception.ProcName().TextA(),
            exception.ErrorMessage().TextA());
        ret = 1;
    }
    return ret;
}