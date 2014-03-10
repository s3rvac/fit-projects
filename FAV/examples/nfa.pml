/*
 *
 *
 *
 *
 *
 */

inline GetRandomChar()
{
	if
	:: i = 'l';
	:: i = 'i';
	:: i = 'b';
	:: i = 'o';
	:: i = 'r';
	fi;
}


active proctype NFA()
{
	byte i;

	GetRandomChar();

q1:
	if
	:: i == 'l' -> printf("l"); GetRandomChar(); goto q1;
	:: i == 'l' -> printf("l"); GetRandomChar(); goto q4;
	:: i == 'i' -> printf("i"); GetRandomChar(); goto q2;
	:: i == 'i' -> printf("i"); GetRandomChar(); goto q3;
	:: else -> GetRandomChar(); goto q1;
	fi;

q2:
	if
	:: i == 'b' -> printf("b"); GetRandomChar(); goto q4;
	:: else -> GetRandomChar(); goto q2;
	fi;

q3:
	if
	:: i == 'l' -> printf("l"); GetRandomChar(); goto q4;
	:: else -> GetRandomChar(); goto q3;
	fi;

q4:
	if
	:: i == 'o' -> printf("o"); GetRandomChar(); goto q4;
	:: i == 'r' -> printf("r"); GetRandomChar(); goto accept;
	:: else -> GetRandomChar(); goto q4;
	fi;

accept:
	printf("\n\nAccepted\n");
}
