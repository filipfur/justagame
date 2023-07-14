build=cmake_build

.PHONY: target
target: | $(build)
	cd $(build) && cmake .. && $(MAKE)


$(build):
	mkdir -p $(build)

.PHONY: test
test:
	make -C test

.PHONY: clean
clean:
	rm -rf $(build)