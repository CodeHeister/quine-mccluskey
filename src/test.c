#include <inttypes.h>

#include <mccluskey.h>

int main() {

	char rows[ROWS];
	uint16_t buffer;

	scanf_minmap(&buffer);
	clean_rows(rows);
	int2rows(rows, &buffer);

	minimalise(rows, &buffer);
	print_rows(rows);
	
	return 0;
}
