//Zbi�r funkcji dzia�aj�cych na pami�ci zewn�trznej

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