TARGET = chatRoomServer chatRoomClient

normal:$(TARGET)

chatRoomServer : chatRoomServer.c
	gcc -Wall chatRoomServer.c -o chatRoomServer

chatRoomClient : chatRoomClient.c
	gcc -Wall chatRoomClient.c -o chatRoomClient

clean:
	$(RM) $(TARGET)
