# Proyek-Array-C-C2583207016

## Identitas
- **Nama Lengkap** : Yerdi Febriansyah Alfaruqi
- **NIM**          : C2583207016
- **Kelas**        : PTI3B
- **Mata Kuliah**  : Algoritma dan Pemrograman Lanjutan
- **Nama Proyek**  : Sistem Monitoring Stok dan Penjualan Kantin Kampus

## Deskripsi Singkat
Aplikasi console berbahasa C untuk membantu pengelola kantin kampus memantau stok, memproses transaksi penjualan, dan melihat laporan penjualan. Data produk disimpan dalam array selama program berjalan (tanpa database atau file eksternal), dengan kapasitas maksimal 50 produk dan 6 produk awal.

## Daftar Fitur
1. **Lihat Daftar Produk**: menampilkan nomor, nama, harga, dan stok semua produk.
2. **Tambah Produk**: menambah produk baru (nama tidak boleh kosong atau duplikat, harga dan stok tidak negatif, kapasitas maksimal 50).
3. **Ubah Produk**: mengubah harga dan stok produk yang sudah ada.
4. **Transaksi Penjualan**: membeli beberapa jenis produk dalam satu transaksi, menghitung subtotal dan total, menolak pembelian yang melebihi stok, lalu mengurangi stok dan menambah jumlah terjual setelah dikonfirmasi.
5. **Tambah Stok**: menambah stok produk (restock).
6. **Cari Produk**: mencari produk berdasarkan nama (tidak membedakan huruf besar/kecil, bisa sebagian kata).
7. **Urutkan Produk**: mengurutkan berdasarkan harga termurah atau jumlah terjual terbanyak.
8. **Laporan Penjualan**: jumlah terjual dan pendapatan per produk, total unit terjual, total pendapatan, dan produk terlaris.
9. **Cek Stok Menipis**: menampilkan produk dengan stok kurang dari 5.
0. **Keluar**.

Validasi input: pilihan menu tidak tersedia, input bukan angka, nomor produk tidak ditemukan, jumlah beli nol atau negatif, harga/stok negatif, jumlah beli melebihi stok, dan kapasitas penuh.

## Penjelasan Penggunaan Array
Array adalah struktur data utama program:

| Array | Tipe | Fungsi |
|---|---|---|
| `nama[50][30]` | array karakter 2 dimensi | menyimpan nama produk; baris = produk ke-i, kolom = karakter nama |
| `harga[50]` | array 1 dimensi `int` | harga tiap produk |
| `stok[50]` | array 1 dimensi `int` | stok tiap produk |
| `terjual[50]` | array 1 dimensi `int` | jumlah unit terjual tiap produk |
| `pendapatan[50]` | array 1 dimensi `long` | akumulasi pendapatan tiap produk |
| `jumlah` | `int` | jumlah produk aktif |

- Kelima array bersifat **paralel**: data satu produk berada pada indeks yang sama (misalnya `nama[2]`, `harga[2]`, `stok[2]`).
- Data aktif berada pada indeks `0` sampai `jumlah - 1`. Produk baru disimpan di indeks `jumlah`, lalu `jumlah` bertambah. Penambahan ditolak jika `jumlah` sudah mencapai 50.
- Pada transaksi, array `qtyBeli[]` menampung jumlah beli per produk sementara. Stok baru berubah setelah transaksi dikonfirmasi, sehingga transaksi yang dibatalkan tidak mengubah data.
- Pengurutan memakai array indeks `urut[]` yang diurutkan dengan bubble sort. Data asli tidak dipindahkan, sehingga kelima array tidak mungkin tertukar sebagian.

## Petunjuk Kompilasi dan Menjalankan
**Menggunakan GCC (terminal):**
```bash
gcc -Wall -Wextra -o kantin main.c
./kantin
```

**Menggunakan Code::Blocks:**
1. Buka file `main.c` (atau buka project).
2. Tekan **F9** (Build and Run).

## Pengujian

Pengujian dilakukan dengan 7 skenario wajib. Setiap skenario disertai input, hasil yang diharapkan, keterangan hubungan input-proses-output, dan tangkapan layar.

| No | Skenario | Menu yang dipakai | Hasil |
|----|----------|-------------------|-------|
| 1 | Tambah produk | 2, 1 | Berhasil |
| 2 | Transaksi | 4 | Berhasil |
| 3 | Pembaruan stok | 1, 4, 1, 8 | Berhasil |
| 4 | Stok tidak cukup | 1, 4, 1 | Berhasil |
| 5 | Laporan | 8 | Berhasil |
| 6 | Pengurutan | 7 | Berhasil |
| 7 | Pencarian dan stok < 5 | 6, 9 | Berhasil |

---

### Skenario 1: Tambah Produk
- **Input/Tindakan**: Menu 2, nama `Roti Bakar`, harga `7000`, stok awal `10`, lalu menu 1.
- **Hasil yang diharapkan**: Produk baru muncul dalam daftar.
- **Keterangan**: Program memvalidasi nama (tidak kosong, tidak duplikat), harga, dan stok (tidak negatif). Data disimpan pada indeks `jumlah`, lalu `jumlah` bertambah. Roti Bakar muncul sebagai produk nomor 7 dengan harga 7000 dan stok 10.

<img width="960" height="540" alt="tmbh" src="https://github.com/user-attachments/assets/7d0fa08a-c944-4907-9919-463afc439dc5" />

