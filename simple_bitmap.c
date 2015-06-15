#include <stdio.h>
#include <limits.h>
#include <string.h>

/*
 * this is not guaranteed to be portable since an integral type size
 * doesn't necessarily define the range
 */
#define BITARRAY_LIMB_BITS (sizeof(unsigned long) * CHAR_BIT)

#define BITARRAY_NLIMBS(num_bits)                                      \
    (1 + (((num_bits) + BITARRAY_LIMB_BITS - 1) / BITARRAY_LIMB_BITS))

#define BITARRAY_LIMB(bitarray, n_bit)                    \
    ((bitarray)[1 + ((n_bit) / BITARRAY_LIMB_BITS)].limb)

#define BITARRAY_LIMBBIT(n_bit)                                      \
    ((1UL << (BITARRAY_LIMB_BITS - ((n_bit) % BITARRAY_LIMB_BITS))))

typedef union
{
    unsigned long limb;
    unsigned long num_bits;
} bitarray;

/*
 * To declare a bitarray with automatic storage:
 * {
 *     BITARRAY_DECLARE(x, 1024); // 'x' will now be set to bitarray
 * }
 */
#define BITARRAY_DECLARE(name, nbits) \
    bitarray (name)[BITARRAY_NLIMBS(nbits)] =  { nbits, 0 };

static inline size_t bitarray_num_limbs(bitarray *bitarray)
{
    return BITARRAY_NLIMBS(bitarray[0].num_bits);
}

static inline void bitarray_set_bit(bitarray *bitarray, unsigned long n)
{
    BITARRAY_LIMB(bitarray, n) |= BITARRAY_LIMBBIT(n);
}

static inline void bitarray_clear_bit(bitarray *bitarray, unsigned long n)
{
    BITARRAY_LIMB(bitarray, n) &= ~BITARRAY_LIMBBIT(n);
}

static inline void bitarray_change_bit(bitarray *bitarray, unsigned long n)
{
    BITARRAY_LIMB(bitarray, n) ^= BITARRAY_LIMBBIT(n);
}

static inline int bitarray_test_bit(const bitarray *bitarray, unsigned long n)
{
    return !!(BITARRAY_LIMB(bitarray, n) & BITARRAY_LIMBBIT(n));
}

static inline void bitarray_zero(bitarray *bitarray)
{
    memset(&bitarray[1], 0, bitarray_num_limbs(bitarray));
}

static inline void bitarray_print_base2(bitarray *bitarray)
{
    unsigned long i;

    printf("num_bits=%lu\n", bitarray[0].num_bits);
    for (i = 1; i < bitarray_num_limbs(bitarray); i++)
    {
        printf("%d", bitarray_test_bit(bitarray, i));
    }
}

static inline void bitarray_print_base16(bitarray *bitarray)
{
    unsigned long i;

    printf("num_limbs=%lu\n", bitarray_num_limbs(bitarray));
    for (i = 1; i <= bitarray_num_limbs(bitarray); i++)
    {
        printf("%#lx ", bitarray[i].limb);
    }
}

int main(int argc, char **argv)
{
    BITARRAY_DECLARE(ba, 1024);
    bitarray_zero(ba);
    bitarray_change_bit(ba, 0);
    //bitarray_change_bit(ba, 1);
    //bitarray_change_bit(ba, 2);
    //bitarray_change_bit(ba, 3);
    //bitarray_change_bit(ba, 4);
    //bitarray_change_bit(ba, 5);
    //bitarray_change_bit(ba, 6);
    //bitarray_change_bit(ba, 7);
    //bitarray_change_bit(ba, 8);
    //bitarray_change_bit(ba, 9);
    //bitarray_change_bit(ba, 10);
    //bitarray_change_bit(ba, 11);
    //bitarray_change_bit(ba, 12);
    //bitarray_change_bit(ba, 13);
    //bitarray_change_bit(ba, 14);
    //bitarray_change_bit(ba, 15);
    //bitarray_change_bit(ba, 16);
    //bitarray_change_bit(ba, 17);
    //bitarray_change_bit(ba, 18);
    //bitarray_change_bit(ba, 19);
    //bitarray_change_bit(ba, 20);
    //bitarray_change_bit(ba, 21);
    //bitarray_change_bit(ba, 22);
    //bitarray_change_bit(ba, 23);
    //bitarray_change_bit(ba, 24);
    //bitarray_change_bit(ba, 25);
    //bitarray_change_bit(ba, 26);
    //bitarray_change_bit(ba, 27);
    //bitarray_change_bit(ba, 28);
    //bitarray_change_bit(ba, 29);
    //bitarray_change_bit(ba, 30);
    bitarray_change_bit(ba, 31);
    //bitarray_change_bit(ba, 32);

    bitarray_print_base16(ba);
    printf("\n");
    bitarray_print_base2(ba);
    printf("\n");
    return (0);
}
