#include <iostream>
#include <cmath>

using namespace std;

long random_time;

int main(void){
    while (1) {
        random_time = rand() % 10000;
        cout << formatTime(random_time) << "\n";
    }
}

char* formatTime(long time) {
	char out[20] ;
	long hh, mm, ss;
	time = floor(time/1000); // convert to seconds from milliseconds
	mm = time / 60;
	ss = time % 60;
	hh = mm / 60;	
	mm = mm % 60;
    printf("\n\n--%d:%d:%d--\n\n", hh, mm, ss);
	sprintf(out, "%d:%d:%d", hh, mm ,ss);
	return out;
}