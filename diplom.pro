TEMPLATE = subdirs
SUBDIRS = libs/libcryptograph \
    examples \
    application

examples.depends = libs/libcryptograph

!exists(lib) {
    system(mkdir lib)
}

!exists(lib/libarageli.so) {
    message("configure and make libarageli...")
    system(cd libs/libarageli/src/arageli \
    && ./mk_lnx_lib.sh \
    && cd ../.. \
    && cp lib/*.so ./../../lib/)
}
exists(lib/libarageli.so) {
    message("libarageli found")
}

!exists(lib/libintel_aes.so) {
    message("configure and make lib_intel_aes...")
    system(cd libs/intel_aes_lib \
    && ./mk_lnx_lib.sh \
    && cp lib/* ./../../lib/)
}
exists(lib/libarageli.so) {
    message("lib_intel_aes found")
}

