BEGIN {
    sum = 0   
}

{
    if( NR == 1 )
    {
        sum = $1
        printf "%3d\n", sum
    }
    else if ( NR == 5 )
    {
        printf "%-2s %3d\n", $1, $2
    }
    else
    {
        sum += $2
        printf "%-2s %3d\n%3d\n", $1, $2, sum
    }
}

END {

}
