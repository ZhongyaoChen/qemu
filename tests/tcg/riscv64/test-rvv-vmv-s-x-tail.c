/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int test_vmv_s_x(void)
{
    uint64_t out[2];
    uint64_t in = 0x0123456789abcdefULL;

    asm volatile(
        "vsetivli zero, 2, e64, m1, ta, ma\n\t"
        "vmv.v.i v1, 0\n\t"
        "vmv.s.x v1, %[in]\n\t"
        "vse64.v v1, (%[out])\n\t"
        :
        : [in] "r"(in), [out] "r"(out)
        : "memory", "v1");

    if (out[0] != in || out[1] != UINT64_MAX) {
        fprintf(stderr,
                "vmv.s.x failed: got [%#018llx, %#018llx], expected [%#018llx, %#018llx]\n",
                (unsigned long long)out[0],
                (unsigned long long)out[1],
                (unsigned long long)in,
                (unsigned long long)UINT64_MAX);
        return 1;
    }

    return 0;
}

static int test_vfmv_s_f(void)
{
    uint64_t bits = 0x400921fb54442d18ULL;
    double in;
    uint64_t out[2];

    memcpy(&in, &bits, sizeof(in));

    asm volatile(
        "vsetivli zero, 2, e64, m1, ta, ma\n\t"
        "vmv.v.i v2, 0\n\t"
        "vfmv.s.f v2, %[in]\n\t"
        "vse64.v v2, (%[out])\n\t"
        :
        : [in] "f"(in), [out] "r"(out)
        : "memory", "v2");

    if (out[0] != bits || out[1] != UINT64_MAX) {
        fprintf(stderr,
                "vfmv.s.f failed: got [%#018llx, %#018llx], expected [%#018llx, %#018llx]\n",
                (unsigned long long)out[0],
                (unsigned long long)out[1],
                (unsigned long long)bits,
                (unsigned long long)UINT64_MAX);
        return 1;
    }

    return 0;
}

int main(void)
{
    return test_vmv_s_x() || test_vfmv_s_f();
}
