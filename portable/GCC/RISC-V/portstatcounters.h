/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021 Hesham Almatary
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _PORT_HPM_COUNTERS
#define _PORT_HPM_COUNTERS

#include <stdlib.h>
#include <stdint.h>

#define STR( x )     XSTR( x )
#define XSTR( x )    # x

#define EVENT_REDIRECT                  0x1
#define EVENT_BRANCH                    0x3
#define EVENT_JAL                       0x4
#define EVENT_JALR                      0x5
#define EVENT_TRAP                      0x2

#define EVENT_LOAD_WAIT                 0x10
#define EVENT_CAP_LOAD                  0x1a
#define EVENT_CAP_STORE                 0x1b

#define EVENT_ITLB_MISS                 0x2a
#define EVENT_ICACHE_LOAD               0x20
#define EVENT_ICACHE_LOAD_MISS          0x21
#define EVENT_ICACHE_LOAD_MISS_WAIT     0x22

#define EVENT_DTLB_ACCESS               0x39
#define EVENT_DTLB_MISS                 0x3a
#define EVENT_DTLB_MISS_WAIT            0x3b
#define EVENT_DCACHE_LOAD               0x30
#define EVENT_DCACHE_LOAD_MISS          0x31
#define EVENT_DCACHE_LOAD_MISS_WAIT     0x32
#define EVENT_DCACHE_STORE              0x33
#define EVENT_DCACHE_STORE_MISS         0x34

#define EVENT_LLCACHE_FILL              0x61
#define EVENT_LLCACHE_FILL_WAIT         0x62
#define EVENT_LLCACHE_EVICT             0x64

#define EVENT_TAGCACHE_LOAD             0x42
#define EVENT_TAGCACHE_LOAD_MISS        0x43
#define EVENT_TAGCACHE_STORE            0x40
#define EVENT_TAGCACHE_STORE_MISS       0x41
#define EVENT_TAGCACHE_EVICT            0x44

/* Static mapping of events */
#define EVENT_3                         EVENT_REDIRECT
#define EVENT_4                         EVENT_BRANCH
#define EVENT_5                         EVENT_JAL
#define EVENT_6                         EVENT_JALR
#define EVENT_7                         EVENT_TRAP
#define EVENT_8                         EVENT_LOAD_WAIT
#define EVENT_9                         EVENT_CAP_LOAD
#define EVENT_10                        EVENT_CAP_STORE
#define EVENT_11                        EVENT_ITLB_MISS
#define EVENT_12                        EVENT_ICACHE_LOAD
#define EVENT_13                        EVENT_ICACHE_LOAD_MISS
#define EVENT_14                        EVENT_ICACHE_LOAD_MISS_WAIT
#define EVENT_15                        EVENT_DTLB_ACCESS
#define EVENT_16                        EVENT_DTLB_MISS
#define EVENT_17                        EVENT_DTLB_MISS_WAIT
#define EVENT_18                        EVENT_DCACHE_LOAD
#define EVENT_19                        EVENT_DCACHE_LOAD_MISS
#define EVENT_20                        EVENT_DCACHE_LOAD_MISS_WAIT
#define EVENT_21                        EVENT_DCACHE_STORE
#define EVENT_22                        EVENT_DCACHE_STORE_MISS
#define EVENT_23                        EVENT_LLCACHE_FILL
#define EVENT_24                        EVENT_LLCACHE_FILL_WAIT
#define EVENT_25                        EVENT_TAGCACHE_LOAD
#define EVENT_26                        EVENT_TAGCACHE_LOAD_MISS
#define EVENT_27                        EVENT_LLCACHE_EVICT
#define EVENT_28                        EVENT_TAGCACHE_STORE_MISS
#define EVENT_29                        EVENT_TAGCACHE_EVICT

