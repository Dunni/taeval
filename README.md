TAEval
KungFu Pandas


Building:
	Server:
	-- cd(change dir) into ServerSide
	-- qmake TAEvalServer.pro
	-- make

	Client:
	-- cd into ClientSide
	-- qmake TAEvalClient.pro
	-- make

Configuraion:
	-- modify config.ini in Common/Communication
		HostAddress="change on the client side"
		ServerIp="change on the Server side"
		portNumber=60009
	-- HostAddress, used by Client,specifys the Server IP to which it connects.
	-- ServerIp, used by Server, specifys the IP to which it listens.
	-- portNumber, shared by Server and Client, specifys the port being used.

Executables:
	Server: ServerSide/TAEvalServer
	Client: ClientSide/TAEvalClient


Remarks:
	-- the HostAddress should be the same to ServerIp in most cases, but in our Lab try the following:
		HostAddress="ServerIP"
		ServerIp="10.0.2.15"
		portNumber=60009
