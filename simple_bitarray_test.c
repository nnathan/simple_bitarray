#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "simple_bitarray.h"

#define STR(x) #x
#define STRINGIFY(x) STR(x)
#define FILE_LINE __FILE__ ":" STRINGIFY(__LINE__)

#define mu_assert(message, test) \
    do                           \
    {                            \
        if (!(test))             \
        {                        \
            return FILE_LINE ": " message; \
        }                                  \
    } while (0)

#define mu_run_test(test)    \
do                           \
{                            \
    char *message = test();  \
    tests_run++;             \
    if (message)             \
    {                        \
        return message;      \
    }                        \
} while(0)


int tests_run = 0;
 
static char *test_declare()
{
    size_t i = 0;
    BITARRAY_DECLARE(b, 1024);

    size_t n_limbs = BITARRAY_NUM_BITS_TO_LIMBS(1024);

    /* verify correct size. */
    mu_assert(
        "sizeof(b) incorrect",
        sizeof(b) == sizeof(bitarray) * (n_limbs + 1)
    );

    /* verify the first limb contains the number of bits. */
    mu_assert("b[0].limb != 1024", b[0].limb == 1024);

    /* verify all remaining limbs are zeroed. */
    for (i = 1; i <= n_limbs; i++)
    {
        mu_assert("b has a non-zero limb", b[i].limb == 0);
    }

    return 0;
}

static char *test_declare_empty_size()
{
    size_t i = 0;
    BITARRAY_DECLARE(b, 0);

    /* verify the first limb contains the number of bits. */
    mu_assert(
        "should have size 1 limb",
        sizeof(b) == sizeof(bitarray)
    );

    mu_assert("b[0].limb != 0", b[0].limb == 0);

    return 0;
}

/*
 * TODO: break this unit into smaller unit tests:
 *  - test_set_bit
 *  - test_clear_bit
 *  - test_test_bit
 *
 * Each "limb" should be manually checked instead.
 *
 */
static char *test_set_bits()
{
    size_t i = 0;
    BITARRAY_DECLARE(b, 128);

    /* set all bits */
    for (i = 0; i < 128; i++)
    {
        bitarray_set_bit(b, i);
    }

    /* verify */
    for (i = 0; i < 128; i++)
    {
        mu_assert(
            "bitarray_get_bit(b, i) should be set to 1",
            bitarray_test_bit(b, i)
        );
    }

    /* set every second bit */
    for (i = 0; i < 128; i += 2)
    {
        bitarray_clear_bit(b, i);
    }

    /* verify */
    for (i = 0; i < 128; i++)
    {
        if ((i % 2) == 1)
        {
            mu_assert(
                "bitarray_test_bit(b, i) should be set to 1",
                bitarray_test_bit(b, i)
            );
        }
        else
        {
            mu_assert(
                "bitarray_test_bit(b, i) should be set to 0",
                !bitarray_test_bit(b, i)
            );
        }
    }

    return 0;
}

static char *test_buf_init_too_small()
{
    unsigned char buf[1] = { UCHAR_MAX };
    bitarray *ba = NULL;

    ba = bitarray_buf_init(buf, sizeof(buf));
    mu_assert("ba should be NULL", ba == NULL);
    mu_assert("buf should be unchanged", buf[0] == UCHAR_MAX);

    return 0;
}

static char *test_buf_init_unaligned()
{
    unsigned char buf[sizeof(bitarray) + 2];
    bitarray *ba = NULL;

    /* set all bits in buffer */
    memset(buf, UCHAR_MAX, sizeof(buf));
    printf("last byte == %lu\n", buf[sizeof(buf) - 1]);

    /* reserve the last byte in buf for testing */
    ba = bitarray_buf_init(buf, sizeof(buf) - 1);

    mu_assert("ba should point to buf", ba == (bitarray *) buf);
    mu_assert("ba[0].limb == CHAR_BIT", ba[0].limb == CHAR_BIT);
    printf("last byte == %lu\n", buf[1]);
    mu_assert(
        "last byte of buf should have all bits set",
        buf[sizeof(buf) - 1] == UCHAR_MAX
    );

    return 0;
}

static char *all_tests()
{
    mu_run_test(test_declare);
    mu_run_test(test_declare_empty_size);
    mu_run_test(test_set_bits);
    mu_run_test(test_buf_init_too_small);
    mu_run_test(test_buf_init_unaligned);
    return 0;
 }

int main(int argc, char **argv)
{
    char *result = all_tests();

    if (result != 0)
    {
        printf("%s\n", result);
        printf("TEST FAILURE\n");
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

