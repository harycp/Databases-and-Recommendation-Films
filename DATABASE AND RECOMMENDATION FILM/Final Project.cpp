#include <iostream>
#include <fstream> // Untuk membaca dan menulis file eksternal
#include <string> //  Untuk menyimpan dan memanipulasi judul film, genre, dan baris-baris teks lainnya.
#include <ctime> // Mengambil real time waktu untuk data pengunjung
#include <set> // untuk mengelola himpunan pada tampilkan film
#include <algorithm> //  Untuk mengurutkan array film berdasarkan judul dalam mode ascending.
#include <map> // Untuk memanage data yang duplikat

using namespace std;

// Sebuah struct untuk menginisialisasikan data data Film
struct Film {
    string judul;
    string genre;
    int tahun;
    int durasi;
    double rating;
};

bool isValidGenre(const string& genre); 
bool isValidYear(int year); 
bool isValidDuration(int duration); 
bool isValidRating(double rating); 
void tambahkanFilm(); 
void tampilkanFilm();
void hapusFilmJudul(const string& judul, const string& genre); 
void tampilkanGuest(); 
void sortFilmByRating(Film films[], int low, int high); // Quick Sort
int binarySearchByTitle(Film films[], int low, int high, string judul);  // Binary
void cariFilmJudul();
void cariFilmTahun();
void cariFilmAbjad(); 
void mencariFilm();
void rekomendasikanFilm();
void adminMenu(); 
void guestMenu();

// Fungsi utama untuk mengatur alur sistem, terdapat pilihan login yang menggunakan do while dan switch statement(adminMenu(), guestMenu())
int main() {
    int loginChoice;

    do{
        cout << "==============================" << endl;
        cout << "SELAMAT DATANG DI FILMRATEQU" << endl;
        cout << "==============================\n" << endl;
        cout << "Login Sebagai:" << endl;
        cout << "1. Admin" << endl;
        cout << "2. Guest" << endl;
        cout << "0. Keluar\n" << endl;
        cout << "------------------------------\n" << endl;
        cout << "Masukkan pilihan: ";
        cin >> loginChoice;

        switch (loginChoice){
            case 1:
                adminMenu();
                break;
            case 2:
                guestMenu();
                break;
            case 0:
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }

    }while(loginChoice != 0);

    return 0;
}

