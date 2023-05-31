// Quine-McCluskey.c v1.0.0

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

// bit manipulations and prototypes

#include <mccluskey.h>

// write 16bit binary input to uint16_t

void scanf_minmap(uint16_t *buffer) {
    
	char buf[16];
    *buffer = 0;

	// clean input

	int i;
	while ((i = getchar()) != '\n' && i != EOF) { }
    
	// get input

	if (scanf("%16[01]", buf) == 1)
        for (i = 0; buf[i]; i++)
            *buffer = (*buffer << 1) | (buf[i] - '0');
    
	return;
}

// print single row

void print_row(char *rows, int8_t n) {
	
	printf("%hhu | ", n);
	
	for(int8_t i = 3; i > -1; i--)
		printf("%c ", (GET_POS(rows[n], i) > 1) ? '-' : '0' + GET_POS(rows[n], i));
	
	printf("| "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(rows[n]));

	return;
}

// print all rows (length: ROWS)

void print_rows(char *rows) {
	
	for (uint8_t i = 0; i < ROWS; i++)
		print_row(rows, i);
	
	return;
}

// set all rows to 0x11111111

void clean_rows(char *rows) {
	
	memset(rows, 255, sizeof(*rows) * ROWS);
	
	return;
}

// prepare row for calcs

void set_row(char *rows, int8_t n) {
	
	int8_t i = 0;
	int8_t j = n;
	
	// translate dec to bin and fill

	while (j != 0) {
		
		if (j&1) {
			SET_POS(rows[n], i);
		}
		else {
			UNSET_POS(rows[n], i);
		}
		
		j /= 2;
		i++;
	}

	// fill left space

	while (i < sizeof(*rows)*4) {

		UNSET_POS(rows[n], i);
		i++;
	}
	
	return;
}

// prepare all rows

void set_rows(char *rows) {
	
	for (uint8_t i = 0; i < ROWS; i++)
		set_row(rows, i);
	
	return;
}

// prepare rows indicated in minmap

void int2rows(char *rows, const uint16_t *buffer) {

	for (uint8_t i = 0; i < ROWS; i++)
		if (GET_BIT(*buffer, i))
			set_row(rows, i);

	return;
}

// Quine-McCluskey

