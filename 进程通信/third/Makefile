all: sender receiver
sender: sender.c
	gcc sender.c -o sender -pthread
receiver: receiver.c
	gcc receiver.c -o receiver -pthread
clean:
	rm sender receiver
