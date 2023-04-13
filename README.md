# LVGL dan FreeRTOS monitoring cuaca dan jadwan sholat

## Deskripsi
GUI dengan `LVGL` dan `FreeRTOS` untuk Monitoring _Cuaca_ dari API `https://openweathermap.org/`
dan Mengambil data dari API `https://api.myquran.com` untuk keperluan *Jam jadwal sholat*.

## Fitur
Berikut adalah fitur yang sudah selesai dan belum selesai:
  1. Sinkronisasi dengan `pool.ntp.org` dengan GMT+7 formated 12Jam untuk Jam waktu dan Tanggal - SELESAI.
  2. Mengatur dan Mengubah SSID dan PASSWORD WiFi - SELESAI.
  3. Mengatur Koordinat dari monitoring Cuaca - BELUM SELESAI.
  4. Mengatur Kota dari Jam jadwal sholat - BELUM SELESAI.

## Untuk mengubah pin dan type dari LCD TFT
Berikut intruksi untuk mengubah pin dan type dari layar tft:
  1. Di dalam file `tft.ino` di baris **73** terdapat `class` 
   ```cpp 
   class LGFX : public lgfx::LGFX_Device
   ```
  unutk mengubah dengan jenis panel anda hanya perlu mengubahnya di baris **75** contoh:
  ```cpp
  lgfx::Panel_ST7796 _panel_instance;
  ```
  dan untuk menubah pin nya di setiap constructor terdapat class `cs miso mosi ck dc` untuk setiap panel dari touch screen lcd dan backlight berada dalam class tersebut.
  
## Cara meng-install PlatformIO di Visual Studio Code

Untuk menggunakan project ini anda harus menggunakan PlatformIO. PlatformIO adalah platform pengembangan perangkat keras (hardware) dan perangkat lunak (software) sumber terbuka yang menyediakan lingkungan pengembangan terpadu (IDE) untuk memprogram mikrokontroler, mikroprosesor, dan sistem terbenam (embedded systems) dengan dukungan untuk berbagai papan pengembangan, bahasa pemrograman, dan integrasi dengan alat pengembangan lainnya.
berikut langkah-langkah untuk menggunakan PlatformIO
1. Buka VS Code lalu masuk ke halaman Extensions pada bilah tab sebelah kiri. Atau anda juga dapat menggunakan shortcut `Ctrl+Shift+X`, cari di bilah pencarian dengan keyword `PlatformIO` lalu install.
![Instal Extensions PlatformIO pada VS Code kalian](media/Screenshot%20(16).png)
2. Setelah penginstalan selesai nanti bakalan otomatis masuk ke halaman Home `PlatformIO`, click `Open Project`.
   ![PlatformIO](media/Screenshot%20(17).png)
3. Setelah membuka project maka tampilan nya berubah seperti ini.
   ![Opened Project](media/Screenshot%20(19).png)
4. Untuk mengetahui program utama anda hanya perlu pergi ke folde `src/main.cpp`.
   ![main.cpp](media/Screenshot%20(18).png)