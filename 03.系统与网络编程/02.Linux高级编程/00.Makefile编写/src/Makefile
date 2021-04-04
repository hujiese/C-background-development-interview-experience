src = $(wildcard ./*.c)
obj = $(patsubst %.c, %.o, $(src))
target = app
$(target):$(obj)
	gcc $^ -o $@
%.o:%.c
	gcc -c $< -o $@

hello:
	echo "hello, Makefile"

.PHONY:clean
clean:
	#-mkdir /abc
	-rm $(obj) $(target) -f
