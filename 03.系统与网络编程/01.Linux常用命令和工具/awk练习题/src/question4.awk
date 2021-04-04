BEGIN {
    id = 0
}
{
    if($1 != id)
    {
        id = $1
        print "["id"]"
    }
    print $2
}

END {

}
