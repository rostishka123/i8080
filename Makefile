include_dir    := include
source_dir    := src
compile_flags      := -Wall -MD -pipe

sources= $(source_dir)/i80.c $(source_dir)/cpu.c $(source_dir)/compiler.c $(source_dir)/logic.c $(source_dir)/unfunc.c
objects= $(sources:.c=.o)
prog_name = i80
bin_dir=bin

all: $(bin_dir)/$(prog_name)

$(bin_dir)/$(prog_name): $(objects) $(bin_dir)
	gcc -o $@ $(compile_flags) $(objects) 

$(bin_dir):
	mkdir $(bin_dir)

$(source_dir)/i80.o: $(source_dir)/i80.c
	gcc -o $@ -c $(compile_flags) $(source_dir)/i80.c 

$(source_dir)/logic.o: $(source_dir)/logic.c
	gcc -o $@ -c $(compile_flags) $(source_dir)/logic.c 

$(source_dir)/compiler.o: $(source_dir)/compiler.c
	gcc -o $@ -c $(compile_flags) $(source_dir)/compiler.c 
	

$(source_dir)/cpu.o: $(source_dir)/cpu.c
	gcc -o $@ -c $(compile_flags) $(source_dir)/cpu.c 
	
$(source_dir)/unfunc.o: $(source_dir)/unfunc.c
	gcc -o $@ -c $(compile_flags) $(source_dir)/unfunc.c 

.PHONY : clean

clean: 
	rm -rf $(source_dir)/$(objects) $(source_dir)/*.d  $(source_dir)/i80.o

install:
	sudo cp $(bin_dir)/$(prog_name) /usr/local/bin/
	
uninstall:
	sudo rm /usr/local/bin/$(prog_name)
