/*
 *
 *
 *
 *
 */

#define SERVER_COUNT 2

#define REQ_CHAN_SIZE 4
#define REP_CHAN_SIZE 4

#define CNT_LEN 2

bool c1req = false;
bool c2req = false;
bool c3req = false;

bool c1rep = false;
bool c2rep = false;
bool c3rep = false;

chan request = [REQ_CHAN_SIZE] of
{
	byte,        // client ID
	byte         // message num
};

chan reply = [REP_CHAN_SIZE] of
{
	byte,        // server ID
	byte,        // client ID
	byte         // message reply num
};


active [SERVER_COUNT] proctype Server()
{
	byte client;
	byte message;

end:
	do
	:: request ? client, message ->
		reply ! _pid, client, message;
		printf("Server %d processed request %d from client %d\n", _pid, message, client);
	od;
}


active proctype Client1()
{
	byte server;
	byte cnt;

	do
	:: cnt == CNT_LEN ->
		cnt = 0;
	:: else ->
		// clear flags
		c1req = false;
		c1rep = false;

		// send request
		request ! _pid, cnt;
		c1req = true;

		// receive reply
		reply ?? server, eval(_pid), eval(cnt);
		c1rep = true;

		// increment counter
		cnt++;

		printf("Reply received by client %d from server %d\n", _pid, server);
	od;
}


active proctype Client2()
{
	byte server;
	byte cnt;

	do
	:: cnt == CNT_LEN ->
		cnt = 0;
	:: else ->
		// clear flags
		c2req = false;
		c2rep = false;

		// send request
		request ! _pid, cnt;
		c2req = true;

		// receive reply
		reply ?? server, eval(_pid), eval(cnt);
		c2rep = true;

		// increment counter
		cnt++;

		printf("Reply received by client %d from server %d\n", _pid, server);
	od;
}


active proctype Client3()
{
	byte server;
	byte cnt;

	do
	:: cnt == CNT_LEN ->
		cnt = 0;
	:: else ->
		// clear flags
		c3req = false;
		c3rep = false;

		// send request
		request ! _pid, cnt;
		c3req = true;

		// receive reply
		reply ?? server, eval(_pid), eval(cnt);
		c3rep = true;

		// increment message sequence number
		cnt++;

		printf("Reply received by client %d from server %d\n", _pid, server);
	od;
}
