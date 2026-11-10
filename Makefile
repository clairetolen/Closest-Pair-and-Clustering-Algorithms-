CC = g++
CFLAGS = -Wall -Wextra

submission: submission.cpp
	$(CC) $(CFLAGS) submission.cpp -o submission

clean:
	rm -f submission
	rm -f output.txt