// Mengecek validasi genre bila yang dimasukkan bukan string maka akan input ulang
bool isValidGenre(const string& genre) {
    for (char c : genre) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

// Mengecek valid year yaitu bila yang dimasukkan bukan rentang antara tahun 1800 - 2024 maka akan input ulang
bool isValidYear(int year) {
    return (year >= 1800 && year <= 2024);
}

// Mengcek validasi duration, bila duration yang dimasukkan kurang dari nol maka akan input ulang
bool isValidDuration(int duration) {
    return (duration > 0);
}

// Validasi rating yaitu bila rating yang dimasukkan bukan rentang antara 0 - 100 maka akan input ulang
bool isValidRating(double rating) {
    return (rating >= 0 && rating <= 10);
}

// Membuat objek file output(ios::app), menginput data film, menulis data ke file dan menutup file
void tambahkanFilm() {
    ofstream file("data_film.txt", ios::app);
    Film film;

    cout << "Judul: ";
    getline(cin >> ws, film.judul);

    do {
        cout << "Genre: ";
        getline(cin >> ws, film.genre);
        if(!isValidGenre(film.genre)){
            cout << "Input Invalid!" << endl;
        }

    } while (!isValidGenre(film.genre));


    do {
        cout << "Tahun: ";
        cin >> film.tahun;
        if(!isValidYear(film.tahun)){
            cout << "Input Invalid! (1800 - 2024)" << endl;
        }
    } while (!isValidYear(film.tahun));

    do {
        cout << "Durasi: ";
        cin >> film.durasi;
        if(!isValidDuration(film.durasi)){
            cout << "Input Invalid! (Lebih dari 0)" << endl;
        }
    } while (!isValidDuration(film.durasi));

    do {
        cout << "Rating (0-10): ";
        cin >> film.rating;
        if(!isValidRating(film.rating)){
            cout << "Input Invalid!" << endl;
        }
    } while (!isValidRating(film.rating));

    file << "Judul: " << film.judul << endl;
    file << "Genre: " << film.genre << endl;
    file << "Tahun: " << film.tahun << endl;
    file << "Durasi: " << film.durasi << endl;
    file << "Rating: " << film.rating << endl;
    file << "---------------------------------------" << endl;

    file.close();
    cout << "Film berhasil ditambahkan!" << endl;
}

// Membuka file(ifstream), membaca data film(loop while), menutup file(close.file())
// Menampilkan menu - menu(Switch Statement), Mengecek kegagalan
// objek ifstream dan ofstream adalah pointer. Alokasi memori dinamis menggunakan operator new dan membersihkan memori menggunakan operator delete.
void tampilkanFilm() {
    ifstream* file = new ifstream("data_film.txt");
    string line;

    if (file->is_open()) {
        int totalFilm = 0;
        set<string> genres;
        map<string, int> judulGenreFilmMap;

        while (getline(*file, line)) {
            if (line.find("Judul: ") != string::npos) {
                totalFilm++;
                string judul = line.substr(7);
                string genre;
                while (getline(*file, line) && line.find("Genre: ") == string::npos) {}
                if (line.find("Genre: ") != string::npos) {
                    genre = line.substr(7);
                    judulGenreFilmMap[judul + "_" + genre]++;
                    genres.insert(genre);
                }
            }
        }

        file->close();
        delete file;

        cout << "Total Film: " << totalFilm << endl;

        int choice;
        cout << "1. Tampilkan Genre" << endl;
        cout << "2. Tampilkan Semua Film" << endl;
        cout << "3. Cek Duplikat Judul Film" << endl;
        cout << "Masukkan pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nTerdapat " << genres.size() << " genre:\n" << endl;
                for (const string& genre : genres) {
                    int genreCount = 0;
                    ifstream* file = new ifstream("data_film.txt");
                    while (getline(*file, line)) {
                        if (line.find("Genre: " + genre) != string::npos) {
                            genreCount++;
                        }
                    }
                    file->close();
                    delete file;
                    cout << genre << ": " << genreCount << " film" << endl;
                }
                break;
            case 2:
                file = new ifstream("data_film.txt");
                while (getline(*file, line)) {
                    cout << line << endl;
                }
                file->close();
                delete file;
                break;
            case 3: {
                cout << "\nData film dengan judul dan genre yang duplikat:\n" << endl;
                bool foundDuplikat = false;
                for (const auto& pair : judulGenreFilmMap) {
                    if (pair.second > 1) {
                        cout << pair.first << ": " << pair.second << " kali" << endl;
                        foundDuplikat = true;
                    }
                }
                if (!foundDuplikat) {
                    cout << "Tidak ada judul film yang duplikat dalam database." << endl;
                }
                break;
            }
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } else {
        cout << "Tidak dapat membuka file data_film.txt" << endl;
    }
}

// Pengolahan file pada('data_film.txt'{input} dan 'temp.txt'{output}
// Mengecek apakah file yang dicari ada, Iterasi melalui Data Film memeriksa setiap baris untuk menentukan apakah itu adalah baris judul film atau bukan.
// Mencari Film yang akan dihapus, mengabaikan baris line pembatas.
// Menyalin data film ke file 'temp.txt' yang nantinya data_film akan dihapus dan data 'temp.txt' akan dijadikan data_film yang baru
// objek ifstream dan ofstream adalah pointer. operator new untuk membuat objek file dan delete untuk membersihkan memori setelah selesai digunakan.
void hapusFilm(const string& judul, const string& genre) {
    ifstream* inputFile = new ifstream("data_film.txt");
    ofstream* outputFile = new ofstream("temp.txt");

    if (!(*inputFile) || !(*outputFile)) {
        cout << "Terjadi kesalahan dalam membuka file." << endl;
        delete inputFile;
        delete outputFile;
        return;
    }

    bool filmDitemukan = false;
    string line;

    while (getline(*inputFile, line)) {
        if (line.find("Judul: ") != string::npos) {
            string filmJudul = line.substr(7);

            if (filmJudul == judul) {
                string lineGenre;
                while (getline(*inputFile, lineGenre) && lineGenre.find("Genre: ") == string::npos) {}

                if (lineGenre.find("Genre: " + genre) != string::npos) {
                    filmDitemukan = true;

                    while (getline(*inputFile, line) && line != "---------------------------------------") {}

                    if (inputFile->eof())
                        break;

                    getline(*inputFile, line);
                } else {
                    *outputFile << line << endl;
                    *outputFile << lineGenre << endl;
                    continue;
                }
            }
        }

        *outputFile << line << endl;
    }

    inputFile->close();
    outputFile->close();
    delete inputFile;
    delete outputFile;

    if (filmDitemukan) {
        if (remove("data_film.txt") != 0) {
            cout << "Terjadi kesalahan dalam menghapus film." << endl;
            return;
        }

        if (rename("temp.txt", "data_film.txt") != 0) {
            cout << "Terjadi kesalahan dalam mengubah nama file." << endl;
            return;
        }

        cout << "Film berhasil dihapus." << endl;
    } else {
        cout << "Film dengan judul dan genre tersebut tidak ditemukan." << endl;
    }
}

// Membuka file (guest_Data.txt dengan ifstream), membaca dan menampilkan data (getline())
// Menutup file (close())
void tampilkanGuest() {
    ifstream file("guest_data.txt");
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Tidak dapat membuka file guest_data.txt" << endl;
    }
}

