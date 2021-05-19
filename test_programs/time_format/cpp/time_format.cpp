#include <iostream>
#include <cmath>
#include <time.h>

using namespace std;

long random_time;

char* formatTime(long time);

int main(void){
	srand(time(NULL));
	cout << 0xFFFFFF << endl;
    while (1) {
        random_time = (rand() % 360000) * 1000;
        cout << random_time <<"\n" << formatTime(random_time) << "\n";
		cin.get();
    }
}

char* formatTime(long time) {
	char *out;
	long hh, mm, ss;
	time = floor(time/1000); // convert to seconds from milliseconds
	mm = time / 60;
	ss = time % 60;
	hh = mm / 60;	
	mm = mm % 60;
    printf("--%d:%d:%d--", hh, mm, ss);
	sprintf(out, "%d:%d:%d", hh, mm ,ss);
	return out;
}