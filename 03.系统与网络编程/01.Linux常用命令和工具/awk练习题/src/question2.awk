BEGIN {
    max_ip = 0
    max_ip_cnt = 0
    max_usr = 0
    max_usr_cnt = 0
}

{
    ++ip[$1]
    ++host[$2]
}

END {
    for(i in ip)
    {
        if(ip[i] > max_ip_cnt)
        {
            max_ip = i
            max_ip_cnt = ip[i]
        }
    }
    printf "%-10s %2d\n", max_ip, max_ip_cnt

    for (j in host)
    {
       if(host[j] > max_usr_cnt)
       {
            max_usr = j
            max_usr_cnt = host[j]
       }
    }
    printf "%-10s %2d\n", max_usr, max_usr_cnt
}
