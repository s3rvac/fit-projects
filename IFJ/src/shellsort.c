/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		shellsort.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Implementace radici funkce - Shell sort
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "strptr_stack.h"
#include "error.h"
#include "shellsort.h"

/* Oddelovac jednotlivych slov */
static const char DELIMITER = '$';

/*
 * Vrati celkovy pocet slov v retezci 'str',
 * ktere jsou oddelene pomoci 'delimiter'
 */
static int get_strings_count(const char *str, char delimiter)
{
	int total_count = 0;
	char last_char = '\0';

	while (*str) {
		if (*str == delimiter) {
			// Kontrola, zda se nejdena jen o escape sekvenci \$
			// (nejedna se o konec retezce)
			if (last_char != '\\') {
				total_count++;
			}
		}

		last_char = *str;
		str++;
	}

	// Musime zvysit citac protoze posledni slovo neni ukonceno oddelovacem
	if (total_count > 0)
		total_count++;

	return total_count;
}

/*
 * Zameni vsechny znaky 'input_char' z retezce 'str' za znaky 'output_char'
 * a vrati pocet zamen. Vynechava \$ (escape sekvence)
 */
static int replace_chars(char *str, char input_char, char output_char)
{
	int replaces_count = 0;
	char last_char = '\0';

	while (*str) {
		if (*str == input_char) {
			// Kontrola, zda se nejdena jen o escape sekvenci \$
			// (ta se nenahrazuje)
			if (last_char != '\\') {
				*str = output_char;
				replaces_count++;
			}
		}

		last_char = *str;
		str++;
	}

	return replaces_count;
}

/*
 * Ulozi pocatecni adresy slov z retezce 'str' (pouze 'word_count' slov) do pole 'str_array'.
 * Jednotliva slova jsou oddelena znakem '\0' (proto je nutne zadat pocet slov)
 */
static void save_starting_addresses(const char *str, int word_count, char **str_array)
{
	assert (str != NULL && str_array != NULL);

	for (int i = 0; i < word_count; i++) {
		*str_array++ = (char *) str;

		// Presun se na konec slova a preskoc znak '\0'
		while (*str)
			str++;
		str++;
	}
}

/*
 * Vlastni radici funkce - Shellsort, ktera seradi pole retezcu.
 * str_array - pole ukazatelu na retezce (bude nam stacit presouvat ukazatele
 *             a ne kopirovat cele retezce)
 * l - nejlevejsi index
 * r - nejpravejsi index
 *
 * Zvolena velikost kroku byla prevzata z knizky Algorithms in C, Parts 1-4
 * od R. Sedgewicka
 */
static void shell_sort(char **str_array, int l, int r)
{
	assert (str_array != NULL);

	int k; // Aktualni delka (velikost) kroku

	// Nastaveni velikosti kroku
	const int MAX_STEP_LENGTH = (r - 1) / 9;
	for (k = 1; k <= MAX_STEP_LENGTH; k = 3 * k + 1)
		;

	while (k > 0) {
		for (int i = l + k; i <= r; i++) {
			int j = i;
			char *tmp = str_array[i]; // Vkladany retezec

			// Insertion sort
			while (j >= l + k && (strcmp(tmp, str_array[j - k]) < 0)) {
				str_array[j] = str_array[j - k];
				j -=k;
			}

			str_array[j] = tmp;
		}

		// Zmenseni kroku
		k /= 3;
	}
}

/*
 * Razeni metodou Shell sort.
 * Jako parametr prijima retezec obsahujici jednotliva slova ve formatu:
 * slovo1$slovo2$slovo3...slovoN (retezec je samozrejme ukoncen '\0')
 */
char *sort(const char *str)
{
	// Osetreni proti "neinicializovanemu" retezci - vraci ""
	if (str == NULL) {
		char *result = strmalloc(sizeof(char));
		result[0] = '\0';
		return result;
	}

	// Pokud retezec nema zadne znaky, neni treba nic provadet a vraci ""
	size_t str_length = strlen(str);
	if (str_length == 0) {
		char *result = strmalloc(sizeof(char));
		result[0] = '\0';
		return result;
	}

	// Vytvor docasny retezec (a prekopiruj do nej originalni retezec)
	// a konecny retezec, do ktereho bude vlozen vysledek
	// Pamet pro vysledny retezec se bude alokovat pomoci strmalloc
	// a ukladat na specialni zasobnik a bude pozdeji uvolenena
	char *tmp_str = calloc(1, str_length * sizeof(char) + 1);
	char *result_str = strmalloc(str_length * sizeof(char) + 1);
	if (!tmp_str || !result_str) {
		error(RAW, "Sort: Nedostatek pameti.\n");
		return NULL;
	}
	strncpy(tmp_str, str, str_length);

	int string_count = get_strings_count(str, DELIMITER);

	// Vytvor docasne pole pro uchovani pocatecnich adres slov
	// ulozenych v puvodnim retezci
	char **str_array = malloc(string_count * sizeof(char *));
	if (!str_array) {
		free(tmp_str);
		error(RAW, "Sort: Nedostatek pameti.\n");
		return NULL;
	}

	replace_chars(tmp_str, DELIMITER, '\0');

	save_starting_addresses(tmp_str, string_count, str_array);

	// Vlastni razeni metodou shell sort
	shell_sort(str_array, 0, string_count - 1);

	// Zkopiruj serazene retezce do vysledneho retezce a oddel je oddelovacem
	char *tmp_ptr = result_str;
	for (int i = 0; i < string_count; i++) {
		strcpy(tmp_ptr, str_array[i]);
		tmp_ptr += strlen(str_array[i]);
		if (i < (string_count - 1))
			*tmp_ptr = DELIMITER;
 		tmp_ptr++;
	}

	free(tmp_str);
	free(str_array);

	return result_str;
}

/* Konec souboru shellsort.c */