// Menggunakan Sorting(Quick Short)(array low dan high)
// dipilih sebagai elemen terakhir array films, yaitu films[high]. Dalam kasus ini, pivot dipilih berdasarkan rating film.
// Melakukan partisi untuk  melakukan partisi array dengan membandingkan setiap elemen dari low hingga high-1 dengan pivot.
// Melakukan Swap untuk menukar posisi elemen tersebut dengan elemen pada indeks i+1, di mana i adalah indeks saat ini yang menunjukkan posisi terakhir elemen dengan rating film yang lebih kecil.
// Pivot PositionSetelah melakukan partisi, elemen pivot dipindahkan ke posisi yang benar, yaitu setelah elemen-elemen dengan rating film yang lebih kecil.)
// Rekursif(ada 2 rekursif, 1.(Mengurut bagian kiri dari pivot), 2. (Mengurut bagian kanan dari pivot)
void sortFilmByRating(Film films[], int low, int high) {
    if (low < high) {
        double pivot = films[high].rating;
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (films[j].rating > pivot) {
                i++;
                swap(films[i], films[j]);
            }
        }

        swap(films[i + 1], films[high]);

        int pivotIndex = i + 1;
        sortFilmByRating(films, low, pivotIndex - 1); // Rekursif
        sortFilmByRating(films, pivotIndex + 1, high); // Rekursif
    }
}

// Sebuah searching Binary Search (Mencari indeks suatu film berdassarkan judulnya di dalam array 'films'
// Parameter fungsi (films[](array), low(batas bawah), high(batas atas), judul)
// Logika pencariannya(rekursif(mencari judul), Iterasi(memeriksa judul film pada indeks tengah))
int binarySearchByTitle(Film films[], int low, int high, string judul) {
    if (low <= high) {
        int mid = low + (high - low) / 2;
        if (films[mid].judul == judul) {
            return mid;
        }
        if (films[mid].judul < judul) {
            return binarySearchByTitle(films, mid + 1, high, judul);
        }
        return binarySearchByTitle(films, low, mid - 1, judul);
    }
    return -1;
}