#if __riscv_xlen == 64
    #define RISCV_READ_CSR( reg )                                \
        ( { uint64_t __tmp;                                      \
            asm volatile ( "csrr %0, " # reg : "=r" ( __tmp ) ); \
            __tmp; } )
#else
    #define RISCV_READ_CSR( reg )                                \
        ( { unsigned long __tmpl, __tmph;                        \
            asm volatile (                                       \
                "%=:\n\t"                                        \
                "csrr %1, " # reg "h\n\t"                        \
                "csrr %0, " # reg "\n\t"                         \
                "csrr t1, " # reg "h\n\t"                        \
                "bne  %1, t1, %=b"                               \
                : "=r" ( __tmpl ), "=r" ( __tmph )               \
                : /* No inputs. */                               \
                : "t1" );                                        \
            ( uint64_t ) (( ( ( uint64_t ) __tmph ) << 32 ) | ( uint64_t ) __tmpl ); } )
#endif /* if __riscv_xlen == 64 */

typedef uint64_t PortCounter_t;

typedef enum
{
    COUNTER_CYCLE = 0,
    COUNTER_INSTRET = 1,
    COUNTER_REDIRECT = 3,
    COUNTER_BRANCH = 4,
    COUNTER_JAL = 5,
    COUNTER_JALR = 6,
    COUNTER_TRAP = 7,

    COUNTER_LOAD_WAIT = 8,
    COUNTER_CAP_LOAD = 9,
    COUNTER_CAP_STORE = 10,

    COUNTER_ITLB_MISS = 11,
    COUNTER_ICACHE_LOAD = 12,
    COUNTER_ICACHE_LOAD_MISS = 13,
    COUNTER_ICACHE_LOAD_MISS_WAIT = 14,

    COUNTER_DTLB_ACCESS = 15,
    COUNTER_DTLB_MISS = 16,
    COUNTER_DTLB_MISS_WAIT = 17,
    COUNTER_DCACHE_LOAD = 18,
    COUNTER_DCACHE_LOAD_MISS = 19,
    COUNTER_DCACHE_LOAD_MISS_WAIT = 20,
    COUNTER_DCACHE_STORE = 21,
    COUNTER_DCACHE_STORE_MISS = 22,

    COUNTER_LLCACHE_FILL = 23,
    COUNTER_LLCACHE_FILL_WAIT = 24,

    COUNTER_TAGCACHE_LOAD = 25,
    COUNTER_TAGCACHE_LOAD_MISS = 26,
    COUNTER_LLCACHE_EVICT = 27,
    COUNTER_TAGCACHE_STORE_MISS = 28,
    COUNTER_TAGCACHE_EVICT = 29,

    COUNTERS_NUM
} PortCounterID_t;


static inline void portCountersInit( void )
{
    asm volatile (
        "li t0, 0x0fffffff8\n\t"
        "csrw mhpmevent10, t0\n\t"
        "csrw mhpmevent11, t0\n\t"
        "li t0, " STR( EVENT_12 ) "\n\t"
        "csrw mhpmevent12, t0\n\t"
        "li t0, " STR( EVENT_13 ) "\n\t"
        "csrw mhpmevent13, t0\n\t"
        "li t0, " STR( EVENT_14 ) "\n\t"
        "csrw mhpmevent14, t0\n\t"
        "li t0, " STR( EVENT_15 ) "\n\t"
        "csrw mhpmevent15, t0\n\t"
        "li t0, " STR( EVENT_16 ) "\n\t"
        "csrw mhpmevent16, t0\n\t"
        "li t0, " STR( EVENT_17 ) "\n\t"
        "csrw mhpmevent17, t0\n\t"
        "li t0, " STR( EVENT_18 ) "\n\t"
        "csrw mhpmevent18, t0\n\t"
        "li t0, " STR( EVENT_19 ) "\n\t"
        "csrw mhpmevent19, t0\n\t"
        "li t0, " STR( EVENT_20 ) "\n\t"
        "csrw mhpmevent20, t0\n\t"
        "li t0, " STR( EVENT_21 ) "\n\t"
        "csrw mhpmevent21, t0\n\t"
        "li t0, " STR( EVENT_22 ) "\n\t"
        "csrw mhpmevent22, t0\n\t"
        "li t0, " STR( EVENT_23 ) "\n\t"
        "csrw mhpmevent23, t0\n\t"
        "li t0, " STR( EVENT_24 ) "\n\t"
        "csrw mhpmevent24, t0\n\t"
        "li t0, " STR( EVENT_25 ) "\n\t"
        "csrw mhpmevent25, t0\n\t"
        "li t0, " STR( EVENT_26 ) "\n\t"
        "csrw mhpmevent26, t0\n\t"
        "li t0, " STR( EVENT_27 ) "\n\t"
        "csrw mhpmevent27, t0\n\t"
        "li t0, " STR( EVENT_28 ) "\n\t"
        "csrw mhpmevent28, t0\n\t"
        "li t0, " STR( EVENT_29 ) "\n\t"
        "csrw mhpmevent29, t0\n\t"
        /* initialize all counters to 0 */
        "li t0, 0\n\t"
        "csrw mhpmcounter3, t0\n\t"
        "csrw mhpmcounter4, t0\n\t"
        "csrw mhpmcounter5, t0\n\t"
        "csrw mhpmcounter6, t0\n\t"
        "csrw mhpmcounter7, t0\n\t"
        "csrw mhpmcounter8, t0\n\t"
        "csrw mhpmcounter9, t0\n\t"
        "csrw mhpmcounter10, t0\n\t"
        "csrw mhpmcounter11, t0\n\t"
        "csrw mhpmcounter12, t0\n\t"
        "csrw mhpmcounter13, t0\n\t"
        "csrw mhpmcounter14, t0\n\t"
        "csrw mhpmcounter15, t0\n\t"
        "csrw mhpmcounter16, t0\n\t"
        "csrw mhpmcounter17, t0\n\t"
        "csrw mhpmcounter18, t0\n\t"
        "csrw mhpmcounter19, t0\n\t"
        "csrw mhpmcounter20, t0\n\t"
        "csrw mhpmcounter21, t0\n\t"
        "csrw mhpmcounter22, t0\n\t"
        "csrw mhpmcounter23, t0\n\t"
        "csrw mhpmcounter24, t0\n\t"
        "csrw mhpmcounter25, t0\n\t"
        "csrw mhpmcounter26, t0\n\t"
        "csrw mhpmcounter27, t0\n\t"
        "csrw mhpmcounter28, t0\n\t"
        "csrw mhpmcounter29, t0\n\t"
        /* bitmask in t0 */
        "li t0, 0xfffffff8\n\t"
        /* un-inhibit all counters */
        "csrc mcountinhibit, t0\n\t"
        :    /* no outputs */
        :    /* no inputs */
        : "t0",
        #if __CHERI_PURE_CAPABILITY__
            "ct1"
        #else
            "t1"
        #endif
        );
}

static inline PortCounter_t portCounterGet( PortCounterID_t counterID )
{
    switch( counterID )
    {
        case COUNTER_CYCLE:
            return RISCV_READ_CSR( mcycle );

            break;

        case COUNTER_INSTRET:
            return RISCV_READ_CSR( minstret );

            break;

        case COUNTER_REDIRECT:
            return RISCV_READ_CSR( mhpmcounter3 );

            break;

        case COUNTER_BRANCH:
            return RISCV_READ_CSR( mhpmcounter4 );

            break;

        case COUNTER_JAL:
            return RISCV_READ_CSR( mhpmcounter5 );

            break;

        case COUNTER_JALR:
            return RISCV_READ_CSR( mhpmcounter6 );

            break;

        case COUNTER_TRAP:
            return RISCV_READ_CSR( mhpmcounter7 );

            break;

        case COUNTER_LOAD_WAIT:
            return RISCV_READ_CSR( mhpmcounter8 );

            break;

        case COUNTER_CAP_LOAD:
            return RISCV_READ_CSR( mhpmcounter9 );

            break;

        case COUNTER_CAP_STORE:
            return RISCV_READ_CSR( mhpmcounter10 );

            break;

        case COUNTER_ITLB_MISS:
            return RISCV_READ_CSR( mhpmcounter11 );

            break;

        case COUNTER_ICACHE_LOAD:
            return RISCV_READ_CSR( mhpmcounter12 );

            break;

        case COUNTER_ICACHE_LOAD_MISS:
            return RISCV_READ_CSR( mhpmcounter13 );

            break;

        case COUNTER_ICACHE_LOAD_MISS_WAIT:
            return RISCV_READ_CSR( mhpmcounter14 );

            break;

        case COUNTER_DTLB_ACCESS:
            return RISCV_READ_CSR( mhpmcounter15 );

            break;

        case COUNTER_DTLB_MISS:
            return RISCV_READ_CSR( mhpmcounter16 );

            break;

        case COUNTER_DTLB_MISS_WAIT:
            return RISCV_READ_CSR( mhpmcounter17 );

            break;

        case COUNTER_DCACHE_LOAD:
            return RISCV_READ_CSR( mhpmcounter18 );

            break;

        case COUNTER_DCACHE_LOAD_MISS:
            return RISCV_READ_CSR( mhpmcounter19 );

            break;

        case COUNTER_DCACHE_LOAD_MISS_WAIT:
            return RISCV_READ_CSR( mhpmcounter20 );

            break;

        case COUNTER_DCACHE_STORE:
            return RISCV_READ_CSR( mhpmcounter21 );

            break;

        case COUNTER_DCACHE_STORE_MISS:
            return RISCV_READ_CSR( mhpmcounter22 );

            break;

        case COUNTER_LLCACHE_FILL:
            return RISCV_READ_CSR( mhpmcounter23 );

            break;

        case COUNTER_LLCACHE_FILL_WAIT:
            return RISCV_READ_CSR( mhpmcounter24 );

            break;

        case COUNTER_TAGCACHE_LOAD:
            return RISCV_READ_CSR( mhpmcounter25 );

            break;

        case COUNTER_TAGCACHE_LOAD_MISS:
            return RISCV_READ_CSR( mhpmcounter26 );

            break;

        case COUNTER_LLCACHE_EVICT:
            return RISCV_READ_CSR( mhpmcounter27 );

            break;

        case COUNTER_TAGCACHE_STORE_MISS:
            return RISCV_READ_CSR( mhpmcounter28 );

            break;

        case COUNTER_TAGCACHE_EVICT:
            return RISCV_READ_CSR( mhpmcounter29 );

            break;

        default:
            return 0;
    }
}

#endif /* _PORT_HPM_COUNTERS */
