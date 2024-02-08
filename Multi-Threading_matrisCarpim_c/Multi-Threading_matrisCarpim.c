// Nadire Nur SAGLAM

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SATIR 4  // Matris satir sayisi
#define MAX_SUTUN 4  // Matris sütun sayisi
#define MAX_THREAD MAX_SATIR * MAX_SUTUN //max_thread-> kullanilacak toplam thread sayisi

int matrisA[MAX_SATIR][MAX_SUTUN];
int matrisB[MAX_SATIR][MAX_SUTUN];
int carpimMatris[MAX_SATIR][MAX_SUTUN];
int tempSum[MAX_THREAD];  // Her bir thread icin ayri bir tempSum

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex tanimlanir.
//multithreading programlarda güvenli erisim icin kullanilir.


struct ThreadVeriler {
    int satir;
    int sutun;
}; //threadlerin satir ve sutunlarini tutar

void *matrisCarpim(void *arg) {
  //her thread in yaptigi carpma islemini yapar ve sonucu tempsun a yazar


    struct ThreadVeriler *veri = (struct ThreadVeriler *)arg;
    int satir = veri->satir; //threadin hangi satirda calistigi belirtilir
    int sutun = veri->sutun; //threadin hangi sutunda calistigi belirtilir

    int geciciCarpimTut = 0; //her thread'in kendi carpma islemi sonucunu gecici olarak saklar.

    for (int i = 0; i < MAX_SUTUN; i++) {
        geciciCarpimTut += matrisA[satir][i] * matrisB[i][sutun];
    }//Matris elemanlari carpildi


 // Kritik bolgeye (tempSum dizisine yazma islemi) girmek icin mutex kilitlenir
    pthread_mutex_lock(&mutex);

 // gecici carpim sonucunu tempSum dizisine yaz
    tempSum[satir * MAX_SUTUN + sutun] = geciciCarpimTut;

  // Kritik bolgeden cikmak icin mutex'i serbest birak
    pthread_mutex_unlock(&mutex); // Mutex'i serbest bırak


    pthread_exit(NULL); //threadi sonlandır
}

void dosyadanOkuYaz() {
    //dosyadan matrisler okunur ve yazdirilir

    //inputA yi oku

    FILE *fileA = fopen("inputA.txt", "r"); //dosya okuma modunda acildi
    if (fileA == NULL) {
        perror("Dosya acma hatasi");
        return;
    }

    for (int i = 0; i < MAX_SATIR; i++) { //satir sayisi kadar doner
        for (int j = 0; j < MAX_SUTUN; j++) { //sutun sayisi kadar doner
            if (fscanf(fileA, "%d", &matrisA[i][j]) != 1) { //dosyadan bir tam sayi okur
            //sayi okunamazsa !=1 kontrolu ile kontrol edilir ve mesaj verilir
                fprintf(stderr, "Dosya okuma hatasi\n");
                fclose(fileA);
                return;
            }
        }
    }

    fclose(fileA);

    //okunan matrisA yi ekrana yazdir
    printf("Matris A: \n");
    for (int i = 0; i < MAX_SATIR; i++) {
        for (int j = 0; j < MAX_SUTUN; j++) { //satir sutun dolasir
          printf("%d ", matrisA[i][j]); //matrisin her elemani yazdirilir
        }
        printf("\n"); //satir satir olsun diye
    }
    //-------------------------------------------------------
    //inputB yi oku
    //matrisA icin yapilan islemler matrisB icinde yapilir

    FILE *fileB = fopen("inputB.txt", "r");
    if (fileB == NULL) {
        perror("Dosya acma hatasi");
        return;
    }

    for (int i = 0; i < MAX_SATIR; i++) {
        for (int j = 0; j < MAX_SUTUN; j++) {
            if (fscanf(fileB, "%d", &matrisB[i][j]) != 1) {
                fprintf(stderr, "Dosya okuma hatasi\n");
                fclose(fileB);
                return;
            }
        }
    }
    fclose(fileB);

    //okunan matrisB yi ekrana yazdir
    printf("\nMatris B: \n");
    for (int i = 0; i < MAX_SATIR; i++) {
        for (int j = 0; j < MAX_SUTUN; j++) {
            printf("%d ", matrisB[i][j]);
        }
        printf("\n");
    }


}

void threadOlusturBeklet(pthread_t *threads, struct ThreadVeriler *threadVeri) {
    // carpim islemleri icin threadleri olusturur ve tamamlanmasini bekler parametreleri sunlardir:
    //olusrurulan threadlerin id lerini tutan bir dizi(threads) ve her threadin calisma baglamini tasiyan bir dizi(threadVeri)

    for (int i = 0; i < MAX_SATIR; i++) {
        for (int j = 0; j < MAX_SUTUN; j++) { //satir sutun gezer
            threadVeri[i * MAX_SUTUN + j].satir = i;
            //her thread icin Threadveriler struct inin satir elemanina distaki dongunun indisini atar

            threadVeri[i * MAX_SUTUN + j].sutun = j;
            //her thread icin Threadveriler struct inin satir elemanina icteki dongunun indisini atar

            pthread_create(&threads[i * MAX_SUTUN + j], NULL, matrisCarpim, (void *)&threadVeri[i * MAX_SUTUN + j]);
            //her bir thread olusturulur
            //calisma fonksiyonu olarak matrisCarpim fonksiyonu kullanilir
            //matrisCarpim kismindan sonraki kisim, matrisCarpim fonk. a gecirilcek argumandir
        }
    }

    // Thread'lerin tamamlanmasını bekle
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_join(threads[i], NULL); //null->thread in geri donus degeri ile ilgili bilgi alinamaz
    }
}

int main() {
    dosyadanOkuYaz(); //dosya okuma ve matris yazdirma yaptirilir

    pthread_t threads[MAX_THREAD]; //carpim icin kullanilacak threadleri tanimlar
    //pthread_t tipinde MAX_THREAD uzunlugunda bir dizi olusturur

    struct ThreadVeriler threadVeri[MAX_THREAD]; //ThreadVeriler structindan MAX_THREAD uzunlugunda dizi olusturulur

    threadOlusturBeklet(threads, threadVeri);

    // carpim soonucunda olusan gecisi tempsun dizisinden sonuc matrisi olusturulur
    for (int i = 0; i < MAX_SATIR; i++) {
        for (int j = 0; j < MAX_SUTUN; j++) {
            carpimMatris[i][j] = tempSum[i * MAX_SUTUN + j]; //her elemani tempsun dan alip sonuc matrisine atar
        }
    }

    // Sonuç matrisini ekrana yazdır
    printf("\nCarpim Matrisi: \n");
    for (int i = 0; i < MAX_SATIR; i++) {
        for (int j = 0; j < MAX_SUTUN; j++) {
            printf("%d ", carpimMatris[i][j]); //her eleman gezilir ve yazdirilir
        }
        printf("\n"); //satir satir yazdirabilmek icin
    }

    return 0;
}
