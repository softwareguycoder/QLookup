#include <winsock.h>

#pragma comment(lib, "Ws2_32.lib")

#define PROG_NAME "Simple DNS Lookup"

#define HOST_NAME "GOOGLE.COM"		// This can be any valid host name
#define WINSOCK_VERSION	0x0101		// Program requires Winsock version 1.1
#define AF_INET_LENGTH	4			// Address length for Internet protocol family is always 16 bytes

#define HOST_ADDR "129.79.26.27"	// 027.inlocc.iu.edu

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, INT nCmdShow)
{
	WSADATA wsaData;				// Winsock implementation details
	LPHOSTENT lpHostEnt;			// Internet host information structure
	DWORD dwIPAddr;					// IP address as an unsigned long
	LPSTR szIPAddr;					// IP address as a dotted decimal string

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		MessageBox(NULL, "Could not load Windows Sockets DLL.",
			PROG_NAME, MB_OK | MB_ICONSTOP);

		WSACleanup();	// Free all allocated program resources and exit
		return 1;
	}

	// Resolve the host name
	lpHostEnt = gethostbyname(HOST_NAME);

	if (!lpHostEnt)
	{
		MessageBox(NULL, "Could not get IP address.",
			PROG_NAME, MB_OK | MB_ICONSTOP);

		WSACleanup();	// Free all allocated program resources and exit
		return 1;
	}

	// Convert the IP address into a dotted-decimal string
	szIPAddr = inet_ntoa(*(LPIN_ADDR)*(lpHostEnt->h_addr_list));
	MessageBox(NULL, szIPAddr,
		lpHostEnt->h_name, MB_OK | MB_ICONASTERISK);

	// Convert a dotted-decimal string into a 32-bit IP address
	dwIPAddr = inet_addr(HOST_ADDR);

	if (dwIPAddr == INADDR_NONE)
	{
		MessageBox(NULL, "Invalid Internet address.",
			PROG_NAME, MB_OK | MB_ICONSTOP);

		WSACleanup();	// Free all allocated program resources and exit
		return 1;
	}

	// Resolve the IP address
	lpHostEnt = gethostbyaddr((LPSTR)&dwIPAddr, AF_INET_LENGTH, AF_INET);

	if (!lpHostEnt)
	{
		char *s = NULL;

		// Translates the output of the WSAGetLastError() function into a
		// human-understandable message.
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&s, 0, NULL);

		MessageBoxA(NULL, s, PROG_NAME, MB_OK | MB_ICONSTOP);

		LocalFree(s);

		WSACleanup();	// Free all allocated program resources and exit
		return 1;
	}

	// Display the name of the host with the IP address defined by the HOST_ADDR symbol
	MessageBox(NULL, lpHostEnt->h_name, HOST_ADDR,
		MB_OK | MB_ICONASTERISK);

	WSACleanup();	// Free all allocated program resources and exit
	return 0;
}