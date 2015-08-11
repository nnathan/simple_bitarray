#ifndef SIMPLE_BITARRAY_H
#define SIMPLE_BITARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#if (__STDC_VERSION__ < 199901L)
#define inline
#endif


/* compile time assertions used to perform sanity checks */
#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate, __LINE__, file)
#define _impl_PASTE(a, b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_in_##file##_on_line_, line)[2*!!(predicate)-1]

CASSERT(1 == 2, simple_bitarray_h);

typedef unsigned int bitarray_word;

/*
 * this is not guaranteed to be portable since an integral type size
 * doesn't necessarily define the range
 */
#define BITARRAY_LIMB_BITS (sizeof(bitarray_word) * CHAR_BIT)

/* macro to find the number of limbs to store num-bits */
#define BITARRAY_NUM_BITS_TO_LIMBS(num_bits) \
    (((num_bits) + BITARRAY_LIMB_BITS - 1) / BITARRAY_LIMB_BITS)

/* fetch the limb containing bit position n */
#define BITARRAY_LIMB(bitarray, n_bit) \
    ((bitarray)[1 + ((n_bit) / BITARRAY_LIMB_BITS)].limb)

/* build mask to select bit in limb */
#define BITARRAY_LIMBBIT(n_bit) \
    ((1UL << ((n_bit) % BITARRAY_LIMB_BITS)))

/*
 * we use a struct to give a name;
 * when used, we reserve bitarray[0] for storing the number of bits.
 */
typedef struct
{
    bitarray_word limb;
} bitarray;

/*
 * To declare a bitarray with automatic storage:
 * {
 *     BITARRAY_DECLARE(x, 1024); // 'x' will now be set to bitarray
 * }
 */
#define BITARRAY_DECLARE(name, nbits) \
    bitarray (name)[1 + BITARRAY_NUM_BITS_TO_LIMBS((nbits))] = { { nbits } }

static inline size_t bitarray_num_limbs(bitarray *bitarray)
{
    return BITARRAY_NUM_BITS_TO_LIMBS(bitarray[0].limb);
}

static inline size_t bitarray_num_bits(bitarray *bitarray)
{
    return bitarray[0].limb;
}

static inline void bitarray_set_bit(bitarray *bitarray, bitarray_word n)
{
    BITARRAY_LIMB(bitarray, n) |= BITARRAY_LIMBBIT(n);
}

static inline void bitarray_clear_bit(bitarray *bitarray, bitarray_word n)
{
    BITARRAY_LIMB(bitarray, n) &= ~BITARRAY_LIMBBIT(n);
}

static inline void bitarray_change_bit(bitarray *bitarray, bitarray_word n)
{
    BITARRAY_LIMB(bitarray, n) ^= BITARRAY_LIMBBIT(n);
}

static inline int bitarray_test_bit(const bitarray *bitarray, bitarray_word n)
{
    return !!(BITARRAY_LIMB(bitarray, n) & BITARRAY_LIMBBIT(n));
}

static inline void bitarray_zero(bitarray *bitarray)
{
    size_t n_bytes = sizeof(bitarray_word) * bitarray_num_limbs(bitarray);
    memset(&bitarray[1], 0, n_bytes);
}

/* clear bits at position 0 to (nbits-1) */
static inline void bitarray_clear_bits(bitarray *bitarray, size_t nbits)
{
    if (bitarray[0].limb >= nbits)
    {
        /* clear the entire bitarray */
        bitarray_zero(bitarray);
    }
    else
    {
        size_t n_bytes;

        /*
         * cases:
         *   - (1) nbits align on byte boundary
         *   - (2) nbits does not align on byte boundary,
         *         manually clear remaining bits
         */
        if (nbits % CHAR_BIT == 0)
        {
            n_bytes = nbits * CHAR_BIT;
        }
        else
        {
            size_t remaining_bits = nbits % CHAR_BIT;
            size_t i;

            /* clear bits in the partial byte first */
            for (i = nbits; i > (nbits - remaining_bits); i--)
            {
                bitarray_clear_bit(bitarray, i - 1);
            }
        }

        memset(&bitarray[1], 0, n_bytes);
    }
}


/*
 * Allocation functions
 */
static inline bitarray *bitarray_alloc(unsigned long nbits)
{
        bitarray *ba = NULL;

        size_t n_limbs = 1 + BITARRAY_NUM_BITS_TO_LIMBS(nbits);

        ba = malloc(sizeof(bitarray) * n_limbs);
        if (ba)
        {
            ba[0].limb = nbits;
        }

        return ba;
}

static inline bitarray *bitarray_calloc(unsigned long nbits)
{
        bitarray *ba = NULL;

        size_t n_limbs = 1 + BITARRAY_NUM_BITS_TO_LIMBS(nbits);

        ba = calloc(sizeof(bitarray), n_limbs);
        if (ba)
        {
            ba[0].limb = nbits;
        }

        return ba;
}

static inline void bitarray_free(bitarray *bitarray)
{
    if (bitarray)
    {
        free(bitarray);
    }
}

/*
 * take in a raw buffer and corresponding size and return a pointer to
 * a newly initialised bitarray struct.
 *
 * e.g.
 *     unsigned char p[1024]; // bitarray will be slightly <8192 bits.
 *     bitarray *ba = bitarray_buf_init(p, sizeof(p));
 */
static inline bitarray *bitarray_buf_init(void *buf, size_t buf_size)
{
    bitarray *ba = buf;
    printf("buf_size = %lu\n", buf_size);

    /* clamp to the number of limbs truncating excess bytes */
    size_t nlimbs = buf_size / sizeof(bitarray_word);
    printf("nlimbs = %lu\n", nlimbs);

    /* reserve a limb for storing number of bits */
    size_t nbits = (nlimbs - 1) * sizeof(bitarray_word) * CHAR_BIT;
    printf("nbits = %lu\n", nbits);

    if (nlimbs < 1)
    {
        return NULL;
    }

    if (ba)
    {
        ba[0].limb = nbits;
        bitarray_zero(ba);
    }

    return (ba);
}

static inline void bitarray_print_base16(bitarray *bitarray)
{
    bitarray_word i;
    size_t num_limbs = bitarray_num_limbs(bitarray);

    printf("numbits=%lu\n", sizeof(bitarray[0].limb));
    printf("sizeof(limb)=%lu\n", sizeof(bitarray[0].limb));
    printf("num_limbs=%lu\n", num_limbs);
    for (i = 0; i <= num_limbs; i++)
    {
            unsigned char c;
            unsigned int j;
            size_t limb_size = sizeof(bitarray_word);

            for (j = 0; j < limb_size; j++)
            {
                /* mask the byte we want to print in hex */
                unsigned long mask = (0xFFUL) << (j * CHAR_BIT);
                c = (unsigned char) ((bitarray[i].limb & mask) >> (j * CHAR_BIT));
                printf("%02x", c);
            }

            printf(" ");
        }

    printf("\n");
}


#endif
