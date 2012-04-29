//Zbiór funkcji dzia³aj¹cych na pamiêci zewnêtrznej

void strcopy(char *d, char *s) {
	while(*d++ = *s++);
}

int strleng(unsigned char *s) {
	int n=0;
	while(*s++) {
		n++;
	}
	return ++n;
}