#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "fields.h"

#define MAX_LINE_LENGTH 50000

static char yazilacak[MAX_LINE_LENGTH];
int cursor = 0;

//ekleme islemi
void arayaEkle(char harf) {
    int diziUzunlugu = strlen(yazilacak);
    char *yeniDizi = (char *)malloc((diziUzunlugu + 2) * sizeof(char));

    // Ilk kismi kopyalama
    strncpy(yeniDizi, yazilacak, cursor);
    
    // Eklenecek harfi yerlestirme
    yeniDizi[cursor] = harf;
    yeniDizi[cursor + 1] = '\0'; // Yeni dizi sonunu belirleme

    // Geri kalanlari kopyalama
    strcat(yeniDizi, yazilacak + cursor);

    // Yeni diziyi orijinal diziye kopyalama
    strcpy(yazilacak, yeniDizi);

    // Bellegi serbest birak
    free(yeniDizi);
}

//arayaEkle'yi kullanarak ekleme islemi 
void yaz(char *line) {
    char slash = 92;
    int adet;
    char *word = strtok(line, ":");
    int i = 0;
    while ((word = strtok(NULL, " ")) != NULL) {
        if (i % 2 == 0) {
            adet = atoi(word);
        }

        if (i % 2 == 1) {
            for (int j = 0; j < adet; j++) {
                if (word[0] == slash && word[1] == 'n') {
                    arayaEkle('\n');
                } else if (word[0] == slash && word[1] == 'b') {
			arayaEkle(' ');
                } else {

			arayaEkle(*word);
                }
                cursor ++;
            }
        }
        i++;
    }
}

//sil fonksiyonu
void sil(char *line){
    int length = strlen(yazilacak);
    int deleted = 0;
        
    char *word = strtok(line, ":");
    int a = 0;
    int adet;
    while ((word = strtok(NULL, " ")) != NULL) {
        if (a % 2 == 0) {
            adet = atoi(word);
        }
        if (a % 2 == 1) {
        
	    for (int i = length - 1; i >= 0; i--) {
		if (yazilacak[i] == *word && deleted < adet ) {
		//
		    //silinen karakterin uzerine yazmak icin karakterleri sola kaydırma
		    for (int j = i; j < length - 1; j++) {
		        yazilacak[j] = yazilacak[j + 1];
		    }
		    deleted ++;
		    length--; 
		    //bir karakter kaldirinca uzunlugu bir azalt
		    cursor = i ;
		}
	    }
	    yazilacak[length] = '\0';
        }
        a++;
    }
    

}
//yazilan satirin sonuna gider
void sonagit() {
	cursor = strlen(yazilacak);
}


//dosyaya yazma
void dur(char *kelime,char *argv) {
    int fd, sz;
    
    if(argv != NULL){
	    	fd = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	    if (fd < 0) {
		perror(argv);
		exit(1);
   	 	}

    }   
    sz = write(fd, kelime, strlen(kelime));
    
}

//diger fonksiyonlari parametrelere gore cagiran fonksiyon
void bak(char *word, IS is, const char *filename, char *argv) {
    if (strcmp(word, "yaz:") == 0) {
        yaz(is->text1);
    }
    if (strcmp(word, "sil:") == 0) {
        sil(is->text1);
    }
    if (strcmp(word, "sonagit:") == 0) {
        sonagit();
    }
    if (strcmp(word, "dur:") == 0) {
        dur(yazilacak,argv);
    }
}


int main(int argc, char *argv[]) {
	//dosya isimlerinin eksik girilip girilmedigi kontrolu
    if (argc != 3) {
    	if(argc == 2){
    		fprintf(stderr, "Cikis dosya girisi eksik! \n");
        	return 1;
    	}
    	else{
    		fprintf(stderr, "Eksik dosya girisi! \n");
        	return 1;
    	}
    }
	
	
	char * format = strrchr(argv[1],'.');
	
	//dosya uzantisi kontrolu
    if(format == NULL){
    	printf("Giris dosyanin uzantisi yok , .dat ya da .txt olmalidir.\n");
    	return 1;
    }
    
    //dosya uzantisi kontrolu
    if( strcmp(format, ".dat") != 0 && strcmp(format, ".txt") != 0){
    	printf("Hatali giris dosya formati .dat ya da .txt olmaidir. \n");
    	return 1;
    }
    else{
    	const char *filename = argv[1];
    
    IS is;
    is = new_inputstruct(filename);

    if (is == NULL) {
        fprintf(stderr, "Error: Cannot open input file.\n");
        return 1;
    }

    while (get_line(is) >= 0) {
        for (int i = 0; i < is->NF; i++) {
           bak(is->fields[i], is, filename,argv[2]);

        }
    }

    jettison_inputstruct(is);
    
    
    printf("%s :\n%s\n",argv[2],yazilacak);
    return 0;
    }
    
}
