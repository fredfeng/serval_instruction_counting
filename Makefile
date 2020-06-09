CONTAINER_ID = 6425e548de65

all:
	docker cp sort.c $(CONTAINER_ID):/serval/test
	docker cp sort.rkt $(CONTAINER_ID):/serval/test
	docker cp parse.rkt $(CONTAINER_ID):/serval/serval/llvm/
	docker cp llvm.rkt $(CONTAINER_ID):/serval/serval/llvm.rkt
	docker exec $(CONTAINER_ID) make -C test check-racket-sort

setup:
	docker exec $(CONTAINER_ID) raco pkg install --auto
	docker exec $(CONTAINER_ID) export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/lib64"
