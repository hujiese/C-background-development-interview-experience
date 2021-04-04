{
    ++url[$3]    
}

END{
    for(i in url)
    {
        print url[i] " " i
    }
}
