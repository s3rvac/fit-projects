/* Program that crashes when started (core dump testing) */

int main(void) {
	int a = *((char *)0);
	return 0;
}