void minimalise(char *rows, const uint16_t *buffer) {

	// main and tmp calc storages
	
	NumSet_t primary[LEN];
	NumSet_t tmp[LEN];

	// copy and clean arrays (0-ROWS)
	
	for (int8_t i = 0; i < ROWS; i++) {
		
		primary[i].num = rows[i];
		primary[i].refers[0] = i;
		memset(primary[i].refers+1, 255, sizeof(*(primary[i].refers)) * 7);
		
		tmp[i].num = (char)255;
		memset(tmp[i].refers, 255, sizeof(*(tmp[i].refers)) * 8);
	}

	// clean left space (ROWS-LEN)

	for (int8_t i = ROWS; i < LEN; i++) {
		
		primary[i].num = (char)255;
		primary[i].refers[0] = i;
		memset(primary[i].refers+1, 255, sizeof(*(primary[i].refers)) * 7);
		
		tmp[i].num = (char)255;
		memset(tmp[i].refers, 255, sizeof(*(tmp[i].refers)) * 8);
	}

	// iterator and calc checker

	uint8_t g = 0;
	uint32_t checked = 0;

	while (1) {
		
		// clean calc checker

		checked = 0;

		for (uint8_t i = LEN; i-->0;) {
			
			for (uint8_t j = i; j-->0;) {
				
				if ((char)primary[j].num != (char)255 && (char)primary[i].num != (char)255) {
					
					// counter and position

					uint8_t pos = 3, can_comb = 0;
					
					// count differences and last diff position

					for (g = 0; g < 4; g++) {

						if (GET_POS(primary[i].num, g) != GET_POS(primary[j].num, g)) {
							
							pos = g;
							can_comb++;
						}
					}

					// check if numbers can be combined

					if (can_comb == 1) {

						// store and modify new number

						char l_tmp = primary[i].num;
						LOCK_POS(l_tmp, pos);

						// get free position and check if dublicate
						
						for (g = 0; (char)tmp[g].num != (char)255 && tmp[g].num != l_tmp; g++) { }

						// if not dublicate

						if ((char)tmp[g].num == (char)255) {

							// copy number and all references

							tmp[g].num = l_tmp;
							memcpy(tmp[g].refers, primary[i].refers, sizeof(*(tmp[g].refers)) * 8);
							
							for (pos = 1; (char)tmp[g].refers[pos] != (char)255; pos++) { }

							for (uint8_t h = 0; (char)primary[j].refers[h] != (char)255; h++, pos++)
								tmp[g].refers[pos] = primary[j].refers[h];
						}

						// add numbers as used

						checked |= (1<<i) | (1<<j);
					}
				}
			}
		}


		// check if no chenges

		if ((char)tmp[0].num != (char)255) {

			// get free space position

			for (g = 0; (char)tmp[g].num != (char)255; g++) { }

			for (uint8_t i = 0; i < LEN; i++) {
				
				// fill free space with unused numbers

				if ((char)GET_BIT(checked,i) == (char)0 && (char)primary[i].num != (char)255) {
					
					tmp[g].num = primary[i].num;
					memcpy(tmp[g].refers, primary[i].refers, sizeof(*(tmp[g].refers)) * 8);
					g++;
				}
			}

			// copy from tmp to primary and clean tmp

			for (g = 0; g < LEN; g++) {
				
				primary[g].num = tmp[g].num;
				memcpy(primary[g].refers, tmp[g].refers, sizeof(*(primary[g].refers)) * 8);
				
				tmp[g].num = (char)255;
				memset(tmp[g].refers, 255, sizeof(*(tmp[g].refers)) * 8);
			}
		}
		else {

			// prepare tmp for references counting

			for (g = 0; g < ROWS; g++) {

				tmp[g].num = g;
				memset(tmp[g].refers, 0, sizeof(*(tmp[g].refers)) * 8);
			}

			// count every reference

			for (g = 0; g < ROWS; g++)
				if ((char)primary[g].num != (char)255)
					for (uint8_t i = 0; (char)primary[g].refers[i] != (char)255; i++)
						tmp[(uint8_t)primary[g].refers[i]].refers[0]++;

			// set all 0 to 255

			for (g = 0; g < ROWS; g++)
				if (!tmp[g].refers[0])
					tmp[g].refers[0] = (char)255;

			// insertation sort all references by their quantity 

			uint8_t i;
			int8_t j;
			struct {char num; char refer;} key;

			for (i = 1; i < ROWS; i++) {
				key.num = tmp[i].num;
				key.refer = tmp[i].refers[0];
				j = i - 1;
		 
				while (j >= 0 && (uint8_t)tmp[j].refers[0] > (uint8_t)key.refer) {
					tmp[j + 1].num = tmp[j].num;
					tmp[j + 1].refers[0] = tmp[j].refers[0];
					j = j - 1;
				}
				tmp[j + 1].num = key.num;
				tmp[j + 1].refers[0] = key.refer;
			}

			// clean initial array

			memset(rows, 255, sizeof(*rows) * ROWS);

			// prepare tmp buffer

			checked = 0;

			// find minimal solution out of exisiting numbers

			for (i = 0; checked != *buffer && (char)tmp[i].refers[0] != (char)255; i++) {
				
				// loop dropper by count

				uint8_t loop_drop = tmp[i].refers[0];

				// iterate all numbers for references

				for (j = 0; (char)primary[j].num != (char)255 && loop_drop; j++) {

					// checkers

					uint8_t is_included = 0;
					uint8_t filled = 0;
					
					for (g = 0; (char)primary[j].refers[g] != (char)255; g++) {

						if ((char)tmp[i].num == (char)primary[j].refers[g])
							is_included = 1;

						if (GET_BIT(checked, primary[j].refers[g]))
							filled++;
					}

					// if number has needed reference

					if (is_included) {
						
						// if number covers uncovered solution

						if (g-filled) {
							
							// get free space and store

							for (g = 0; (char)rows[g] != (char)255; g++) { }

							rows[g] = primary[j].num;

							// add references as bits to tmp buffer as covered solutions

							for (g = 0; (char)primary[j].refers[g] != (char)255; g++)
								SET_BIT(checked, primary[j].refers[g]);
						}

						loop_drop--;
					}
				}
			}

			// end infinite loop

			break;
		}
	}

	return;
}
