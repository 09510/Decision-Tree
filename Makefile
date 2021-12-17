CC = g++
CFLAGS = -Wall -shared -std=c++11 -g -fPIC

PINC = `python3 -m pybind11 --includes`
SUFFIX = `python3-config --extension-suffix`
EXEC = forest




MODNAME=_forest.so


.PHONY: mod
mod: ${MODNAME}
${MODNAME}:_forest.cpp _forest.hpp
	$(CC) $(CFLAGS) $(PINC) $< -o ${MODNAME} -I/usr/include/python3.8


.PHONY: clean
clean:
	rm -rf $(EXEC) *.o *.so ./__pycache__ ./.pytest_cache 



.PHONY: run
run:mod
	python3 class_test.py