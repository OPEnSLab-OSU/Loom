#include "Loom_Misc.h"


/////////////////////////////////////////////////////////////////////
void print_array(String data [], int len, int format)
{
	#if LOOM_DEBUG == 1
		for (int i = 0; i < len; i++) {
			if (format == 1) { Serial.println(data[i].c_str()); }
			if (format  > 1) { Serial.print(data[i].c_str()); Serial.print(" "); }
			if ((format > 2) && (i%5==0)) { Serial.println(); }
		}
		Serial.println();
	#endif
}

/////////////////////////////////////////////////////////////////////
void str_replace(char* target, const char* needle, const char* replacement)
{
	char buffer[384] = { 0 };
	char *insert_point = &buffer[0];
	const char *tmp = target;
	size_t needle_len = strlen(needle);
	size_t repl_len = strlen(replacement);

	while (1) {
		const char *p = strstr(tmp, needle);
		
		if (p == NULL) { 
			strcpy(insert_point, tmp); 
			break; 
		}

		// copy part before needle
		memcpy(insert_point, tmp, p - tmp);
		insert_point += p - tmp;

		// copy replacement string
		memcpy(insert_point, replacement, repl_len);
		insert_point += repl_len;

		// adjust pointers, move on
		tmp = p + needle_len;
	}

    // write altered string back to target
	strcpy(target, buffer);
}

/////////////////////////////////////////////////////////////////////
const char* nth_strchr(const char* s, char c, int n)
{
	int c_count;
	char* nth_ptr;

	for (c_count=1,nth_ptr=strchr(s,c); nth_ptr != NULL && c_count < n && c!=0; c_count++) { 
		nth_ptr = strchr(nth_ptr+1, c); 
	}

	return nth_ptr;
}

/////////////////////////////////////////////////////////////////////

void replace_char(char* str, const char orig, const char rep) 
{
	char *ix = str;
	while((ix = strchr(ix, orig)) != NULL) {
		*ix++ = rep;
	}
}



bool functionRoute(const char* name) { return false; }
