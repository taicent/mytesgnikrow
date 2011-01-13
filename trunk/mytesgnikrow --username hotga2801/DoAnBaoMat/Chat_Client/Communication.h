#if !defined asdfldsjfnasdjklfnao
#define asdfldsjfnasdjklfnao
#include "StdAfx.h"

void sendText(SOCKET sock,CString message)
{
	int Len=message.GetLength();
	char* sendBuff=new char[Len+1];
	strcpy(sendBuff,LPCTSTR(message));
	
	//Send kich thuoc truoc
	send(sock,(char*)&Len,sizeof(Len),0);
	send(sock,(char*)sendBuff,Len,0);
	delete sendBuff;
}
void sendText_1(SOCKET sock,CString MessageHeader,char * Data, int DataSize)
{
	int HeaderSize = MessageHeader.GetLength();
	int Len=HeaderSize  + DataSize;
	char* sendBuff=new char[Len+1];
	strcpy(sendBuff,LPCTSTR(MessageHeader));
	for (int i =0 ; i < DataSize ; i++)
	{
		sendBuff[i+HeaderSize ] = Data[i];
	}
	send(sock,(char*)&Len,sizeof(Len),0);
	send(sock,(char*)sendBuff,Len,0);
	/*
	char bufferrr[10];
	itoa(Len,bufferrr,10);
	CString Thongbao ;
	Thongbao = "truyen di so byte la :";
	Thongbao += bufferrr;	
	AfxMessageBox(Thongbao);
	*/
	delete sendBuff;

}
void recvText()
{

}

void EndConnect(SOCKET & socketClient)
{
	
	shutdown(socketClient,SD_BOTH);
	closesocket(socketClient);
	WSACleanup();
}
void readfile();
void IntServerSock(sockaddr_in & serverAdd ,SOCKET & sockServer,int Port)
{

	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	serverAdd.sin_family=AF_INET;
	serverAdd.sin_port = htons(Port);    
	serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	while( bind(sockServer, (SOCKADDR *)&serverAdd, sizeof(serverAdd))== SOCKET_ERROR)
	{
		Port++;
		serverAdd.sin_port= htons(Port  );
		
	}
	listen(sockServer,5);

}

#endif