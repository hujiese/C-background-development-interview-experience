target = app
flag = -I ./
src = $(wildcard ./*.c)
obj = $(patsubst %.c, %.o, $(src))
$(target):$(obj)
#	gcc $(obj) -o app
	gcc $^ -o $@

# 使用模式规则
%.o:%.c
	gcc -c $< -o $@ $(flag) -g -std=c99

.PHONY:clean
clean:
	-rm $(obj) $(target) -f
