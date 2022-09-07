/**
 * CS 2110 Fall 2021 HW1
 * Part 2 - Coding with bases
 *
 * @author Shihui Liu
 *
 * Global rules for this file:
 * - You may not use more than 2 conditionals per method. Conditionals are
 *   if-statements, if-else statements, or ternary expressions. The else block
 *   associated with an if-statement does not count toward this sum.
 * - You may not use more than 2 looping constructs per method. Looping
 *   constructs include for loops, while loops and do-while loops.
 * - You may not use nested loops.
 * - You may not declare any file-level variables.
 * - You may not use switch statements.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any method from this or
 *   another file to implement any method. Recursive solutions are not
 *   permitted.
 * - The only Java API methods you are allowed to invoke are:
 *     String.length()
 *     String.charAt()
 * - You may not invoke the above methods from string literals.
 *     Example: "12345".length()
 * - When concatenating numbers with Strings, you may only do so if the number
 *   is a single digit.
 *
 * Method-specific rules for this file:
 * - You may not use multiplication, division or modulus in any method, EXCEPT
 *   decimalStringToInt (where you may use multiplication only)
 * - You may declare exactly one String variable each in intToHexString and
 *   and binaryStringToOctalString.
 */
public class Bases
{

    public static int binaryStringToInt(String binary)
    {
        int decimal = 0;

        for(int i = 0; i < binary.length(); i++) {
            if(binary.charAt(i) == '1') {
                decimal += 1 << (binary.length() - i - 1);
            }
        }

        return decimal;
    }

    /**
     * Convert a string containing ASCII characters (in decimal) to an int.
     *
     * You do not need to handle negative numbers. The Strings we will pass in
     * will be valid decimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: decimalStringToInt("46"); // => 46
     *
     * You may use multiplication in this method.
     */
    public static int decimalStringToInt(String decimal)
    {
        int num = 0;
        for(int i  = 0; i < decimal.length(); i++) {
            num = (int)decimal.charAt(i) - 48 + num * 10;
        }
        return num;
    }

    /**
     * Convert a string containing ASCII characters (in octal) to an int.
     *
     * You do not need to handle negative numbers. The Strings we will pass in
     * will be valid octal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: octalStringToInt("27"); // => 23
     */
    public static int octalStringToInt(String octal)
    {
        int num = 0;
        for(int i  = 0; i < octal.length(); i++) {
            num = (int)octal.charAt(i) - 48 + (num << 3);
        }
        return num;
    }

    /**
     * Convert a int into a String containing ASCII characters (in hex).
     *
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters
     * necessary to represent the number that was passed in.
     *
     * Example: intToHexString(166); // => "A6"
     *
     * You may declare one String variable in this method.
     */
    public static String intToHexString(int hex)
    {
        String hexString = "";
        while (hex > 15) {
            int quotient = hex >> 4;
            int remainder = hex - ((hex >> 4) << 4);
            if (remainder < 10) {
                hexString = (char) (remainder + 48) + hexString;
            } else {
                hexString = (char) (remainder + 55) + hexString;
            }
            hex = quotient;
        }
        if (hex < 10) {
            hexString = (char) (hex + 48) + hexString;
        } else {
            hexString = (char) (hex + 55) + hexString;
        }
        return hexString;
    }

    /**
     * Convert a String containing ASCII characters representing a number in
     * binary into a String containing ASCII characters that represent that same
     * value in octal.
     *
     * The output string should only contain numbers.
     * You do not need to handle negative numbers.
     * The length of all the binary strings passed in will be of size 24.
     * The octal string returned should contain 8 characters.
     *
     * Example: binaryStringToOctalString("000001101010000111100100"); // => "01520744"
     *
     * You may declare one String variable in this method.
     */
    public static String binaryStringToOctalString(String binary)
    {
        String hexString = "";
        int temp = 0;
        int count = 2;
        for (int i = 0; i < binary.length(); i++) {
            temp += ((int) binary.charAt(i) - 48) << count;
            count--;
            if (count < 0) {
                hexString += (char) (temp + 48);
                count = 2;
                temp = 0;
            }
        }
        return hexString;
    }
}
