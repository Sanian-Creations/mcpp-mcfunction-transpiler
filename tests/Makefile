MAKEFLAGS += --no-builtin-rules



obj_dir := ../obj/
src_dir := ../src/
target_exec := run_tests.exe

test_sources := $(shell ls *.c)
test_objects := $(test_sources:.c=.o)

src_sources := $(filter-out $(src_dir)main.c, $(shell ls $(src_dir)*.c))
src_objects := $(addprefix $(obj_dir), $(notdir $(src_sources:.c=.o)))

# main stuff
$(target_exec): $(test_objects) # doesn't have to depend on src_objs, first command runs make in src so that they are up to date.
	(cd $(src_dir) && make objs)
	gcc $(test_objects) $(src_objects) -o $@

%.o: %.c
	gcc $< -c -o $@

# make commands
.PHONY: clean rebuild run
clean:
	rm *.o

rebuild: clean $(target_exec)

run: $(target_exec)
	$(target_exec)
