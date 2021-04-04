
BEGIN {
    math = 0
    english = 0
    computer = 0
    printf "NAME    NO.   MATH  ENGLISH  COMPUTER   TOTAL\n"
    printf "---------------------------------------------\n"
}

{
   math += $3
   english += $4
   computer += $5

   printf "%-6s %-4s %5d %5d %5d %5d\n", $1, $2, $3, $4, $5, $1+$2+$3
}

END{
    printf "TOTAL: %10d %8d %6d\n", math, english, computer
    printf "AVG: %10.2f %8.2f %6.2f\n", math/NR, english/NR, computer/NR
}
