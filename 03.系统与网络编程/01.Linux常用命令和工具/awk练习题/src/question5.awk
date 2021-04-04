BEGIN { 
    max = 0
    maxIp
}

{
    ++IP[$1]

}

END {
    for(i in IP)
    {
        if(IP[i] > max)
            max = IP[i]
            maxIp = i
    }
    print maxIp
}