---

### Skenario 2: Transaksi
- **Input/Tindakan**: Menu 4, beli `Nasi Goreng` x2 dan `Mie Ayam` x3, ketik `0` untuk selesai.
- **Hasil yang diharapkan**: Subtotal dan total benar.
- **Keterangan**: Subtotal = harga x jumlah. Nasi Goreng: 2 x 12.000 = 24.000. Mie Ayam: 3 x 10.000 = 30.000. Total pembayaran = 54.000.

<img width="656" height="174" alt="sken2" src="https://github.com/user-attachments/assets/e0075308-0c9d-43cf-afd1-7d9eb5de8bad" />


---

### Skenario 3: Pembaruan Stok
- **Input/Tindakan**: Menyelesaikan transaksi pada skenario 2 dengan menjawab `y`, lalu menampilkan daftar produk dan laporan.
- **Hasil yang diharapkan**: Stok dan jumlah terjual berubah.
- **Keterangan**: Stok baru dikurangi setelah transaksi dikonfirmasi. Stok Nasi Goreng berubah dari 20 menjadi 18 dan terjualnya menjadi 2. Stok Mie Ayam berubah dari 15 menjadi 12 dan terjualnya menjadi 3.

-<img width="960" height="540" alt="trx1" src="https://github.com/user-attachments/assets/2c756616-22cd-4e17-8a9a-159ab1afc047" />
-<img width="960" height="540" alt="trx2" src="https://github.com/user-attachments/assets/ecd6b831-c3f9-414c-b9fd-e7300c0227db" />
-<img width="960" height="540" alt="trx3" src="https://github.com/user-attachments/assets/f05c2003-9124-4fd7-94d8-d82c18ba4457" />
-<img width="960" height="540" alt="trx4" src="https://github.com/user-attachments/assets/92628995-35dd-4a90-9b30-743bf9356fe4" />


---

### Skenario 4: Stok Tidak Cukup
- **Input/Tindakan**: Menu 4, beli `Air Mineral` (stok 4) sebanyak `10`. Setelah itu cek stok lewat menu 1.
- **Hasil yang diharapkan**: Transaksi ditolak dan stok tetap.
- **Keterangan**: Program membandingkan jumlah beli (termasuk yang sudah ada di keranjang) dengan stok. Karena 10 > 4, pembelian ditolak dengan pesan stok tidak cukup. Stok Air Mineral tetap 4.

-<img width="960" height="540" alt="sken4" src="https://github.com/user-attachments/assets/041b492d-0ecf-49a2-b4c6-2fe7bdde4476" />
-<img width="960" height="540" alt="ken4" src="https://github.com/user-attachments/assets/5e91dc43-4aa5-47d0-904b-def6c5624516" />

---

### Skenario 5: Laporan
- **Input/Tindakan**: Menu 8 setelah transaksi skenario 2.
- **Hasil yang diharapkan**: Unit, pendapatan, dan produk terlaris benar.
- **Keterangan**: Total unit terjual = 2 + 3 = 5. Total pendapatan = 24.000 + 30.000 = 54.000. Produk terlaris adalah Mie Ayam dengan 3 unit.

-<img width="960" height="540" alt="sken5" src="https://github.com/user-attachments/assets/e4f1ab0c-de2f-426c-a472-d7a920a19d8a" />

---

### Skenario 6: Pengurutan
- **Input/Tindakan**: Setelah transaksi tambahan (Gorengan x10, Es Teh Manis x4), menu 7 pilihan `1` (harga termurah), lalu menu 7 pilihan `2` (terjual terbanyak).
- **Hasil yang diharapkan**: Urutan sesuai ketentuan.
- **Keterangan**: Pilihan 1 menampilkan harga naik dari atas ke bawah (2.000, 3.000, 4.000, 7.000, 8.000, 10.000, 12.000). Pilihan 2 menampilkan jumlah terjual turun dari atas ke bawah (Gorengan 10, Es Teh Manis 4, Mie Ayam 3, Nasi Goreng 2, lalu produk yang belum terjual). Pengurutan memakai bubble sort pada array indeks `urut[]`, sehingga data asli tidak berubah.

-<img width="960" height="540" alt="sken6" src="https://github.com/user-attachments/assets/f283f4ae-c1c3-42f5-81fc-797c45207cd3" />
-<img width="960" height="540" alt="ken6" src="https://github.com/user-attachments/assets/e10960e4-54e6-4870-b992-3281e5ca035d" />

---

### Skenario 7: Pencarian dan Stok Menipis
- **Input/Tindakan**: Menu 6 dengan kata kunci `nasi`, menu 6 dengan kata kunci `pizza`, lalu menu 9.
- **Hasil yang diharapkan**: Hasil dan peringatan tepat.
- **Keterangan**: Pencarian tidak membedakan huruf besar/kecil dan mencocokkan sebagian kata, sehingga `nasi` menemukan Nasi Goreng. Kata `pizza` menghasilkan pesan tidak ditemukan. Menu 9 menampilkan produk dengan stok kurang dari 5, yaitu Air Mineral (stok 4).

-<img width="960" height="540" alt="sken7" src="https://github.com/user-attachments/assets/76871c53-dd96-4cf9-9c1a-af521de83198" />
-<img width="960" height="540" alt="ken7" src="https://github.com/user-attachments/assets/e8576e06-a911-45f8-a586-23bc6ae349cd" />
