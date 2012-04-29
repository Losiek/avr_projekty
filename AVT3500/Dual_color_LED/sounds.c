
//Funkcje
////////////////////////////////////////////

void play_C1(char a) {
	uint16_t dl_taktu = takt/a;
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-C1) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(C1);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(C1);
		}
	}

void play_D1(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-D1) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(D1);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(D1);
		}
	}

void play_E1(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-E1) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(E1);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(E1);
		}
	}

void play_F1(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-F1){
		PORTPiez |= 1<<Piezo;
		_delay_ms(F1);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(F1);
		}
	}

void play_G1(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-G1){
		PORTPiez |= 1<<Piezo;
		_delay_ms(G1);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(G1);
		}
	}

void play_A1(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-A1) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(A1);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(A1);
		}
	}

void play_H1(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-H1) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(H1);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(H1);
		}
	}
//Druga Oktawa
//////////////////////////////////////////////
void play_C2(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-(C1/2)) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(C1/2);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(C1/2);
		}
	}

void play_D2(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-(D1/2)) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(D1/2);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(D1/2);
		}
	}

void play_E2(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-(E1/2)) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(E1/2);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(E1/2);
		}
	}

void play_pause(char a) {
	uint16_t dl_taktu = takt/a;
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-C1) {
		_delay_ms(C1);
		_delay_ms(C1);
		}
	}

void play_Fis(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-(H1/2)) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(1.35);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(1.35);
		}
	}
	
void play_H0(char a) {
	uint16_t dl_taktu = takt/a; 
	for (dl_taktu; dl_taktu>0; dl_taktu = dl_taktu-H1) {
		PORTPiez |= 1<<Piezo;
		_delay_ms(H1*2);
		PORTPiez &= ~(1<<Piezo);
		_delay_ms(H1*2);
		}
	}