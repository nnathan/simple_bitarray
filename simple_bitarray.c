#include <stdio.h>
#include <limits.h>
#include <string.h>

#if (__STDC_VERSION__ < 199901L)
#define inline
#endif

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

typedef union
{
    bitarray_word limb;
    bitarray_word num_bits;
} bitarray;

/*
 * To declare a bitarray with automatic storage:
 * {
 *     BITARRAY_DECLARE(x, 1024); // 'x' will now be set to bitarray
 * }
 */
#define BITARRAY_DECLARE(name, nbits) \
    bitarray (name)[1 + BITARRAY_NUM_BITS_TO_LIMBS(nbits)] = { nbits, 0 }

static inline size_t bitarray_num_limbs(bitarray *bitarray)
{
    return BITARRAY_NUM_BITS_TO_LIMBS(bitarray[0].num_bits);
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
    memset(&bitarray[1], 0, bitarray_num_limbs(bitarray));
}

static inline void bitarray_print_base16(bitarray *bitarray)
{
    bitarray_word i;
    size_t num_limbs = bitarray_num_limbs(bitarray);

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

int main(int argc, char **argv)
{
    BITARRAY_DECLARE(ba, 33);
    int x;
    bitarray_change_bit(ba, 0);
    bitarray_change_bit(ba, 1);
    bitarray_change_bit(ba, 2);
    bitarray_change_bit(ba, 3);
    bitarray_change_bit(ba, 4);
    bitarray_change_bit(ba, 5);
    bitarray_change_bit(ba, 6);
    bitarray_change_bit(ba, 7);
    bitarray_change_bit(ba, 8);
    bitarray_change_bit(ba, 9);
    bitarray_change_bit(ba, 10);
    bitarray_change_bit(ba, 11);
    bitarray_change_bit(ba, 12);
    bitarray_change_bit(ba, 13);
    bitarray_change_bit(ba, 14);
    bitarray_change_bit(ba, 15);
    bitarray_change_bit(ba, 16);
    bitarray_change_bit(ba, 17);
    bitarray_change_bit(ba, 18);
    bitarray_change_bit(ba, 19);
    bitarray_change_bit(ba, 20);
    bitarray_change_bit(ba, 21);
    bitarray_change_bit(ba, 22);
    bitarray_change_bit(ba, 23);
    bitarray_change_bit(ba, 24);
    bitarray_change_bit(ba, 25);
    bitarray_change_bit(ba, 26);
    bitarray_change_bit(ba, 27);
    bitarray_change_bit(ba, 28);
    bitarray_change_bit(ba, 29);
    bitarray_change_bit(ba, 30);
    bitarray_change_bit(ba, 31);
    bitarray_change_bit(ba, 32);

    bitarray_print_base16(ba);
    return (0);
}
