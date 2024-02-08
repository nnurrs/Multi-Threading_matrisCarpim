# 4x4 Matris Çarpımı  

Bu projede, C programlama dili kullanılarak Multi-Threading programlama ile 4x4 boyutlarında olan iki matrisin çarpımı (Matrix Multiplication) gerçekleştirilmiştir.  
- A ve B matrisleri,verilen dosyalardan okunmuştur. Çarpım işlemlerinin sonucu ise geçici bir shared variable (paylaşılan değişken/*ortak alan) üzerinden oluşturulan 2 boyutlu bir diziye yazılmıştır.  
Çarpım işlemleri için her bir satır ve sütün çarpımı için bir thread oluşturulmuştur. Yani sonuç matrisinin her bir elemanını hesaplayan bir thread vardır.  

- Her bir thread çarpım sonuçlarının toplamını adına “tempSum” denilen ortak bir değişkene yapmaktadır. Yani bir satır ve sütunu çarpıp daha sonra bunların toplamını buraya yazmaktadır. Bu değişken global yapılmıştır, böylece x adet iş parçacıkları onu eşzamanlı (concurrently) olarak ve birbirini dışlayan bir şekilde (mutual exclusive/race condition) güncellemektedir.  

- Daha sonra buradaki sonuç 2 boyutlu global bir bir dizinin ilgili elemanına (indeksine/ hücresine/satır ve sütununa) yazılmıştır.  

- Multi-threading işlemleri için uygun senkronizasyon mekanizmaları (lock, mutex) kullanılmıştır.

- Son olarak global 2 boyutlu dizide tutulan sonuç matrisi ekrana satır ve sütün yapısına uygun olarak (bir matris formatında) bastırılmıştır.  

![image](https://github.com/nnurrs/Multi-Threading_matrisCarpim/assets/96475696/da5be8f1-bb24-4321-ad32-4765eff0fac9)