// untuk mencari informasi film berdasarkan judul, Ada Variable lokal (judul, count(menghitung jumlah film dalam database))
// Membaca data film dari file eksternal ('data_film.txt') menggunakan loop while(membaca setiap baris dalam file) dan menghitung jumlah film dengan memperhatikan line pembatas
// Alokasi memori dan pengisian data film(buat array film dengan ukuran count, menggunakan loop while untuk baca setiap baris, disimpan di array film
// Menutup file(file.close()), memeriksa jumlah film, Mengurutkan data film berdasarkan judul (sort() secara ascending)
// Mencari secara biner menggunakan(binarySearchByTitle(), Menampilkan hasil pencarian(result), dan menghapus memori yang dialokasikan(hapus memori untuk array)
void cariFilmJudul() {
    string judul;
    cout << "Masukkan judul film: ";
    cin.ignore();
    getline(cin, judul);

    ifstream file("data_film.txt");
    string line;
    int count = 0;

    while (getline(file, line)) {
        if (line == "---------------------------------------") {
            count++;
        }
    }

    Film* films = new Film[count];
    int index = 0;

    file.clear();
    file.seekg(0, ios::beg);

    string filmJudul, genre;
    int tahun, durasi;
    double rating;

    while (getline(file, line)) {
        if (line.find("Judul: ") != string::npos) {
            filmJudul = line.substr(7);
        } else if (line.find("Genre: ") != string::npos) {
            genre = line.substr(7);
        } else if (line.find("Tahun: ") != string::npos) {
            tahun = stoi(line.substr(7));
        } else if (line.find("Durasi: ") != string::npos) {
            durasi = stoi(line.substr(8));
        } else if (line.find("Rating: ") != string::npos) {
            rating = stod(line.substr(8));
        } else if (line == "---------------------------------------") {
            films[index].judul = filmJudul;
            films[index].genre = genre;
            films[index].tahun = tahun;
            films[index].durasi = durasi;
            films[index].rating = rating;
            index++;
        }
    }

    file.close();

    if (index == 0) {
        cout << "Tidak ada film dalam database." << endl;
        return;
    }

    // Sort films secara ascending mode
    sort(films, films + index, [](const Film& a, const Film& b) {
        return a.judul < b.judul;
    });

    int result = binarySearchByTitle(films, 0, index - 1, judul);

    if (result != -1) {
        cout << "\nFilm ditemukan:" << endl;
        cout << "Judul: " << films[result].judul << endl;
        cout << "Genre: " << films[result].genre << endl;
        cout << "Tahun: " << films[result].tahun << endl;
        cout << "Durasi: " << films[result].durasi << " menit" << endl;
        cout << "Rating: " << films[result].rating << endl;
        cout << "---------------------------------------" << endl;
    } else {
        cout << "Film tidak ditemukan." << endl;
    }

    delete[] films;
}

void cariFilmTahun(){
    int searchChoice;
    cout << "\n1. Terlama" << endl;
    cout << "2. Terbaru" << endl;
    cout << "Masukkan pilihan: ";
    cin >> searchChoice;

    ifstream file("data_film.txt");
    string line;
    int count = 0;

    while (getline(file, line)) {
        if (line == "---------------------------------------") {
            count++;
         }
    }

    Film* films = new Film[count];
    int index = 0;

    file.clear();
    file.seekg(0, ios::beg);

    string judul, genre;
    int tahun, durasi;
    double rating;

    while (getline(file, line)) {
         if (line.find("Judul: ") != string::npos) {
            judul = line.substr(7);
        }else if (line.find("Genre: ") != string::npos) {
            genre = line.substr(7);
        }else if (line.find("Tahun: ") != string::npos) {
            tahun = stoi(line.substr(7));
        }else if (line.find("Durasi: ") != string::npos) {
            durasi = stoi(line.substr(8));
        }else if (line.find("Rating: ") != string::npos) {
            rating = stod(line.substr(8));
        }else if (line == "---------------------------------------") {
            films[index].judul = judul;
            films[index].genre = genre;
            films[index].tahun = tahun;
            films[index].durasi = durasi;
            films[index].rating = rating;
            index++;
        }
    }

    file.close();

    if (index == 0) {
        cout << "Tidak ada film dalam database." << endl;
        return;
    }

    // Sort film berdasarkan terbaru dan terlama
    if (searchChoice == 1) {
        sort(films, films + index, [](const Film& a, const Film& b){
            return a.tahun < b.tahun;
        });
    }else if (searchChoice == 2) {
        sort(films, films + index, [](const Film& a, const Film& b) {
            return a.tahun > b.tahun;
        });
    }else {
        cout << "Pilihan tidak valid." << endl;
        return;
    }

    cout << "\nFilm Terurut berdasarkan Tahun ";
    if (searchChoice == 1) {
        cout << "(Terlama):" << endl;
    }else if (searchChoice == 2) {
        cout << "(Terbaru):" << endl;
    }

    for (int i = 0; i < index; i++) {
        cout << "Judul: " << films[i].judul << endl;
        cout << "Genre: " << films[i].genre << endl;
        cout << "Tahun: " << films[i].tahun << endl;
        cout << "Durasi: " << films[i].durasi << " menit" << endl;
        cout << "Rating: " << films[i].rating << endl;
        cout << "---------------------------------------" << endl;
    }

    delete[] films;
}

