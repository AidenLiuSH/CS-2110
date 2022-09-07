/**
 * CS 2110 Fall 2021 HW1
 * Part 3 - Coding with bitwise operators
 *
 * @author Shihui Liu
 *
 * Global rules for this file:
 * - All of these functions must be completed in ONE line. That means they
 *   should be of the form "return [...];". No partial credit will be awarded
 *   for any method that isn't completed in one line.
 *
 * - You may not use conditionals.
 * - You may not declare any variables.
 * - You may not use casting.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any method from this or
 *   another file to implement any method. Recursive solutions are not
 *   permitted.
 * - You may not use addition or subtraction. However, you may use the
 *   unary negation operator (-) to negate a number.
 * - You may not use multiplication, division or modulus
 *
 * - Basically, the global rules are you can only use logical (&&, ||) and
 *   bitwise (&, |, ^, >>, <<) operators, as well as the unary negation (-)
 *   operator, all in one line.
 *
 * Method-specific rules for this file:
 * - You may not use bit shifting or the exclusive OR operator (^) in xor.
 *
 * Some notes:
 *
 * All of these functions accept ints as parameters because if you pass in a
 * number (which is of type int by default) into a Method accepting a byte, then
 * the Java compiler will complain even if the number would fit into that type.
 *
 * Now, keep in mind the return value is also an int. Please read the comments
 * about how many significant bits to return and make sure that the other bits
 * are not set or else you will not get any points for that test case. For
 * example, if we ask you to return 6 bits and you return "0xFFFFFFFF", you will
 * lose points.
 *
 * Definitions of types:
 * nibble - 4 bits
 * byte   - 8 bits
 * short  - 16 bits
 * int    - 32 bits
 */
public class Operations
{
    /**
     * Get a 8-bit byte from an int.
     *
     * Ints are made of 4 bytes, numbered like so:
     *   |   N3   |   N2   |   N1   |   N0   |
     *
     * For a graphical representation of the bits:
     *
     * bit 31                             bit 0
     *    v                                 v
     *    01100010 00110001 10100001 00000001
     *   +--------+--------+--------+--------+
     *   |   N3   |   N2   |   N1   |   N0   |
     *
     * Examples:
     *     getByte(0x56781234, 0); // => 0x34
     *     getByte(0x56781234, 2); // => 0x78
     *
     * Note: Remember, no multiplication allowed!
     *
     * @param num The int to get a byte from.
     * @param which Determines which byte gets returned - 0 for
     *              least-significant byte.
     *
     * @return A byte from num corresponding to the "which" parameter.
     */
    int getByte(int num, int which) {
        return num >> (which << 3) & 0b11111111;
    }

    /**
     * Set a 4-bit nibble in an int.
     *
     * Ints are made of 8 nibbles, numbered like so:
     *   | N7 | N6 | N5 | N4 | N3 | N2 | N1 | N0 |
     *
     * For a graphical representation of the bits:
     *
     * bit 31                                 bit 0
     *    v                                     v
     *    0011 0010 0011 0001 0011 0001 0011 0001
     *   +----+----+----+----+----+----+----+----+
     *   | N7 | N6 | N5 | N4 | N3 | N2 | N1 | N0 |
     *
     * Examples:
     *     setNibble(0x56789abc, 0x1, 0); // => 0x56789ab1
     *     setNibble(0xdef01234, 0x5, 3); // => 0xdef05234
     *
     * Note: Remember, no multiplication allowed!
     *
     * @param num The int that will be modified.
     * @param a_nibble The nibble to insert into the integer.
     * @param which Selects which nibble to modify - 0 for least-significant
     * nibble.
     *
     * @return The modified int.
     */
    int setNibble(int num, int a_nibble, int which) {
        return (num & ~((0b1111) << (which << 2))) | (a_nibble << (which << 2));
    }

    /**
     * Pack a byte, a short, and another byte into an int.
     *
     * They should be placed consecutively in the 32-bit int in the order that
    *  they appear in the parameters.
     *
     * Example:
     *     pack(0x12, 0x3456, 0x78); // => 0x12345678
     *     pack(0xCO, 0xFFEE, 0x10); // => 0xCOFFEE10
     *
     * @param b2 Most significant byte (will always be a 8-bit number).
     * @param s1 Middle short (will always be a 16-bit number).
     * @param b0 Least significant byte (will always be a 8-bit number).
     *
     * @return a 32-bit value formatted like so: b2s1b0
     */
    int pack(int b2, int s1, int b0) {
        return b2 << 24 | s1 << 8 | b0;
    }

    /**
     * Extract a range of bits from a number.
     *
     * Examples:
     *     bitRange(0x00001234, 0, 4);  // => 0x00000004
     *     bitRange(0x00001234, 4, 8);  // => 0x00000023
     *     bitRange(0x12345678, 0, 28); // => 0x02345678
     *     bitRange(0x55555555, 5, 7);  // => 0x0000002A
     *
     * Note: We will only pass in values 1 to 32 for n.
     *
     * @param num An n-bit 2's complement number.
     * @param s The starting bit to grab
     * @param n The number of bits to return.
     * @return The n-bit number num[s:s+n-1].
     */
    int bitRange(int num, int s, int n) {
        return (num >> s) & ~(~0 << n);
    }

    /**
     * NOTE: For this method, you may only use &, |, and ~.
     *
     * Perform an exclusive-or on two 32-bit ints.
     *
     * Examples:
     *     xor(0xFF00FF00, 0x00FF00FF); // => 0xFFFFFFFF
     *     xor(0x12345678, 0x87654321); // => 0x95511559
     *
     * @param num1 An int
     * @param num2 Another int
     *
     * @return num1 XOR num2
     */
    int xor(int num1, int num2) {
       return (num1 & ~num2) | (~num1 & num2);
    }

    /**
     * Return true if the given number is a multiple of 2.
     *
     * Examples:
     *     multipleOf2(32);   // => true
     *     multipleOf2(13);   // => false
     *
     * Note: Make sure you handle ALL the cases!
     *
     * @param num a 32-bit int. Since this is an int, it is SIGNED!
     * @return true if num is a multiple of 2 else false.
     */

    boolean multipleOf2(int num) {
        return num == ((num >> 1) << 1);
    }

    /**
     * Return true if the given number is a multiple of 16.
     *
     * Examples:
     *     multipleOf16(256); // => true
     *     multipleOf16(135); // => false
     *     multipleOf16(88);  // => false
     *     multipleOf16(96);  // => true
     *
     * Note: Make sure you handle ALL the cases!
     *
     * @param num a 32-bit int. Since this is an int, it is SIGNED!
     * @return true if num is a multiple of 16 else false.
     */

    boolean multipleOf16(int num) {
        return num == ((num >> 4) << 4);
    }

    /**
     * Return true if the given number is a power of 2.
     *
     * Examples:
     *     powerOf2(32);   // => true
     *     powerOf2(12);   // => false
     *     powerOf2(1);    // => true
     *
     * Note: Make sure you handle ALL the cases!
     *
     * @param num a 32-bit int. Since this is an int, it is SIGNED!
     * @return true if num is a multiple of 2 else false.
     */

    boolean powerOf2(int num) {
        return (num != -num) && ((num & -num) == num);
    }
}
