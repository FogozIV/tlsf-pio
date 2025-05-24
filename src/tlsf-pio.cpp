//
// Created by fogoz on 21/05/2025.
//
#include <cstring>
#include <tlsf.h>
extern "C" {

    void* __real_malloc(size_t size);
    void  __real_free(void* ptr);
    void* __real_realloc(void* ptr, size_t size);
    void* __real_calloc(size_t nmemb, size_t size);

    tlsf_t tlsf_handle;
    char tlsf_pool[128 * 1024];

    void tlsf_init_once() {
        static bool initialized = false;
        if (!initialized) {
            memset(tlsf_pool, 0, sizeof(tlsf_pool));
            tlsf_handle = tlsf_create_with_pool(tlsf_pool, sizeof(tlsf_pool));
            initialized = true;
        }
    }

    void* __wrap_malloc(size_t size) {
        tlsf_init_once();
        return tlsf_malloc(tlsf_handle, size);
    }

    void __wrap_free(void* ptr) {
        tlsf_init_once();
        tlsf_free(tlsf_handle, ptr);
    }

    void* __wrap_realloc(void* ptr, size_t size) {
        tlsf_init_once();
        return tlsf_realloc(tlsf_handle, ptr, size);
    }

    void* __wrap_calloc(size_t nmemb, size_t size) {
        tlsf_init_once();
        return tlsf_memalign(tlsf_handle, 8, nmemb * size);
    }
}