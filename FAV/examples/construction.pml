/*
 *
 *
 *
 *
 *
 *
 */

#define QUEUE_SIZE 4
#define TUNNEL_SIZE 4

// queues in front of semaphores
chan left_queue = [QUEUE_SIZE] of { byte };
chan right_queue = [QUEUE_SIZE] of { byte };

// semaphore lights
bool left_green = true;
bool right_green = false;

// a car wants to enter the tunnel
bool left_request = false;
bool right_request = false;

// car(s) are going to left/right
bool going_to_left = false;
bool going_to_right = false;

bool tunnel_entered = true;

byte cars_in_tunnel = 0;

active proctype CarGenerator()
{
	do
	:: left_queue ! 0;
	:: right_queue ! 1;
	od;
}


active proctype LeftSemaphore()
{
	byte car;

	do
	::
		left_queue ? car;
		atomic
		{
			left_request = true;
			printf("Left request.\n");
		}
		atomic
		{
			cars_in_tunnel < TUNNEL_SIZE;
			printf("Cars in tunnel = %d\n", cars_in_tunnel);
		}
		atomic
		{
			left_green;
			tunnel_entered = true;
			going_to_right = true;
			cars_in_tunnel++;
			printf("Left: car %d entered the tunnel. Cars in tunnel: %d\n", car, cars_in_tunnel);
			left_request = false;
		}

	od;
}

active proctype RightSemaphore()
{
	byte car;

	do
	::
		right_queue ? car;
		atomic
		{
			right_request = true;
			printf("Right request.\n");
		}
		atomic
		{
			cars_in_tunnel < TUNNEL_SIZE;
			printf("Cars in tunnel = %d\n", cars_in_tunnel);
		}
		atomic
		{
			right_green;
			tunnel_entered = true;
			going_to_left = true;
			cars_in_tunnel++;
			printf("Right: car %d entered the tunnel. Cars in tunnel: %d\n", car, cars_in_tunnel);
			right_request = false;
		}

	od;
}

active proctype SemaphoreCommander()
{
	do
	:: left_green && right_request && tunnel_entered ->
		atomic
		{
			left_green = false;
			printf("Left is red: waiting for clearing the tunnel\n");
		}
		assert (!right_green && !left_green);
		cars_in_tunnel == 0;
		atomic
		{
			right_green = true;
			tunnel_entered = false;
			printf("Right is green\n");
		}
	:: right_green && left_request && tunnel_entered ->
		atomic
		{
			right_green = false;
			printf("Right is red: waiting for clearing the tunnel\n");
		}
		assert (!right_green && !left_green);
		cars_in_tunnel == 0;
		atomic
		{
			left_green = true;
			tunnel_entered = false;
			printf("Left is green\n");
		}
	od;
}


active proctype TunnelEnd()
{
	do
	:: going_to_right ->
		atomic
		{
			assert (cars_in_tunnel > 0);
			cars_in_tunnel--;
			printf("Left: a car has left the tunnel. Cars in tunnel: %d\n", cars_in_tunnel);
			if
			:: cars_in_tunnel == 0 ->
				going_to_right = false;
			:: else ->
			fi;
		}
	:: going_to_left ->
		atomic
		{
			assert (cars_in_tunnel > 0);
			cars_in_tunnel--;
			printf("Right: a car has left the tunnel. Cars in tunnel: %d\n", cars_in_tunnel);
			if
			:: cars_in_tunnel == 0 ->
				going_to_left = false;
			:: else ->
			fi;
		}
	od;
}
