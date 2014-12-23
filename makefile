TARGET= chatRoomServer chatRoomClient

normal: $(TARGET)

chatRoomServer: ChatRoomServer.c
	gcc ChatRoomServer.c -o chatRoomServer

chatRoomClient: ChatRoomClient.c
	gcc ChatRoomClient.c -o chatRoomClient

clean:
	$(RM) $(TARGET)
