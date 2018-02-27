all:
	gcc samsungtest.c -o predictor

run:all
	./predictor

clean:
	rm -rf predictor Testing/output.txt

display:run
	cat Testing/input.txt Testing/output.txt
