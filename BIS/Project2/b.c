#include <stdio.h>

int main(int argc, const char *argv[]) {
	FILE *f = fopen("address", "wb");
	int address = 0xffffdc38;
	fwrite(&address, sizeof(address), 1, f);
	fclose(f);

	return 0;
}