void cariFilmAbjad() {
    ifstream file("data_film.txt");
    string line;

    if (file.is_open()) {
        map<char, set<string>> filmByHurufDepan;

        while (getline(file, line)) {
            if (line.find("Judul: ") != string::npos) {
                string judul = line.substr(7);
                char hurufDepan = judul[0];

                // Menambahkan judul film ke set dalam map berdasarkan huruf depan
                filmByHurufDepan[hurufDepan].insert(judul);
            }
        }

        file.close();

        // Menampilkan daftar huruf depan dan judul film yang terkait
        for (const auto& pair : filmByHurufDepan) {
            cout << "Huruf Depan: " << pair.first << endl;
            cout << "Daftar Judul Film:\n" << endl;

            for (const string& judul : pair.second) {
                cout << judul << endl;
            }

            cout << "\n-----------------------" << endl;
        }
    } else {
        cout << "Tidak dapat membuka file data_film.txt" << endl;
    }
}

// Memproses file dengan ifstream, terdapat struktur data yang digunakan untuk menyimpan informasi film yang ditemukan di file
// Menyimpan data film kedalam array secara dinamis, mengurutkan film dengan menggunakan sort(Terbaru atau terlama)
// Menghapus memori dari alokasi memori array films
void mencariFilm(){
    int searchChoice;
    cout << "\n1. Berdasarkan Judul" << endl;
    cout << "2. Berdasarkan Tahun" << endl;
    cout << "3. Berdasarkan Abjad" << endl;
    cout << "Masukkan pilihan: ";
    cin >> searchChoice;

    if (searchChoice == 1) {
        cariFilmJudul();
    }   
    else if (searchChoice == 2) {
        cariFilmTahun();
    }
    else if (searchChoice == 3){
        cariFilmAbjad();
    }else {
        cout << "Pilihan tidak valid." << endl;
    }
}

