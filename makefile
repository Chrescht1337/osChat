TARGET= chatRoomServer chatRoomClient

normal: $(TARGET)

chatRoomServer: ChatRoomServer.c
	gcc -Wall ChatRoomServer.c -o chatRoomServer

chatRoomClient: ChatRoomClient.c
	gcc -Wall ChatRoomClient.c -o chatRoomClient

clean:
	$(RM) $(TARGET)