//Menginput genre yang dicari, menginput jumlah film yang akan direkomendasikan, memproses file(hanya film sesuai kriteria yang akan dimasukkan ke array)
// Struktur data film untuk disimpan kedalam array
// Output hasil film yang direkomendasikan dan menghapus memori.
void rekomendasikanFilm() {
    string genre;
    cout << "Genre yang tersedia : \n";
    cout << "Drama Crime Action Sci-Fi Fantasy\nAnimation Thriller Adventure Romance Musical\nComedy Western Biography Horror Mystery" << endl;
    cout << "\nMasukkan genre yang dicari: ";
    cin.ignore();
    getline(cin, genre);

    int jumlahFilm;
    cout << "\nBerapa film yang ingin direkomendasikan: ";
    cin >> jumlahFilm;

    ifstream file("data_film.txt");
    string line;
    int count = 0;

    while (getline(file, line)) {
        if (line == "---------------------------------------") {
            count++;
        }
    }

    Film* films = new Film[count];
    int index = 0;

    file.clear();
    file.seekg(0, ios::beg);

    string judul, filmGenre;
    int tahun, durasi;
    double rating;

    while (getline(file, line)) {
        if (line.find("Judul: ") != string::npos) {
            judul = line.substr(7);
        } else if (line.find("Genre: ") != string::npos) {
            filmGenre = line.substr(7);
        } else if (line.find("Tahun: ") != string::npos) {
            tahun = stoi(line.substr(7));
        } else if (line.find("Durasi: ") != string::npos) {
            durasi = stoi(line.substr(8));
        } else if (line.find("Rating: ") != string::npos) {
            rating = stod(line.substr(8));
        } else if (line == "---------------------------------------") {
            if (filmGenre == genre) {
                films[index].judul = judul;
                films[index].genre = filmGenre;
                films[index].tahun = tahun;
                films[index].durasi = durasi;
                films[index].rating = rating;
                index++;
            }
        }
    }

    file.close();

    if (index == 0) {
        cout << "Tidak ada film dengan genre tersebut." << endl;
        return;
    }

    sortFilmByRating(films, 0, index - 1);

    cout << "\nFilm Rekomendasi berdasarkan Genre '" << genre << "':" << endl;

    // Batasi jumlah film yang ditampilkan sesuai dengan input pengguna
    for (int i = 0; i < min(jumlahFilm, index); i++) {
        cout << "Judul: " << films[i].judul << endl;
        cout << "Genre: " << films[i].genre << endl;
        cout << "Tahun: " << films[i].tahun << endl;
        cout << "Durasi: " << films[i].durasi << " menit" << endl;
        cout << "Rating: " << films[i].rating << endl;
        cout << "---------------------------------------" << endl;
    }

    delete[] films;
}

// Input Passowrd, pilihan menu, perulangan menu(do while), switch statement(tambahkan, tampilkan, hapus, tampilkan guest)
// Input tambahan untuk menghapus film
void adminMenu() {
    string password;
    cout << "Masukkan password: ";
    cin >> password;

    if (password != "punyaku121") {
        cout << "Password salah. Akses ditolak." << endl;
        return;
    }

    int choice;

    do {
        cout << "\n==============================" << endl;
        cout << "DATABASE FILMRATE" << endl;
        cout << "==============================\n" << endl;
        cout << "1. Tambahkan Film" << endl;
        cout << "2. Tampilkan Film" << endl;
        cout << "3. Menghapus Film" << endl;
        cout << "4. Menampilkan Pengunjung" << endl;

        cout << "0. Keluar" << endl;
        cout << "------------------------------\n" << endl;
        cout << "Masukkan pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cin.ignore();
                tambahkanFilm();
                break;
            case 2:
                tampilkanFilm();
                break;
            case 3: {
                string judulFilm;
                string genreFilm;
                cin.ignore();
                cout << "Masukkan judul film yang akan dihapus: ";
                getline(cin, judulFilm);
                cout << "Masukkan genre film yang akan dihapus: ";
                getline(cin, genreFilm);
                hapusFilm(judulFilm, genreFilm);
                break;
            }
            case 4:
                tampilkanGuest();
                break;
            case 0:
                cout << "Keluar dari admin mode." << endl;
                return;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while (choice != 0);
}

// Menginput nama pengunjung yang nanti akan dicatat kedalam file guest_data.txt (ofstream (ios::app))
// Pilihan menu menggunakan do while dan switch statement(mencariFilm(), rekomendasiFilm())
void guestMenu() {
    string nama;
    cout << "Masukkan Nama Anda: ";
    cin.ignore();
    getline(cin, nama);


    ofstream file("guest_data.txt", ios::app);
    time_t now = time(0);
    string waktu = ctime(&now);

    file << "Nama: " << nama << ", Waktu: " << waktu;
    file.close();

    int choice;

    do {
        cout << "\n==============================" << endl;
        cout << "SELAMAT DATANG DI FILMRATE" << endl;
        cout << "==============================\n" << endl;
        cout << "1. Mencari Film" << endl;
        cout << "2. Rekomendasikan Film" << endl;

        cout << "0. Keluar" << endl;
        cout << "-------------------------------\n" << endl;
        cout << "Masukkan pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                mencariFilm();
                break;
            }

            case 2:
                rekomendasikanFilm();
                break;

            case 0:
                cout << "Keluar dari guest mode." << endl;
                return;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while(choice != 0);
}